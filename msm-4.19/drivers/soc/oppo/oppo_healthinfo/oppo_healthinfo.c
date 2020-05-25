/**********************************************************************************
* Copyright (c)  2008-2015  Guangdong OPPO Mobile Comm Corp., Ltd
* VENDOR_EDIT
* Description:     Healthinfo Monitor  Kernel Driver
*
* Version    : 2.0
* Date       : 2018-11-01
* Author     : wenbin.liu@PSW.Platform.Kernel
* ------------------------------ Revision History: --------------------------------
* <version>           <date>                <author>                            <desc>
* Revision 1.0        2018-05-24       wenbin.liu@PSW.Platform.Kernel      Created for Healthinfomonitor
* Revision 2.0        2018-11-01       wenbin.liu@PSW.Platform.Kernel      2.0 Feature
***********************************************************************************/

#include <soc/oppo/oppo_healthinfo.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/ratelimit.h>
#include <linux/blkdev.h>
#ifdef CONFIG_OPPO_MEM_MONITOR
#include <linux/memory_monitor.h>
#endif
#ifdef CONFIG_UFSFEATURE
#include "../../../drivers/scsi/ufs/ufsfeature.h"
#endif
struct sched_stat_para {
        bool ctrl;
        bool logon;
        bool trig;
        int low_thresh_ms;
        int high_thresh_ms;
        u64 low_cnt;
        u64 high_cnt;
        u64 total_ms;
        u64 total_cnt;
        u64 fg_low_cnt;
        u64 fg_high_cnt;
        u64 fg_total_ms;
        u64 fg_total_cnt;
        u64 fg_max_delta_ms;
        u64 delta_ms;
};

struct io_latency_para{
        bool ctrl;
        bool logon;
        bool trig;

        int low_thresh_ms;
        u64 low_cnt;

        int high_thresh_ms;
        u64 high_cnt;

        u64 total_us;
        u64 emmc_total_us;
        u64 total_cnt;
        u64 fg_low_cnt;
        u64 fg_high_cnt;
        u64 fg_total_ms;
        u64 fg_total_cnt;
        u64 fg_max_delta_ms;
        u64 delta_ms;

        //fg
        u64 iosize_write_count_fg;
        u64 iosize_write_us_fg;
        u64 iosize_500ms_syncwrite_count_fg;
        u64 iosize_200ms_syncwrite_count_fg;
        u64 iosize_500ms_asyncwrite_count_fg;
        u64 iosize_200ms_asyncwrite_count_fg;
        u64 iosize_read_count_fg;
        u64 iosize_read_us_fg;
        u64 iosize_500ms_read_count_fg;
        u64 iosize_200ms_read_count_fg;
        //bg
        u64 iosize_write_count_bg;
        u64 iosize_write_us_bg;
        u64 iosize_2s_asyncwrite_count_bg;
        u64 iosize_500ms_asyncwrite_count_bg;
        u64 iosize_200ms_asyncwrite_count_bg;
        u64 iosize_2s_syncwrite_count_bg;
        u64 iosize_500ms_syncwrite_count_bg;
        u64 iosize_200ms_syncwrite_count_bg;
        u64 iosize_read_count_bg;
        u64 iosize_read_us_bg;
        u64 iosize_2s_read_count_bg;
        u64 iosize_500ms_read_count_bg;
        u64 iosize_200ms_read_count_bg;

		  //4k
        u64 iosize_4k_read_count;
        u64 iosize_4k_read_us;
        u64 iosize_4k_write_count;
        u64 iosize_4k_write_us;
};
struct io_latency_para oppo_io_para;
struct sched_stat_para oppo_sched_para[OHM_SCHED_TOTAL];
static char *sched_list[OHM_TYPE_TOTAL] = {
        /* SCHED_STATS 0 -11 */
        "iowait",
        "sched_latency",
        "fsync",
        "emmcio",
        "dstate",
        "sched_default_05",
        "sched_default_06",
        "sched_default_07",
        "sched_default_08",
        "sched_default_09",
        "sched_default_10",
        "sched_default_11",
        /* OTHER_TYPE 12 - */
        "cur_cpu_load",
        "memory_monitor",
        "io_panic",
        "svm_monitor",
        "rlimit_monitor",
        "ionwait_monitor"
};

/******  Action  ******/
#define MAX_OHMEVENT_PARAM 4
#define OH_MSG_LEN 256
static struct kobject *ohm_kobj = NULL;
static struct work_struct ohm_detect_ws;
static char *ohm_detect_env[MAX_OHMEVENT_PARAM] = { "OHMACTION=uevent", NULL };
static bool ohm_action_ctrl = false;
static char msg_buf[OH_MSG_LEN] = {0};

void ohm_action_trig(int type)
{
        if (!ohm_action_ctrl) {
                ohm_err("ctrl off\n");
                return;
        }
        ohm_debug("%s trig action\n", sched_list[type]);
        if (OHM_MEM_MON == type || OHM_SCHED_FSYNC == type) {
                if (!ohm_kobj) {
                        ohm_err("kobj NULL\n");
                        return;
                }
                sprintf(ohm_detect_env[1], "OHMTYPE=%s", sched_list[type]);
		ohm_detect_env[MAX_OHMEVENT_PARAM - 2] = NULL;
                ohm_detect_env[MAX_OHMEVENT_PARAM - 1] = NULL;
                schedule_work(&ohm_detect_ws);
        }
}

void ohm_action_trig_with_msg(int type, char *msg)
{
	int len;

        if (!ohm_action_ctrl) {
                ohm_err("ctrl off\n");
                return;
        }

	if (!ohm_kobj) {
                ohm_err("kobj NULL\n");
                return;
        }

        if (OHM_SVM_MON == type || OHM_RLIMIT_MON == type) {
                sprintf(ohm_detect_env[1], "OHMTYPE=%s", sched_list[type]);
		len = snprintf(msg_buf, OH_MSG_LEN-1, "OHMMSG=%s", msg);
		msg_buf[len] = '\0';
		ohm_detect_env[MAX_OHMEVENT_PARAM - 2] = msg_buf;
		ohm_detect_env[MAX_OHMEVENT_PARAM - 1] = NULL;
                schedule_work(&ohm_detect_ws);
        }
}

void ohm_detect_work(struct work_struct *work)
{
        ohm_debug("Uevent Para: %s, %s\n", ohm_detect_env[0], ohm_detect_env[1]);
        kobject_uevent_env(ohm_kobj, KOBJ_CHANGE, ohm_detect_env);
        ohm_debug("Uevent Done!\n");
}

void ohm_action_init(void)
{
        int i = 0;
        for(i = 1; i < MAX_OHMEVENT_PARAM - 2; i++) {
                ohm_detect_env[i] = kzalloc(50, GFP_KERNEL);
                if (!ohm_detect_env[i]) {
                        ohm_err("kzalloc ohm uevent param failed\n");
                        goto ohm_action_init_free_memory;
                }
        }

        ohm_kobj = kset_find_obj(module_kset, KBUILD_MODNAME);
        if (!ohm_kobj) {
                goto ohm_action_init_kobj_failed;
        }
        INIT_WORK(&ohm_detect_ws, ohm_detect_work);
        ohm_debug("Success !\n");
        return;

ohm_action_init_kobj_failed:
        ohm_err("Ohm kobj init err\n");
ohm_action_init_free_memory:
        for(i--; i > 0; i--) {
                kfree(ohm_detect_env[i]);
        }
        ohm_err("Failed !\n");
}

/******  Sched record  ******/
void ohm_schedstats_record(int sched_type, int fg, u64 delta_ms)
{

        if (!oppo_sched_para[sched_type].ctrl)
                return;

        if (fg) {
                oppo_sched_para[sched_type].fg_total_ms += delta_ms;
                oppo_sched_para[sched_type].fg_total_cnt++;
                if (delta_ms > oppo_sched_para[sched_type].fg_max_delta_ms) {
                        oppo_sched_para[sched_type].fg_max_delta_ms = delta_ms;
                }
        }

        if (delta_ms >= oppo_sched_para[sched_type].high_thresh_ms) {
			 	static DEFINE_RATELIMIT_STATE(ratelimit, 60*HZ, 1);
                oppo_sched_para[sched_type].high_cnt++;

                if (oppo_sched_para[sched_type].logon && __ratelimit(&ratelimit)) {
                        ohm_debug_deferred("[%s / %s] high_cnt, delay = %llu ms\n",
                                sched_list[sched_type], (fg ? "fg":"bg"), delta_ms);
                }
                if (fg) {
                        oppo_sched_para[sched_type].fg_high_cnt++;
                        if (oppo_sched_para[sched_type].trig)
                                ohm_action_trig(sched_type);
                }
        } else if (delta_ms >= oppo_sched_para[sched_type].low_thresh_ms) {
                oppo_sched_para[sched_type].low_cnt++;
                if (fg) {
                        oppo_sched_para[sched_type].fg_low_cnt++;
                }
        }

        oppo_sched_para[sched_type].delta_ms = delta_ms;
        oppo_sched_para[sched_type].total_ms += delta_ms;
        oppo_sched_para[sched_type].total_cnt++;

        return;
}

/******  Flash IO Latency record  ******/
void ohm_iolatency_record(struct request *req, unsigned int nr_bytes, int fg, u64 delta_us)
{
        u64 delta_ms = delta_us/1000;

        if (!oppo_io_para.ctrl)
                return;
        if (!req)
                return;
        if (fg) {
                oppo_io_para.fg_total_ms += delta_ms;
                oppo_io_para.fg_total_cnt++;
                if (delta_ms > oppo_io_para.fg_max_delta_ms) {
                        oppo_io_para.fg_max_delta_ms = delta_ms;
                }
        }

        if (delta_ms >= oppo_io_para.high_thresh_ms) {
                oppo_io_para.high_cnt++;

                if (oppo_io_para.logon) {
                        ohm_debug("[io latency / %s] high_cnt, delay = %llu ms\n",
                                (fg ? "fg":"bg"), delta_ms);
                }
                if (fg) {
                                oppo_io_para.fg_high_cnt++;
                                if (oppo_io_para.trig)
                                      ohm_action_trig(OHM_SCHED_EMMCIO);
                          }
        } else if (delta_ms >= oppo_io_para.low_thresh_ms) {
                oppo_io_para.low_cnt++;
                if (fg) {
                        oppo_io_para.fg_low_cnt++;
                }
        }

		if(fg){
	        if((req_op(req)!=REQ_OP_DISCARD)&&(req_op(req)!= REQ_OP_SECURE_ERASE)){
                if(req_op(req) == REQ_OP_WRITE || req_op(req) == REQ_OP_WRITE_SAME){
                  oppo_io_para.iosize_write_count_fg++;
				  oppo_io_para.iosize_write_us_fg += delta_us;
				  if(rq_is_sync(req)){
                      if( delta_ms > 500 ){
                        oppo_io_para.iosize_500ms_syncwrite_count_fg ++;
				      }else if( delta_ms > 200 ){
                        oppo_io_para.iosize_200ms_syncwrite_count_fg ++;
				      }
				  }else{
					  if( delta_ms > 500 ){
					    oppo_io_para.iosize_500ms_asyncwrite_count_fg ++;
				      }else if( delta_ms > 200 ){
						oppo_io_para.iosize_200ms_asyncwrite_count_fg ++;
					  } 
				  }

                }else{
                  oppo_io_para.iosize_read_count_fg++;
				  oppo_io_para.iosize_read_us_fg += delta_us;
				  if( delta_ms > 500 ){
                     oppo_io_para.iosize_500ms_read_count_fg ++;
				  }else if( delta_ms > 200 ){
                     oppo_io_para.iosize_200ms_read_count_fg ++;
				  }
                }
			}      
		}else{
			if((req_op(req)!=REQ_OP_DISCARD)&&(req_op(req)!= REQ_OP_SECURE_ERASE)){
                if(req_op(req) == REQ_OP_WRITE || req_op(req) == REQ_OP_WRITE_SAME){
                  oppo_io_para.iosize_write_count_bg++;
				  oppo_io_para.iosize_write_us_bg += delta_us;
				  if(rq_is_sync(req)){
					  if( delta_ms > 2000 )
					  {
                      oppo_io_para.iosize_2s_syncwrite_count_bg ++;
						if (oppo_io_para.trig)
	                            ohm_action_trig(OHM_SCHED_EMMCIO);
					  }else if( delta_ms > 500 ){
	                     oppo_io_para.iosize_500ms_syncwrite_count_bg ++;
					  }else if( delta_ms > 200 ){
	                     oppo_io_para.iosize_200ms_syncwrite_count_bg ++;
					  }
				  }else{
					  if( delta_ms > 2000 )
					 {
						oppo_io_para.iosize_2s_asyncwrite_count_bg ++;
						if (oppo_io_para.trig)
								ohm_action_trig(OHM_SCHED_EMMCIO);
						}else if( delta_ms > 500 ){
					       oppo_io_para.iosize_500ms_asyncwrite_count_bg ++;
						}else if( delta_ms > 200 ){
						   oppo_io_para.iosize_200ms_asyncwrite_count_bg ++;
						}
				  }
                }else{
                  oppo_io_para.iosize_read_count_bg++;
				  oppo_io_para.iosize_read_us_bg += delta_us;
				  if( delta_ms > 2000 )
				  {
                    oppo_io_para.iosize_2s_read_count_bg ++;
					if (oppo_io_para.trig)
                            ohm_action_trig(OHM_SCHED_EMMCIO);
				  }else if( delta_ms > 500 ){
                     oppo_io_para.iosize_500ms_read_count_bg ++;
				  }else if( delta_ms > 200 ){
                     oppo_io_para.iosize_200ms_read_count_bg ++;
				  }
            }
			}

		  }
        //4k
        if((req_op(req)!=REQ_OP_DISCARD)&&(req_op(req)!= REQ_OP_SECURE_ERASE)){
            if(req_op(req) == REQ_OP_WRITE || req_op(req) == REQ_OP_WRITE_SAME){
               if( blk_rq_bytes(req) == 4096){
                  oppo_io_para.iosize_4k_write_count++;
                  oppo_io_para.iosize_4k_write_us += delta_us;
               }
            }
            else{
              if( blk_rq_bytes(req) == 4096){
                 oppo_io_para.iosize_4k_read_count++;
                 oppo_io_para.iosize_4k_read_us += delta_us;
              }
            }
        }
        oppo_io_para.delta_ms = delta_ms;
        oppo_io_para.total_us += delta_us;
        oppo_io_para.emmc_total_us += req->flash_io_latency;
        oppo_io_para.total_cnt++;

        return;
}


/****  Ctrl init  ****/
/*
        CTRL - TOTAL -32
        CTRL0:          logon
iowait record;

        CTRL1:
sched latency;

        CTRL2:          logon           trig
fsync record;

        CTRL3:
emmcio record;
        ******
        ******
        CTRL12:
cpu load cur;

        CTRL13:         logon           trig
mem mon;
......;
......;
        CTRL31:
                ......;
*/
#define OHM_LIST_MAGIC          0x5a000000
#define OHM_CTRL_MAX            32
#define OHM_INT_MAX             20
#define OHM_CTRL_IOWAIT         BIT(OHM_SCHED_IOWAIT)
#define OHM_CTRL_SCHEDLATENCY   BIT(OHM_SCHED_SCHEDLATENCY)
#define OHM_CTRL_FSYNC          BIT(OHM_SCHED_FSYNC)
#define OHM_CTRL_EMMCIO         BIT(OHM_SCHED_EMMCIO)
#define OHM_CTRL_DSTATE         BIT(OHM_SCHED_DSTATE)
#define OHM_CTRL_SCHEDTOTAL     OHM_CTRL_EMMCIO | OHM_CTRL_FSYNC | OHM_CTRL_SCHEDLATENCY | OHM_CTRL_IOWAIT | OHM_CTRL_DSTATE
#define OHM_CTRL_CPU_CUR        BIT(OHM_CPU_LOAD_CUR)
#define OHM_CTRL_MEMMON         BIT(OHM_MEM_MON)
#define OHM_CTRL_IOPANIC_MON    BIT(OHM_IOPANIC_MON)
#define OHM_CTRL_SVM			BIT(OHM_SVM_MON)
#define OHM_CTRL_RLIMIT			BIT(OHM_RLIMIT_MON)
#define OHM_CTRL_IONMON         BIT(OHM_ION_MON)


/*
ohm_ctrl_list    = 0x5a0fffff
ohm_logon_list = 0x5a002005
ohm_trig_list    = 0x5a002000
*/

/*Default*/
static int ohm_ctrl_list = OHM_LIST_MAGIC | OHM_CTRL_CPU_CUR | OHM_CTRL_MEMMON |  OHM_CTRL_IONMON | OHM_CTRL_SCHEDTOTAL;
static int ohm_logon_list = OHM_LIST_MAGIC;
static int ohm_trig_list = OHM_LIST_MAGIC;

bool ohm_cpu_ctrl = true;
bool ohm_cpu_logon = false;
bool ohm_cpu_trig = false;

bool ohm_memmon_ctrl = true;
bool ohm_memmon_logon = false;
bool ohm_memmon_trig = false;

bool ohm_iopanic_mon_ctrl = true;
bool ohm_iopanic_mon_logon = false;
bool ohm_iopanic_mon_trig = false;

bool ohm_ionmon_ctrl = true;
bool ohm_ionmon_logon = false;
bool ohm_ionmon_trig = false;

/******  Para Update  *****/
#define LOW_THRESH_MS_DEFAULT   100
#define HIGH_THRESH_MS_DEFAULT  500
/* low thresh 10~1000ms*/
#define LOW_THRESH_MS_LOW       10
#define LOW_THRESH_MS_HIGH      1000
/* high thresh 100~5000ms*/
#define HIGH_THRESH_MS_LOW      50
#define HIGH_THRESH_MS_HIGH     5000

struct thresh_para {
        int l_ms;
        int h_ms;
};

struct thresh_para ohm_thresh_para[OHM_SCHED_TOTAL] = {
        { LOW_THRESH_MS_DEFAULT, HIGH_THRESH_MS_DEFAULT},
        { LOW_THRESH_MS_DEFAULT, HIGH_THRESH_MS_DEFAULT},
        { LOW_THRESH_MS_DEFAULT, HIGH_THRESH_MS_DEFAULT},
        { 100, 200},
        { LOW_THRESH_MS_DEFAULT, HIGH_THRESH_MS_DEFAULT},
};

void ohm_para_update(void)
{
        int i;
        for (i = 0 ;i < OHM_SCHED_TOTAL ;i++ ) {
                if ( ohm_thresh_para[i].l_ms < LOW_THRESH_MS_LOW
                        || ohm_thresh_para[i].l_ms > LOW_THRESH_MS_HIGH
                        || ohm_thresh_para[i].h_ms < HIGH_THRESH_MS_LOW
                        || ohm_thresh_para[i].h_ms > HIGH_THRESH_MS_HIGH ) {
                        /********** Legal Check **********/
                        ohm_err("Para illegal: sched_type %s, l_ms %d, h_ms %d\n",
                                sched_list[i], ohm_thresh_para[i].l_ms, ohm_thresh_para[i].h_ms);
                        ohm_thresh_para[i].l_ms = LOW_THRESH_MS_DEFAULT;
                        ohm_thresh_para[i].h_ms = HIGH_THRESH_MS_DEFAULT;
                        return;
                }
                oppo_sched_para[i].low_thresh_ms = ohm_thresh_para[i].l_ms;
                oppo_sched_para[i].high_thresh_ms = ohm_thresh_para[i].h_ms;
        }
        ohm_debug("Success update ohm_para!\n");
}

/****  Init  ****/
void ohm_trig_init(void)
{
        int i;
        ohm_memmon_trig = (ohm_trig_list & OHM_CTRL_MEMMON) ? true : false;
        ohm_cpu_trig = (ohm_trig_list & OHM_CTRL_CPU_CUR) ? true : false;
        ohm_iopanic_mon_trig = (ohm_trig_list & OHM_CTRL_IOPANIC_MON) ? true : false;
		ohm_ionmon_trig = (ohm_trig_list & OHM_CTRL_IONMON) ? true : false;
        for (i = 0 ;i < OHM_SCHED_TOTAL ;i++ ) {
                oppo_sched_para[i].trig = (ohm_trig_list & BIT(i)) ? true : false;
					 if(i == OHM_SCHED_EMMCIO )
                 oppo_io_para.trig = (ohm_trig_list & BIT(i)) ? true : false;
        }
        return;
}

void ohm_logon_init(void)
{
        int i;
        ohm_cpu_logon = (ohm_logon_list & OHM_CTRL_CPU_CUR) ? true : false;
        ohm_memmon_logon = (ohm_logon_list & OHM_CTRL_MEMMON) ? true : false;
        ohm_iopanic_mon_logon = (ohm_logon_list & OHM_CTRL_IOPANIC_MON) ? true : false;
		ohm_ionmon_logon = (ohm_logon_list & OHM_CTRL_IONMON) ? true : false;
        for (i = 0 ;i < OHM_SCHED_TOTAL ;i++ ) {
                oppo_sched_para[i].logon = (ohm_logon_list & BIT(i)) ? true : false;
					 if(i == OHM_SCHED_EMMCIO )
                  oppo_io_para.logon = (ohm_logon_list & BIT(i)) ? true : false;
        }
        return;
}

void ohm_ctrl_init(void)
{
        int i;
        ohm_cpu_ctrl = (ohm_ctrl_list & OHM_CTRL_CPU_CUR) ? true : false;
        ohm_memmon_ctrl = (ohm_ctrl_list & OHM_CTRL_MEMMON) ? true : false;
        ohm_iopanic_mon_ctrl = (ohm_ctrl_list & OHM_CTRL_IOPANIC_MON) ? true : false;
		ohm_ionmon_ctrl = (ohm_ctrl_list & OHM_CTRL_IONMON) ? true : false;
        for (i = 0 ;i < OHM_SCHED_TOTAL ;i++ ) {
                oppo_sched_para[i].ctrl = (ohm_ctrl_list & BIT(i)) ? true : false;
					 if(i == OHM_SCHED_EMMCIO )
                  oppo_io_para.ctrl = (ohm_ctrl_list & BIT(i)) ? true : false;
        }
        return;
}

void ohm_para_init(void)
{
        int i;
        for (i = 0 ;i < OHM_SCHED_TOTAL ;i++ ) {
                memset(&oppo_sched_para[i], 0 , sizeof(struct sched_stat_para));
                oppo_sched_para[i].low_thresh_ms = 100;
                oppo_sched_para[i].high_thresh_ms = 500;
        }
        oppo_sched_para[OHM_SCHED_EMMCIO].low_thresh_ms = 100;
        oppo_sched_para[OHM_SCHED_EMMCIO].high_thresh_ms = 200;
        oppo_io_para.low_thresh_ms = 100;
        oppo_io_para.high_thresh_ms = 200;
        ohm_ctrl_init();
        ohm_logon_init();
        ohm_trig_init();
        ohm_debug("origin list: ctrl 0x%08x, logon 0x%08x, trig 0x%08x\n", ohm_ctrl_list, ohm_logon_list, ohm_trig_list);
        return;
}

/******  Cur cpuloading  ******/
static ssize_t cpu_load_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        int load = ohm_get_cur_cpuload(ohm_cpu_ctrl);

        if(load < 0)
                load = 0;
        len = sprintf( page, "cur_cpuloading: %d\n""cur_cpu_ctrl: %s\n""cur_cpu_logon: %s\n""cur_cpu_trig: %s\n",
                load, (ohm_cpu_ctrl ? "true" : "false"), (ohm_cpu_logon ? "true" : "false"), (ohm_cpu_trig ? "true" : "false"));

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_cpu_load_fops = {
	.read = cpu_load_read,
};

/******  Sched latency stat  *****/
static ssize_t sched_latency_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;
        int type = OHM_SCHED_SCHEDLATENCY;

        len = sprintf(page, "sched_low_thresh_ms: %d\n""sched_high_thresh_ms: %d\n"
                "sched_low_cnt: %lld\n""sched_high_cnt: %lld\n"
                "sched_total_ms: %lld\n""sched_total_cnt: %lld\n"
                "sched_fg_low_cnt: %lld\n""sched_fg_high_cnt: %lld\n"
                "sched_fg_total_ms: %lld\n""sched_fg_total_cnt: %lld\n"
                "sched_fg_max_ms: %lld\n""sched_delta_ms: %lld\n"
                "sched_latency_ctrl: %s\n""sched_latency_logon: %s\n""sched_latency_trig: %s\n",
                oppo_sched_para[type].low_thresh_ms,
                oppo_sched_para[type].high_thresh_ms,
                oppo_sched_para[type].low_cnt,
                oppo_sched_para[type].high_cnt,
                oppo_sched_para[type].total_ms,
                oppo_sched_para[type].total_cnt,
                oppo_sched_para[type].fg_low_cnt,
                oppo_sched_para[type].fg_high_cnt,
                oppo_sched_para[type].fg_total_ms,
                oppo_sched_para[type].fg_total_cnt,
                oppo_sched_para[type].fg_max_delta_ms,
                oppo_sched_para[type].delta_ms,
                oppo_sched_para[type].ctrl ? "true":"false",
                oppo_sched_para[type].logon ? "true":"false",
                oppo_sched_para[type].trig ? "true":"false");

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_sched_latency_fops = {
       .read = sched_latency_read,
};

/****** Sched iowait stat  *****/
static ssize_t iowait_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;
        int type = OHM_SCHED_IOWAIT;

        len = sprintf(page, "iowait_low_thresh_ms: %d\n""iowait_high_thresh_ms: %d\n"
                "iowait_low_cnt: %lld\n""iowait_high_cnt: %lld\n"
                "iowait_total_ms: %lld\n""iowait_total_cnt: %lld\n"
                "iowait_fg_low_cnt: %lld\n""iowait_fg_high_cnt: %lld\n"
                "iowait_fg_total_ms: %lld\n""iowait_fg_total_cnt: %lld\n"
                "iowait_fg_max_ms: %lld\n""iowait_delta_ms: %lld\n"
                "iowait_ctrl: %s\n""iowait_logon: %s\n""iowait_trig: %s\n",
                oppo_sched_para[type].low_thresh_ms,
                oppo_sched_para[type].high_thresh_ms,
                oppo_sched_para[type].low_cnt,
                oppo_sched_para[type].high_cnt,
                oppo_sched_para[type].total_ms,
                oppo_sched_para[type].total_cnt,
                oppo_sched_para[type].fg_low_cnt,
                oppo_sched_para[type].fg_high_cnt,
                oppo_sched_para[type].fg_total_ms,
                oppo_sched_para[type].fg_total_cnt,
                oppo_sched_para[type].fg_max_delta_ms,
                oppo_sched_para[type].delta_ms,
                oppo_sched_para[type].ctrl ? "true":"false",
                oppo_sched_para[type].logon ? "true":"false",
                oppo_sched_para[type].trig ? "true":"false");

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_iowait_fops = {
       .read = iowait_read,
};

/****** Sched sync wait stat  ******/
static ssize_t fsync_wait_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;
        int type = OHM_SCHED_FSYNC;

        len = sprintf(page, "fsync_low_thresh_ms: %d\n""fsync_high_thresh_ms: %d\n"
                "fsync_low_cnt: %lld\n""fsync_high_cnt: %lld\n"
                "fsync_total_ms: %lld\n""fsync_total_cnt: %lld\n"
                "fsync_fg_low_cnt: %lld\n""fsync_fg_high_cnt: %lld\n"
                "fsync_fg_total_ms: %lld\n""fsync_fg_total_cnt: %lld\n"
                "fsync_fg_max_ms: %lld\n""fsync_delta_ms: %lld\n"
                "fsync_ctrl: %s\n""fsync_logon: %s\n""fsync_trig: %s\n",
                oppo_sched_para[type].low_thresh_ms,
                oppo_sched_para[type].high_thresh_ms,
                oppo_sched_para[type].low_cnt,
                oppo_sched_para[type].high_cnt,
                oppo_sched_para[type].total_ms,
                oppo_sched_para[type].total_cnt,
                oppo_sched_para[type].fg_low_cnt,
                oppo_sched_para[type].fg_high_cnt,
                oppo_sched_para[type].fg_total_ms,
                oppo_sched_para[type].fg_total_cnt,
                oppo_sched_para[type].fg_max_delta_ms,
                oppo_sched_para[type].delta_ms,
                oppo_sched_para[type].ctrl ? "true":"false",
                oppo_sched_para[type].logon ? "true":"false",
                oppo_sched_para[type].trig ? "true":"false");

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_fsync_wait_fops = {
       .read = fsync_wait_read,
};

/****** emcdrv_iowait stat  ******/
/* Emmc - 1 ; Ufs - 2 */
int ohm_flash_type = OHM_FLASH_TYPE_UFS;
static ssize_t emmcio_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
      int len = 0;
      char *page = kzalloc(2048,GFP_KERNEL);
      if (!page)
        return -ENOMEM;
      //int type = OHM_SCHED_EMMCIO;
      len = sprintf(page, "emcdrv_iowait_low_thresh_ms: %d\n" //low thresh parameter
                            "emcdrv_iowait_low_cnt: %lld\n"
                            //high thresh parameter
                            "emcdrv_iowait_high_thresh_ms: %d\n"
                            "emcdrv_iowait_high_cnt: %lld\n"
                            //total parameter
                            "emcdrv_iowait_total_ms: %lld\n"
                            "flashio_total_latency: %lld\n"
                            "blockio_total_latency: %lld\n"
                            "emcdrv_iowait_total_cnt: %lld\n"
                            //fg latency parameter
                            "emcdrv_iowait_fg_low_cnt: %lld\n"
                            "emcdrv_iowait_fg_high_cnt: %lld\n"
                            "emcdrv_iowait_fg_total_ms: %lld\n"
                            "emcdrv_iowait_fg_total_cnt: %lld\n"
                            "emcdrv_iowait_fg_max_ms: %lld\n"
                            "emcdrv_iowait_delta_ms: %lld\n"
                            // fg
                            "iosize_write_count_fg: %lld\n"
                            "iosize_write_us_fg: %lld\n"
                            "iosize_500ms_syncwrite_count_fg: %lld\n"
                            "iosize_200ms_syncwrite_count_fg: %lld\n"
                            "iosize_500ms_asyncwrite_count_fg: %lld\n"
                            "iosize_200ms_asyncwrite_count_fg: %lld\n"
                            "iosize_read_count_fg: %lld\n"
                            "iosize_read_us_fg: %lld\n"
                            "iosize_500ms_read_count_fg: %lld\n"
                            "iosize_200ms_read_count_fg: %lld\n"
                            //bg
                            "iosize_write_count_bg: %lld\n"
                            "iosize_write_us_bg: %lld\n"
                            "iosize_2s_asyncwrite_count_bg: %lld\n"
                            "iosize_500ms_asyncwrite_count_bg: %lld\n"
                            "iosize_200ms_asyncwrite_count_bg: %lld\n"
                            "iosize_2s_syncwrite_count_bg: %lld\n"
                            "iosize_500ms_syncwrite_count_bg: %lld\n"
                            "iosize_200ms_syncwrite_count_bg: %lld\n"
                            "iosize_read_count_bg: %lld\n"
                            "iosize_read_us_bg: %lld\n"
                            "iosize_2s_read_count_bg: %lld\n"
                            "iosize_500ms_read_count_bg: %lld\n"
                            "iosize_200ms_read_count_bg: %lld\n"
                            //4k
                            "iosize_4k_read_count: %lld\n"
                            "iosize_4k_read_ms: %lld\n"
                            "iosize_4k_write_count: %lld\n"
                            "iosize_4k_write_ms: %lld\n"
                            // option
                            "emcdrv_iowait_ctrl: %s\n"
                            "emcdrv_iowait_logon: %s\n"
                            "emcdrv_iowait_trig: %s\n",
                            oppo_io_para.low_thresh_ms, //low thresh parameter
                            oppo_io_para.low_cnt,
                            //high thresh parameter
                            oppo_io_para.high_thresh_ms,
                            oppo_io_para.high_cnt,
                            //total parameter
                            (oppo_io_para.total_us/1000),
                            (oppo_io_para.emmc_total_us/1000),
                            (oppo_io_para.total_us - oppo_io_para.emmc_total_us)/1000,
                            oppo_io_para.total_cnt,
                            //fg latency parameter
                            oppo_io_para.fg_low_cnt,
                            oppo_io_para.fg_high_cnt,
                            oppo_io_para.fg_total_ms,
                            oppo_io_para.fg_total_cnt,
                            oppo_io_para.fg_max_delta_ms,
                            oppo_io_para.delta_ms,
                            //fg
                            oppo_io_para.iosize_write_count_fg,
                            oppo_io_para.iosize_write_us_fg,
                            oppo_io_para.iosize_500ms_syncwrite_count_fg,
                            oppo_io_para.iosize_200ms_syncwrite_count_fg,
                            oppo_io_para.iosize_500ms_asyncwrite_count_fg,
                            oppo_io_para.iosize_200ms_asyncwrite_count_fg,
                            oppo_io_para.iosize_read_count_fg,
                            oppo_io_para.iosize_read_us_fg,
                            oppo_io_para.iosize_500ms_read_count_fg,
                            oppo_io_para.iosize_200ms_read_count_fg,
                            //bg
                            oppo_io_para.iosize_write_count_bg,
                            oppo_io_para.iosize_write_us_bg,
                            oppo_io_para.iosize_2s_asyncwrite_count_bg,
                            oppo_io_para.iosize_500ms_asyncwrite_count_bg,
                            oppo_io_para.iosize_200ms_asyncwrite_count_bg,
                            oppo_io_para.iosize_2s_syncwrite_count_bg,
                            oppo_io_para.iosize_500ms_syncwrite_count_bg,
                            oppo_io_para.iosize_200ms_syncwrite_count_bg,
                            oppo_io_para.iosize_read_count_bg,
                            oppo_io_para.iosize_read_us_bg,
                            oppo_io_para.iosize_2s_read_count_bg,
                            oppo_io_para.iosize_500ms_read_count_bg,
                            oppo_io_para.iosize_200ms_read_count_bg,
                            //4k
                            oppo_io_para.iosize_4k_read_count,
                            (oppo_io_para.iosize_4k_read_us/1000),
                            oppo_io_para.iosize_4k_write_count,
                            (oppo_io_para.iosize_4k_write_us/1000),
                            // option
                            oppo_io_para.ctrl ? "true":"false",
                            oppo_io_para.logon ? "true":"false",
                            oppo_io_para.trig ? "true":"false");

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
              kfree(page);
              return -EFAULT;
        }
        kfree(page);
        *off += len < count ? len : count;
        return (len < count ? len : count);
}


static const struct file_operations proc_emmcio_fops = {
       .read = emmcio_read,
};

/****** dstat statistics  ******/
static ssize_t dstate_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;
        int type = OHM_SCHED_DSTATE;

        len = sprintf(page, "dstate_low_thresh_ms: %d\n""dstate_high_thresh_ms: %d\n"
                "dstate_low_cnt: %lld\n""dstate_high_cnt: %lld\n"
                "dstate_total_ms: %lld\n""dstate_total_cnt: %lld\n"
                "dstate_fg_low_cnt: %lld\n""dstate_fg_high_cnt: %lld\n"
                "dstate_fg_total_ms: %lld\n""dstate_fg_total_cnt: %lld\n"
                "dstate_fg_max_ms: %lld\n""dstate_delta_ms: %lld\n"
                "dstate_ctrl: %s\n""dstate_logon: %s\n""dstate_trig: %s\n",
                oppo_sched_para[type].low_thresh_ms,
                oppo_sched_para[type].high_thresh_ms,
                oppo_sched_para[type].low_cnt,
                oppo_sched_para[type].high_cnt,
                oppo_sched_para[type].total_ms,
                oppo_sched_para[type].total_cnt,
                oppo_sched_para[type].fg_low_cnt,
                oppo_sched_para[type].fg_high_cnt,
                oppo_sched_para[type].fg_total_ms,
                oppo_sched_para[type].fg_total_cnt,
                oppo_sched_para[type].fg_max_delta_ms,
                oppo_sched_para[type].delta_ms,
                oppo_sched_para[type].ctrl ? "true":"false",
                oppo_sched_para[type].logon ? "true":"false",
                oppo_sched_para[type].trig ? "true":"false");


        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_dstate_fops = {
       .read = dstate_read,

};

/******  mem monitor read  ******/
#ifdef CONFIG_OPPO_MEM_MONITOR
static ssize_t alloc_wait_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;

        len = sprintf(page, "total_alloc_wait_h_cnt: %lld\n""total_alloc_wait_l_cnt: %lld\n"
                "fg_alloc_wait_h_cnt: %lld\n""fg_alloc_wait_l_cnt: %lld\n"
                "total_alloc_wait_max_ms: %lld\n""total_alloc_wait_max_order: %lld\n"
                "fg_alloc_wait_max_ms: %lld\n""fg_alloc_wait_max_order: %lld\n"
                "alloc_wait_ctrl: %s\n""alloc_wait_logon: %s\n""alloc_wait_trig: %s\n",
                allocwait_para.total_alloc_wait_h_cnt,allocwait_para.total_alloc_wait_l_cnt,
                allocwait_para.fg_alloc_wait_h_cnt,allocwait_para.fg_alloc_wait_l_cnt,
                allocwait_para.total_alloc_wait_max_ms,allocwait_para.total_alloc_wait_max_order,
                allocwait_para.fg_alloc_wait_max_ms,allocwait_para.fg_alloc_wait_max_order,
                ohm_memmon_ctrl ? "true":"false", ohm_memmon_logon ? "true":"false", ohm_memmon_trig ? "true":"false");

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_alloc_wait_fops = {
       .read = alloc_wait_read,
};
#endif /*CONFIG_OPPO_MEM_MONITOR*/

static ssize_t ion_wait_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;

        len = sprintf(page, "total_ion_wait_h_cnt: %lld\n""total_ion_wait_l_cnt: %lld\n"
                "fg_ion_wait_h_cnt: %lld\n""fg_ion_wait_l_cnt: %lld\n"
                "total_ion_wait_max_ms: %lld\n"
                "ion_wait_ctrl: %s\n""ion_wait_logon: %s\n""ion_wait_trig: %s\n",
                ionwait_para.total_ion_wait_h_cnt,ionwait_para.total_ion_wait_l_cnt,
                ionwait_para.fg_ion_wait_h_cnt,ionwait_para.fg_ion_wait_l_cnt,
                ionwait_para.total_ion_wait_max_ms,
                ohm_ionmon_ctrl ? "true":"false", ohm_ionmon_logon ? "true":"false", ohm_ionmon_trig ? "true":"false");

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_ion_wait_fops = {
       .read = ion_wait_read,
};

/******  Proc para   ******/
static ssize_t ohm_para_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf( page, "action: %s\n""ctrl: 0x%08x\n""logon: 0x%08x\n""trig: 0x%08x\n",
                (ohm_action_ctrl ? "true":"false"), ohm_ctrl_list, ohm_logon_list, ohm_trig_list);

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static ssize_t ohm_para_write(struct file *file, const char __user *buff, size_t len, loff_t *ppos)
{
        char write_data[32] = {0};
        char ctrl_list[32] = {0};
	int action_ctrl;

        if (copy_from_user(&write_data, buff, len)) {
                ohm_err("write error.\n");
                return -EFAULT;
        }
        write_data[len] = '\0';
        if (write_data[len - 1] == '\n') {
                write_data[len - 1] = '\0';
        }

        if (0 == strncmp(write_data, "ohmctrl", 7)) {
                strncpy(ctrl_list, &write_data[7], OHM_INT_MAX);
                ctrl_list[OHM_INT_MAX] = '\0';
                ohm_ctrl_list = (int)simple_strtol(ctrl_list, NULL, 10);
                ohm_ctrl_init();
        } else if (0 == strncmp(write_data, "ohmlogon", 8)) {
                strncpy(ctrl_list, &write_data[8], OHM_INT_MAX);
                ctrl_list[OHM_INT_MAX] = '\0';
                ohm_logon_list = (int)simple_strtol(ctrl_list, NULL, 10);
                ohm_logon_init();
        } else if (0 == strncmp(write_data, "ohmtrig", 7)) {
                strncpy(ctrl_list, &write_data[7], OHM_INT_MAX);
                ctrl_list[OHM_INT_MAX] = '\0';
                ohm_trig_list = (int)simple_strtol(ctrl_list, NULL, 10);
                ohm_trig_init();
        } else if (0 == strncmp(write_data, "ohmparaupdate", 13)) {
                ohm_para_update();
                return len;
        } else if (0 == strncmp(write_data, "ohmacitonctrl", 13)){
		strncpy(ctrl_list, &write_data[13], OHM_INT_MAX);
		ctrl_list[OHM_INT_MAX] = '\0';
		action_ctrl =  (int)simple_strtol(ctrl_list, NULL, 10);
		ohm_action_ctrl = action_ctrl != 0 ? 1 : 0;
	} else {
                ohm_err("input illegal\n");
                return -EFAULT;
        }
        ohm_debug("write: %s, set: %s, ctrl: 0x%08x, logon: 0x%08x, trig: 0x%08x\n",
                write_data, ctrl_list, ohm_ctrl_list, ohm_logon_list, ohm_trig_list);
	return len;
}

static const struct file_operations proc_para_fops = {
        .read = ohm_para_read,
        .write = ohm_para_write,
};

/******  iowait hung show  ******/
unsigned int  iowait_hung_cnt = 0;
unsigned int  iowait_panic_cnt = 0;
static ssize_t iowait_hung_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
        char page[1024] = {0};
        int len = 0;

        len = sprintf( page, "iowait_hung_cnt: %u\n""iowait_panic_cnt: %u\n"
                "ohm_iopanic_mon_ctrl: %s\n""ohm_iopanic_mon_logon: %s\n""ohm_iopanic_mon_trig: %s\n",
                iowait_hung_cnt, iowait_panic_cnt,
                (ohm_iopanic_mon_ctrl ? "true" : "false"), (ohm_iopanic_mon_logon ? "true" : "false"), (ohm_iopanic_mon_trig ? "true" : "false"));

        if (len > *off) {
                len -= *off;
        } else {
                len = 0;
        }
        if (copy_to_user(buff, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

static const struct file_operations proc_iowait_hung_fops = {
       .read = iowait_hung_read,
};

/******  stuck info read  ******/
// Liujie.Xie@TECH.Kernel.Sched, 2019/08/29, add for stuck monitor
/* warning: different project with different setting of LTT_NUM (small core number) */
#define LTT_NUM (4)
void update_stuck_trace_info(struct task_struct *tsk, int trace_type, unsigned int cpu, u64 delta) {
    if (!tsk->stuck_trace) {
        return;
    }

    if (trace_type == UIFIRST_TRACE_RUNNABLE) { // runnable
        tsk->oppo_stuck_info.runnable_state           += delta;
    } else if (trace_type == UIFIRST_TRACE_DSTATE) { // D state
        tsk->oppo_stuck_info.d_state.cnt++;
        if (tsk->in_iowait) {
            tsk->oppo_stuck_info.d_state.iowait_ns    += delta;
        } else if (tsk->in_mutex) {
            tsk->oppo_stuck_info.d_state.mutex_ns     += delta;
        } else if (tsk->in_downread) {
            tsk->oppo_stuck_info.d_state.downread_ns  += delta;
        } else if (tsk->in_downwrite) {
            tsk->oppo_stuck_info.d_state.downwrite_ns += delta;
        } else {
            tsk->oppo_stuck_info.d_state.other_ns     += delta;
        }
    } else if (trace_type == UIFIRST_TRACE_SSTATE) { // S state
        tsk->oppo_stuck_info.s_state.cnt++;
        if (tsk->in_binder) {
            tsk->oppo_stuck_info.s_state.binder_ns    += delta;
        } else if (tsk->in_futex) {
            tsk->oppo_stuck_info.s_state.futex_ns     += delta;
        } else if (tsk->in_epoll) {
            tsk->oppo_stuck_info.s_state.epoll_ns     += delta;
        } else {
            tsk->oppo_stuck_info.s_state.other_ns     += delta;
        }
    } else if (trace_type == UIFIRST_TRACE_RUNNING) { // running
        if (cpu < LTT_NUM) {
            tsk->oppo_stuck_info.ltt_running_state += delta;
        } else {
            tsk->oppo_stuck_info.big_running_state += delta;
        }
    }
}

/******  cpu info show  ******/
extern unsigned int cpufreq_quick_get_max(unsigned int cpu);
static ssize_t cpu_info_read(struct file *filp, char __user *buff, size_t count, loff_t *off)
{
    char page[1024] = {0};
    int len = 0;
    unsigned int cpu;
    unsigned long scale_capacity = 0, last_capacity = 0;

    for_each_possible_cpu(cpu) {
        scale_capacity = arch_scale_cpu_capacity(NULL, cpu);
        if (scale_capacity == last_capacity) {
            continue;
        }
        last_capacity = scale_capacity;
        len += snprintf(page + len, sizeof(page) - len, "%u ", cpufreq_quick_get_max(cpu));
    }

    if (len > 0) {
        len += snprintf(page + len, sizeof(page) - len, "\n");
    }

    if (len > *off) {
            len -= *off;
    } else {
            len = 0;
    }
    if (copy_to_user(buff, page, (len < count ? len : count))) {
            return -EFAULT;
    }
    *off += len < count ? len : count;
    return (len < count ? len : count);
}

static const struct file_operations proc_cpu_info_fops = {
       .read = cpu_info_read,
};

/******  ufs feature read  ******/
#ifdef CONFIG_UFSFEATURE
static ssize_t ufsf_para_read(struct file *filp, char __user *buff, size_t count,
			 loff_t *off)
{
	char page[1024] = {0};
	int len = 0;

#ifdef CONFIG_UFSHPB
	len += sprintf(&page[len], "hit: %lld\nhit_4k: %lld\nhit_8_32k: %lld\n"
				"miss: %lld\nspan: %lld\nspan_hit: %lld\n"
				"noti: %lld\nnoti_act: %lld\nnoti_inact: %lld\n"
				"rgn_act: %lld\nmap_req: %lld\npre_req: %lld\n"
				"hpb_rgns: %d\n",
				ufsf_para.hit, ufsf_para.hit_4k, ufsf_para.hit_8_32k,
				ufsf_para.miss, ufsf_para.span, ufsf_para.span_hit,
				ufsf_para.noti, ufsf_para.noti_act, ufsf_para.noti_inact,
				ufsf_para.rgn_act, ufsf_para.map_req, ufsf_para.pre_req,
				ufsf_para.hpb_rgns);
#endif

#ifdef CONFIG_UFSTW
	len += sprintf(&page[len],
				"tw_count: %llu\n"
				"tw_error_count: %llu\n"
				"tw_total_MB: %llu\n"
				"tw_enable: %d\n"
				"tw_lifetime: %lx\n"
				"h8_enter_count: %llu\n"
				"h8_all_ms: %llu\n"
				"h8_enter_100ms_count: %llu\n"
				"h8_above100_ms: %llu\n"
				"h8_max_ms: %llu\n"
				"buffer_size: %u\n",
				(ufsf_para.tw_enable_count + ufsf_para.tw_disable_count),
				ufsf_para.tw_setflag_error_count,    /* total error count */
				ufsf_para.tw_amount_W_kb >> 10,    /* TW write total : MB */
				ufsf_para.tw_enable,
				ufsf_para.tw_lifetime,
				ufsf_para.hibern8_enter_count, 
				ufsf_para.hibern8_amount_ms,
				ufsf_para.hibern8_enter_count_100ms,
				ufsf_para.hibern8_amount_ms_100ms,
				ufsf_para.hibern8_max_ms,
				ufsf_para.buffer_size);
#endif

	if (len > *off) {
		len -= *off;
	} else {
		len = 0;
	}
	if (copy_to_user(buff, page, (len < count ? len : count))) {
		return -EFAULT;
	}
	*off += len < count ? len : count;
	return (len < count ? len : count);
}

static const struct file_operations proc_ufsf_para_fops = {
	.read = ufsf_para_read,
};
#endif
/******  End  ******/

#define HEALTHINFO_PROC_NODE "oppo_healthinfo"
static struct proc_dir_entry *oppo_healthinfo = NULL;

static int __init oppo_healthinfo_init(void)
{
	int ret = 0;
	struct proc_dir_entry *pentry;

        ohm_para_init();
        ohm_action_init();
	oppo_healthinfo =  proc_mkdir(HEALTHINFO_PROC_NODE, NULL);
	if(!oppo_healthinfo) {
		ohm_err("can't create oppo_healthinfo proc\n");
		goto ERROR_INIT_VERSION;
	}
/******  ctrl  *****/
	pentry = proc_create("para_update", S_IRUGO | S_IWUGO, oppo_healthinfo, &proc_para_fops);
        if(!pentry) {
                ohm_err("create para_update proc failed.\n");
                goto ERROR_INIT_VERSION;
        }

/******  Stat  ******/
	pentry = proc_create("fsync_wait", S_IRUGO, oppo_healthinfo, &proc_fsync_wait_fops);
	if(!pentry) {
		ohm_err("create fsync_wait proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

	pentry = proc_create("cpu_loading", S_IRUGO, oppo_healthinfo, &proc_cpu_load_fops);
	if(!pentry) {
		ohm_err("create cpu_loading proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

	pentry = proc_create("iowait", S_IRUGO, oppo_healthinfo, &proc_iowait_fops);
	if(!pentry) {
		ohm_err("create iowait proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

	pentry = proc_create("sched_latency", S_IRUGO, oppo_healthinfo, &proc_sched_latency_fops);
	if(!pentry) {
		ohm_err("create sched_latency proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

	pentry = proc_create("emcdrv_iowait", S_IRUGO, oppo_healthinfo, &proc_emmcio_fops);
	if(!pentry) {
		ohm_err("create emmc_driver_io_wait proc failed.\n");
		goto ERROR_INIT_VERSION;
	}
	
	pentry = proc_create("dstate", S_IRUGO, oppo_healthinfo, &proc_dstate_fops);
	if(!pentry) {
		ohm_err("create dstate proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

	pentry = proc_create("iowait_hung", S_IRUGO, oppo_healthinfo, &proc_iowait_hung_fops);
	if(!pentry) {
		ohm_err("create iowait_hung proc failed.\n");
		goto ERROR_INIT_VERSION;
	}
	
	pentry = proc_create("ion_wait", S_IRUGO, oppo_healthinfo, &proc_ion_wait_fops);
	if(!pentry) {
		ohm_err("create ion_wait proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

#ifdef CONFIG_OPPO_MEM_MONITOR
	pentry = proc_create("alloc_wait", S_IRUGO, oppo_healthinfo, &proc_alloc_wait_fops);
	if(!pentry) {
		ohm_err("create alloc_wait proc failed.\n");
		goto ERROR_INIT_VERSION;
	}

#endif /*CONFIG_OPPO_MEM_MONITOR*/

    pentry = proc_create("cpu_info", S_IRUGO, oppo_healthinfo, &proc_cpu_info_fops);
    if(!pentry) {
        ohm_err("create cpu info proc failed.\n");
        goto ERROR_INIT_VERSION;
    }

#ifdef CONFIG_UFSFEATURE
	pentry = proc_create("ufs_feature", S_IRUGO, oppo_healthinfo, &proc_ufsf_para_fops);
	if(!pentry) {
		ohm_err("create ufs_feature proc failed.\n");
		goto ERROR_INIT_VERSION;
	}
#endif

        ohm_debug("Success \n");
	return ret;

ERROR_INIT_VERSION:
	remove_proc_entry(HEALTHINFO_PROC_NODE, NULL);
	return -ENOENT;
}

module_init(oppo_healthinfo_init);

module_param_named(ohm_action_ctrl, ohm_action_ctrl, bool, S_IRUGO | S_IWUSR);
module_param_named(ohm_iowait_l_ms, ohm_thresh_para[OHM_SCHED_IOWAIT].l_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_iowait_h_ms, ohm_thresh_para[OHM_SCHED_IOWAIT].h_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_schedlatency_l_ms, ohm_thresh_para[OHM_SCHED_SCHEDLATENCY].l_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_schedlatency_h_ms, ohm_thresh_para[OHM_SCHED_SCHEDLATENCY].h_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_fsync_l_ms, ohm_thresh_para[OHM_SCHED_FSYNC].l_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_fsync_h_ms, ohm_thresh_para[OHM_SCHED_FSYNC].h_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_emmcio_l_ms, ohm_thresh_para[OHM_SCHED_EMMCIO].l_ms, int, S_IRUGO | S_IWUSR);
module_param_named(ohm_emmcio_h_ms, ohm_thresh_para[OHM_SCHED_EMMCIO].h_ms, int, S_IRUGO | S_IWUSR);

MODULE_DESCRIPTION("OPPO healthinfo monitor");
MODULE_LICENSE("GPL v2");
