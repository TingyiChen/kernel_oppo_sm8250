// SPDX-License-Identifier: GPL-2.0-only

/*
 * Copyright (c) 2011-2018, The Linux Foundation. All rights reserved.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include <asm/arch_timer.h>

#define RPM_STATS_NUM_REC	2
#define MSM_ARCH_TIMER_FREQ	19200000

#ifdef VENDOR_EDIT
//Nanwei.Deng@BSP.Power.Basic 2018/06/11 add for get rpm_stats
void __iomem *rpm_phys_addr = NULL;
#endif

#define GET_PDATA_OF_ATTR(attr) \
	(container_of(attr, struct msm_rpmstats_kobj_attr, ka)->pd)

struct msm_rpmstats_record {
	char name[32];
	u32 id;
	u32 val;
};

struct msm_rpmstats_platform_data {
	phys_addr_t phys_addr_base;
	u32 phys_size;
	u32 num_records;
};

struct msm_rpmstats_private_data {
	void __iomem *reg_base;
	u32 num_records;
	u32 read_idx;
	u32 len;
	char buf[480];
	struct msm_rpmstats_platform_data *platform_data;
};

struct msm_rpm_stats_data {
	u32 stat_type;
	u32 count;
	u64 last_entered_at;
	u64 last_exited_at;
	u64 accumulated;
#if defined(CONFIG_MSM_RPM_SMD)
	u32 client_votes;
	u32 reserved[3];
#endif

};

struct msm_rpmstats_kobj_attr {
	struct kobject *kobj;
	struct kobj_attribute ka;
	struct msm_rpmstats_platform_data *pd;
};

static inline u64 get_time_in_sec(u64 counter)
{
	do_div(counter, MSM_ARCH_TIMER_FREQ);

	return counter;
}
#ifndef VENDOR_EDIT //yunqing.zeng@bsp.power.basic 20190702 Modify for log info more accurate
static inline u64 get_time_in_msec(u64 counter)
{
	do_div(counter, MSM_ARCH_TIMER_FREQ);
	counter *= MSEC_PER_SEC;

	return counter;
}
#else
static inline u64 get_time_in_msec(u64 counter)
{
	do_div(counter, (MSM_ARCH_TIMER_FREQ/MSEC_PER_SEC));
	return counter;
}
#endif

static inline int msm_rpmstats_append_data_to_buf(char *buf,
		struct msm_rpm_stats_data *data, int buflength)
{
	char stat_type[5];
	u64 time_in_last_mode;
	u64 time_since_last_mode;
	u64 actual_last_sleep;

	stat_type[4] = 0;
	memcpy(stat_type, &data->stat_type, sizeof(u32));

	time_in_last_mode = data->last_exited_at - data->last_entered_at;
	time_in_last_mode = get_time_in_msec(time_in_last_mode);
	time_since_last_mode = arch_counter_get_cntvct() - data->last_exited_at;
	time_since_last_mode = get_time_in_sec(time_since_last_mode);
	actual_last_sleep = get_time_in_msec(data->accumulated);

#if defined(CONFIG_MSM_RPM_SMD)
	return scnprintf(buf, buflength,
		"RPM Mode:%s\n\t count:%d\ntime in last mode(msec):%llu\n"
		"time since last mode(sec):%llu\nactual last sleep(msec):%llu\n"
		"client votes: %#010x\n\n",
		stat_type, data->count, time_in_last_mode,
		time_since_last_mode, actual_last_sleep,
		data->client_votes);
#else
	return scnprintf(buf, buflength,
		"RPM Mode:%s\n\t count:%d\ntime in last mode(msec):%llu\n"
		"time since last mode(sec):%llu\nactual last sleep(msec):%llu\n\n",
		stat_type, data->count, time_in_last_mode,
		time_since_last_mode, actual_last_sleep);
#endif
}

#ifdef VENDOR_EDIT
//Nanwei.Deng@BSP.Power.Basic 2018/06/11 add for get rpm_stats
static inline int oppo_rpmstats_append_data_to_buf(char *buf,
		struct msm_rpm_stats_data *data, int buflength,int i)
{
	u64 actual_last_sleep;

	actual_last_sleep = get_time_in_msec(data->accumulated);
	if(i == 0) {
		return snprintf(buf, buflength,
			"vlow:%x:%llx\n",
			data->count, actual_last_sleep);
	} else {
	    return snprintf(buf, buflength,
			"vmin:%x:%llx\r\n",
			data->count, actual_last_sleep);
	}
}
#endif /*VENDOR_EDIT*/

static inline u32 msm_rpmstats_read_long_register(void __iomem *regbase,
		int index, int offset)
{
	return readl_relaxed(regbase + offset +
			index * sizeof(struct msm_rpm_stats_data));
}

static inline u64 msm_rpmstats_read_quad_register(void __iomem *regbase,
		int index, int offset)
{
	u64 dst;

	memcpy_fromio(&dst,
		regbase + offset + index * sizeof(struct msm_rpm_stats_data),
		8);
	return dst;
}

static inline int msm_rpmstats_copy_stats(
			struct msm_rpmstats_private_data *prvdata)
{
	void __iomem *reg;
	struct msm_rpm_stats_data data;
	int i, length;

	reg = prvdata->reg_base;

	for (i = 0, length = 0; i < prvdata->num_records; i++) {
		data.stat_type = msm_rpmstats_read_long_register(reg, i,
				offsetof(struct msm_rpm_stats_data,
					stat_type));
		data.count = msm_rpmstats_read_long_register(reg, i,
				offsetof(struct msm_rpm_stats_data, count));
		data.last_entered_at = msm_rpmstats_read_quad_register(reg,
				i, offsetof(struct msm_rpm_stats_data,
					last_entered_at));
		data.last_exited_at = msm_rpmstats_read_quad_register(reg,
				i, offsetof(struct msm_rpm_stats_data,
					last_exited_at));
		data.accumulated = msm_rpmstats_read_quad_register(reg,
				i, offsetof(struct msm_rpm_stats_data,
					accumulated));
#if defined(CONFIG_MSM_RPM_SMD)
		data.client_votes = msm_rpmstats_read_long_register(reg,
				i, offsetof(struct msm_rpm_stats_data,
					client_votes));
#endif

		length += msm_rpmstats_append_data_to_buf(prvdata->buf + length,
				&data, sizeof(prvdata->buf) - length);
		prvdata->read_idx++;
	}

	return length;
}

#ifdef VENDOR_EDIT
//Nanwei.Deng@BSP.Power.Basic 2018/06/11 add for get rpm_stats
static inline int oppo_rpmstats_copy_stats(
			struct msm_rpmstats_private_data *prvdata)
{
	void __iomem *reg;
	struct msm_rpm_stats_data data;
	int i, length;

	reg = prvdata->reg_base;

	for (i = 0, length = 0; i < prvdata->num_records; i++) {
		data.count = msm_rpmstats_read_long_register(reg, i,
				offsetof(struct msm_rpm_stats_data, count));
		data.accumulated = msm_rpmstats_read_quad_register(reg,
				i, offsetof(struct msm_rpm_stats_data,
					accumulated));

		length += oppo_rpmstats_append_data_to_buf(prvdata->buf + length,
				&data, sizeof(prvdata->buf) - length,i);
		prvdata->read_idx++;
	}

	return length;
}
#endif /*VENDOR_EDIT*/

static ssize_t rpmstats_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	struct msm_rpmstats_private_data prvdata;
	struct msm_rpmstats_platform_data *pdata = NULL;
	ssize_t length;

	pdata = GET_PDATA_OF_ATTR(attr);

	prvdata.reg_base = ioremap_nocache(pdata->phys_addr_base,
					pdata->phys_size);
	if (!prvdata.reg_base) {
		pr_err("ERROR could not ioremap start=%pa, len=%u\n",
				&pdata->phys_addr_base, pdata->phys_size);
		return -EBUSY;
	}

	prvdata.read_idx = prvdata.len = 0;
	prvdata.platform_data = pdata;
	prvdata.num_records = pdata->num_records;

	if (prvdata.read_idx < prvdata.num_records)
		prvdata.len = msm_rpmstats_copy_stats(&prvdata);

	length = scnprintf(buf, prvdata.len, "%s", prvdata.buf);
	iounmap(prvdata.reg_base);
	return length;
}
#ifdef VENDOR_EDIT
//Nanwei.Deng@BSP.Power.Basic 2018/06/11 add for get rpm_stats
static ssize_t oppo_rpmstats_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	struct msm_rpmstats_private_data prvdata;
	struct msm_rpmstats_platform_data *pdata = NULL;

	if (rpm_phys_addr == NULL)
	{
		return 0;
	}
	pdata = GET_PDATA_OF_ATTR(attr);
    prvdata.reg_base =rpm_phys_addr; 


	prvdata.read_idx = prvdata.len = 0;
	prvdata.platform_data = pdata;
	prvdata.num_records = RPM_STATS_NUM_REC;

	if (prvdata.read_idx < prvdata.num_records)
		prvdata.len = oppo_rpmstats_copy_stats(&prvdata);

	return snprintf(buf, prvdata.len, "%s", prvdata.buf);
}

#endif /*VENDOR_EDIT*/

static int msm_rpmstats_create_sysfs(struct platform_device *pdev,
				struct msm_rpmstats_platform_data *pd)
{
	struct kobject *rpmstats_kobj = NULL;
	struct msm_rpmstats_kobj_attr *rpms_ka = NULL;
	int ret = 0;
#ifdef VENDOR_EDIT
//Nanwei.Deng@BSP.Power.Basic 2018/05/23 add for get /sys/power/system_sleep/oppo_rpmh_stats
    struct msm_rpmstats_kobj_attr *oppo_rpms_ka = NULL;
#endif

	rpmstats_kobj = kobject_create_and_add("system_sleep", power_kobj);
	if (!rpmstats_kobj) {
		pr_err("Cannot create rpmstats kobject\n");
		ret = -ENOMEM;
		goto fail;
	}

	rpms_ka = kzalloc(sizeof(*rpms_ka), GFP_KERNEL);
	if (!rpms_ka) {
		kobject_put(rpmstats_kobj);
		ret = -ENOMEM;
		goto fail;
	}

	rpms_ka->kobj = rpmstats_kobj;

	sysfs_attr_init(&rpms_ka->ka.attr);
	rpms_ka->pd = pd;
	rpms_ka->ka.attr.mode = 0444;
	rpms_ka->ka.attr.name = "stats";
	rpms_ka->ka.show = rpmstats_show;
	rpms_ka->ka.store = NULL;

	ret = sysfs_create_file(rpmstats_kobj, &rpms_ka->ka.attr);
	platform_set_drvdata(pdev, rpms_ka);
#ifdef VENDOR_EDIT
//Nanwei.Deng@BSP.Power.Basic 2018/05/23 add for get /sys/power/system_sleep/oppo_rpmh_stats
    oppo_rpms_ka = kzalloc(sizeof(*oppo_rpms_ka), GFP_KERNEL);
	if (!oppo_rpms_ka) {
		kobject_put(rpmstats_kobj);
		ret = -ENOMEM;
		goto fail;
	}

    sysfs_attr_init(&oppo_rpms_ka->ka.attr);
	oppo_rpms_ka->pd = pd;
	oppo_rpms_ka->ka.attr.mode = 0444;
	oppo_rpms_ka->ka.attr.name = "oppo_rpmh_stats";
	oppo_rpms_ka->ka.show = oppo_rpmstats_show;
	oppo_rpms_ka->ka.store = NULL;

	ret = sysfs_create_file(rpmstats_kobj, &oppo_rpms_ka->ka.attr);
#endif /*VENDOR_EDIT*/

fail:
	return ret;
}

static int msm_rpmstats_probe(struct platform_device *pdev)
{
	struct msm_rpmstats_platform_data *pdata;
	struct resource *res = NULL, *offset = NULL;
	u32 offset_addr = 0;
	void __iomem *phys_ptr = NULL;
	char *key;

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	key = "phys_addr_base";
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, key);
	if (!res)
		return -EINVAL;

	key = "offset_addr";
	offset = platform_get_resource_byname(pdev, IORESOURCE_MEM, key);
	if (offset) {
		/* Remap the rpm-stats pointer */
		phys_ptr = ioremap_nocache(offset->start, SZ_4);
		if (!phys_ptr) {
			pr_err("Failed to ioremap offset address\n");
			return -ENODEV;
		}
		offset_addr = readl_relaxed(phys_ptr);
		iounmap(phys_ptr);
	}

	pdata->phys_addr_base  = res->start + offset_addr;
	pdata->phys_size = resource_size(res);

	key = "qcom,num-records";
	if (of_property_read_u32(pdev->dev.of_node, key, &pdata->num_records))
		pdata->num_records = RPM_STATS_NUM_REC;

	msm_rpmstats_create_sysfs(pdev, pdata);

#ifdef VENDOR_EDIT
	//Nanwei.Deng@BSP.Power.Basic 2018/05/23 add for get /sys/power/system_sleep/oppo_rpmh_stats
	rpm_phys_addr= ioremap_nocache(pdata->phys_addr_base,
							pdata->phys_size);
	if (!rpm_phys_addr) {
			pr_err("%s: ERROR could not ioremap start=%pa, len=%u\n",
			__func__, &pdata->phys_addr_base,
			pdata->phys_size);
		return -ENODEV;
	}
#endif
	return 0; 

}

static int msm_rpmstats_remove(struct platform_device *pdev)
{
	struct msm_rpmstats_kobj_attr *rpms_ka;

	if (!pdev)
		return -EINVAL;

	rpms_ka = (struct msm_rpmstats_kobj_attr *)
			platform_get_drvdata(pdev);

	sysfs_remove_file(rpms_ka->kobj, &rpms_ka->ka.attr);
	kobject_put(rpms_ka->kobj);
	platform_set_drvdata(pdev, NULL);

	return 0;
}


static const struct of_device_id rpm_stats_table[] = {
	{ .compatible = "qcom,rpm-stats" },
	{ },
};

static struct platform_driver msm_rpmstats_driver = {
	.probe = msm_rpmstats_probe,
	.remove = msm_rpmstats_remove,
	.driver = {
		.name = "msm_rpm_stat",
		.owner = THIS_MODULE,
		.of_match_table = rpm_stats_table,
	},
};
builtin_platform_driver(msm_rpmstats_driver);
