/************************************************************************************
** File:  oppo_p922x.c
** VENDOR_EDIT
** Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
** 
** Description: 
**      for wireless charge
** 
** Version: 1.0
** Date created: 21:03:46,06/11/2018
** Author: Lin Shangbo, Li Jiada
** 
** --------------------------- Revision History: ------------------------------------------------------------
* <version>       <date>        <author>              			<desc>
* Revision 1.0    2018-11-06    Lin Shangbo,Li Jiada   		Created for wireless charge
************************************************************************************************************/

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>

#include <linux/debugfs.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/power_supply.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/bitops.h>
#include <linux/mutex.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>
#include <linux/rtc.h>
#include <linux/random.h>

#include <linux/alarmtimer.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/regmap.h>
#include <linux/iio/consumer.h>
#include <uapi/linux/qg.h>

#include <soc/oppo/device_info.h>
#include <soc/oppo/oppo_project.h>

#include "../oppo_vooc.h"
#include "../oppo_gauge.h"
#include "../oppo_charger.h"
#include "../charger_ic/oppo_mp2650.h"
#include "oppo_chargepump.h"
#include <oppo_p922x.h>
#include <oppo_p922x_fw.h>
#ifdef SUPPORT_SP6001_FW_UPDATE
#include <oppo_sp6001_fw.h>
#endif
#include <soc/oppo/boot_mode.h>

#define DEBUG_BY_FILE_OPS
#define DEBUG_FASTCHG_BY_ADB

//#define FASTCHG_TEST_BY_TIME

#ifdef FASTCHG_TEST_BY_TIME
bool is_fastchging = false;
unsigned long pre_seconds = 0;
bool enable_test = false;
#endif

int send_msg_timer = 0;
#ifdef SUPPORT_SP6001_FW_UPDATE
typedef enum {
BOOT_AREA,
RX_AREA,
TX_AREA,
}E_SP6001_AREA;

#define ERROR_BUFFER_NUM	100
#define I2C_ERROR_HANDLE(x)	if (x != 0) { \
								chg_err("<SP UPDATE>xxxxxxxxxxxxx EXIT UPDATE xxxxxxxxxxxxxxxx\n"); \
								return -3; \
							}
							
static int success_count = 0;
static int error_count = 0;
static int error_buf[ERROR_BUFFER_NUM];
#endif

struct oppo_p922x_ic *p922x_chip = NULL;

extern struct oppo_chg_chip *g_oppo_chip;
extern void oppo_wireless_set_otg_en_val(int value);
extern int oppo_wireless_get_otg_en_val(struct oppo_p922x_ic *chip);
extern void oppo_set_wrx_en_value(int value);
extern void oppo_set_wrx_otg_value(int value);
extern int oppo_get_idt_en_val(void);
extern int oppo_get_wrx_otg_val(void);
extern int oppo_get_wrx_en_val(void);
extern void oppo_chg_cancel_update_work_sync(void);
extern void oppo_chg_restart_update_work(void);
extern bool oppo_get_wired_otg_online(void);
extern bool oppo_get_wired_chg_present(void);
int p922x_get_idt_con_val(void);
int p922x_get_idt_int_val(void);
int p922x_get_vbat_en_val(void);

static DEFINE_MUTEX(p922x_i2c_access);

//This array must be consisten with E_FASTCHARGE_LEVEL
int fasctchg_current[FASTCHARGE_LEVEL_NUM] = 
{
0,			//FASTCHARGE_LEVEL_UNKNOW
2000,		//FASTCHARGE_LEVEL_1
1250,		//FASTCHARGE_LEVEL_2
1000,		//FASTCHARGE_LEVEL_3
900,		//FASTCHARGE_LEVEL_4
800,		//FASTCHARGE_LEVEL_5
700,		//FASTCHARGE_LEVEL_6
600,		//FASTCHARGE_LEVEL_7
500,		//FASTCHARGE_LEVEL_8
400,		//FASTCHARGE_LEVEL_9
};

#ifdef FASTCHG_TEST_BY_TIME
int get_rtc_time(unsigned long *rtc_time)
{
	struct rtc_time tm;
	struct rtc_device *rtc;
	int rc;

	rtc = rtc_class_open(CONFIG_RTC_HCTOSYS_DEVICE);
	if (rtc == NULL) {
		pr_err("Failed to open rtc device (%s)\n",
				CONFIG_RTC_HCTOSYS_DEVICE);
		return -EINVAL;
	}

	rc = rtc_read_time(rtc, &tm);
	if (rc) {
		pr_err("Failed to read rtc time (%s) : %d\n",
				CONFIG_RTC_HCTOSYS_DEVICE, rc);
		goto close_time;
	}

	rc = rtc_valid_tm(&tm);
	if (rc) {
		pr_err("Invalid RTC time (%s): %d\n",
				CONFIG_RTC_HCTOSYS_DEVICE, rc);
		goto close_time;
	}
	rtc_tm_to_time(&tm, rtc_time);

close_time:
	rtc_class_close(rtc);
	return rc;
}
#endif

#define P22X_ADD_COUNT      2
static int __p922x_read_reg(struct oppo_p922x_ic *chip, int reg, char *returnData, int count)
{
	/* We have 16-bit i2c addresses - care for endianness */
	char cmd_buf[2]={ reg >> 8, reg & 0xff };
	int ret = 0;
	int i;
	char val_buf[20];
	
	for (i = 0; i < count; i++) {
		val_buf[i] = 0;
	}

	ret = i2c_master_send(chip->client, cmd_buf, P22X_ADD_COUNT);
	if (ret < P22X_ADD_COUNT) {
		chg_err("%s: i2c read error, reg: %x\n", __func__, reg);
		return ret < 0 ? ret : -EIO;
	}

	ret = i2c_master_recv(chip->client, val_buf, count);
	if (ret < count) {
		chg_err("%s: i2c read error, reg: %x\n", __func__, reg);
		return ret < 0 ? ret : -EIO;
	}

	for (i = 0; i < count; i++) {
		*(returnData + i) = val_buf[i];
	}
	
	return 0;
}

static int __p922x_write_reg(struct oppo_p922x_ic *chip, int reg, int val)
{
	int ret;
	unsigned char data[3] = { reg >> 8, reg & 0xff, val };

	//chg_err("<~WPC~> tongfeng test reg[0x%x]!\n", reg);

	ret = i2c_master_send(chip->client, data, 3);
	if (ret < 3) {
		chg_err("%s: i2c write error, reg: %x\n", __func__, reg);
		return ret < 0 ? ret : -EIO;
	}

	return 0;
}

#ifndef SUPPORT_SP6001_FW_UPDATE
static int p922x_write_reg_multi_byte(struct oppo_p922x_ic *chip, int reg, char *cbuf, int length)
{
	int ret;
	int send_length;
	unsigned char *data_w;

	send_length = length + 2;
	data_w = kzalloc(send_length, GFP_KERNEL);
	if (!data_w) {
		chg_err("can't alloc memory!\n");
		return -1;
	}

	data_w[0] = reg >> 8;
	data_w[1] = reg & 0xff;

	memcpy(data_w + 2, cbuf, length);

	mutex_lock(&p922x_i2c_access);

	ret = i2c_master_send(chip->client, data_w, send_length);
	if (ret < send_length) {
		chg_err("%s: i2c write error, reg: %x\n", __func__, reg);
		kfree(data_w);
		mutex_unlock(&p922x_i2c_access);
		return ret < 0 ? ret : -EIO;
	}

	mutex_unlock(&p922x_i2c_access);

	kfree(data_w);
	return 0;
}
#endif

static int p922x_read_reg(struct oppo_p922x_ic *chip, int reg, char *returnData, int count)
{
	int ret = 0;

	mutex_lock(&p922x_i2c_access);
	ret = __p922x_read_reg(chip, reg, returnData, count);
	mutex_unlock(&p922x_i2c_access);
	return ret;
}

#ifdef WPC_USE_SP6001
static int p922x_write_reg(struct oppo_p922x_ic *chip, int reg, int val)
{
	int ret = 0;

	mutex_lock(&p922x_i2c_access);
	ret = __p922x_write_reg(chip, reg, val);
	mutex_unlock(&p922x_i2c_access);
	return ret;
}
#endif

static int p922x_config_interface (struct oppo_p922x_ic *chip, int RegNum, int val, int MASK)
{
	char p922x_reg = 0;
	int ret = 0;

	mutex_lock(&p922x_i2c_access);
	ret = __p922x_read_reg(chip, RegNum, &p922x_reg, 1);

	p922x_reg &= ~MASK;
	p922x_reg |= val;

	ret = __p922x_write_reg(chip, RegNum, p922x_reg);

	mutex_unlock(&p922x_i2c_access);
	
	return ret;
}

#ifdef WPC_USE_SP6001
static void sp6001_send_msg_ask_adapter_type(struct oppo_p922x_ic *chip)
{
	chg_err("<~WPC~>sp6001_send_msg_ask_adapter_type----------->\n");

	p922x_write_reg(chip, WRITE_REG_00, 0x90);
	p922x_write_reg(chip, WRITE_REG_01, 0x38);
	p922x_write_reg(chip, WRITE_REG_02, 0x3B);
	p922x_write_reg(chip, WRITE_REG_03, 0x04);
	p922x_write_reg(chip, WRITE_REG_04, 0x44);

	p922x_write_reg(chip, WRITE_REG_TX, 0x55); 

	msleep(500);

	p922x_write_reg(chip, WRITE_REG_00, 0x90);
	p922x_write_reg(chip, WRITE_REG_01, 0x48);
	p922x_write_reg(chip, WRITE_REG_02, 0xA1);
	p922x_write_reg(chip, WRITE_REG_03, 0x5E);
	p922x_write_reg(chip, WRITE_REG_04, 0xFF);
	p922x_write_reg(chip, WRITE_REG_05, 0x00);  

	p922x_write_reg(chip, WRITE_REG_TX, 0x55); 
}

static void sp6001_send_msg_enter_fastchg_mode(struct oppo_p922x_ic *chip)
{
	chg_err("<~WPC~>sp6001_send_msg_enter_fastchg_mode----------->\n");

	p922x_write_reg(chip, WRITE_REG_00, 0x90);
	p922x_write_reg(chip, WRITE_REG_01, 0x48);
	p922x_write_reg(chip, WRITE_REG_02, 0xA2);
	p922x_write_reg(chip, WRITE_REG_03, 0x5D);
	p922x_write_reg(chip, WRITE_REG_04, 0xFF);
	p922x_write_reg(chip, WRITE_REG_05, 0x00); 

	p922x_write_reg(chip, WRITE_REG_TX, 0x55); 
}

static void sp6001_send_msg_enter_usb_mode(struct oppo_p922x_ic *chip)
{  
	chg_err("<~WPC~>sp6001_send_msg_enter_usb_mode----------->\n");

	p922x_write_reg(chip, WRITE_REG_00, 0x90);
	p922x_write_reg(chip, WRITE_REG_01, 0x48);
	p922x_write_reg(chip, WRITE_REG_02, 0xA3);
	p922x_write_reg(chip, WRITE_REG_03, 0x5C);
	p922x_write_reg(chip, WRITE_REG_04, 0xFF);
	p922x_write_reg(chip, WRITE_REG_05, 0x00);

	p922x_write_reg(chip, WRITE_REG_TX, 0x55); 
}

static void sp6001_send_msg_enter_normal_mode(struct oppo_p922x_ic *chip)
{
	chg_err("<~WPC~>sp6001_send_msg_enter_normal_mode----------->\n");

	p922x_write_reg(chip, WRITE_REG_00, 0x90);
	p922x_write_reg(chip, WRITE_REG_01, 0x48);
	p922x_write_reg(chip, WRITE_REG_02, 0xA4);
	p922x_write_reg(chip, WRITE_REG_03, 0x5B);
	p922x_write_reg(chip, WRITE_REG_04, 0xFF);
	p922x_write_reg(chip, WRITE_REG_05, 0x00);

	p922x_write_reg(chip, WRITE_REG_TX, 0x55); 
}

#else

static int p922x_set_tx_charger_dect(struct oppo_p922x_ic *chip)
{
	/*临时措施，清中断*/
	char write_data[2] = {0, 0};

	write_data[0] = 0x10;
	write_data[1] = 0x00;
	p922x_write_reg_multi_byte(chip, 0x0038, write_data, 2);
	
	write_data[0] = 0x10;
	write_data[1] = 0x00;
	p922x_write_reg_multi_byte(chip, 0x0056, write_data, 2);

	write_data[0] = 0x20;
	write_data[1] = 0x00;
	p922x_write_reg_multi_byte(chip, 0x004E, write_data, 2);
	/*临时措施，清中断*/

	//send the Q value
	p922x_config_interface(chip, 0x0050, 0x38, 0xFF);
	p922x_config_interface(chip, 0x0051, 0x48, 0xFF);
	p922x_config_interface(chip, 0x0052, 0x00, 0xFF);
	p922x_config_interface(chip, 0x0053, 0x32, 0xFF);

	p922x_config_interface(chip, 0x004E, 0x01, 0x01);//BIT0

	msleep(300);

	chg_err("<~WPC~>p922x_set_tx_charger_dect----------->\n");
	p922x_config_interface(chip, 0x0050, 0x48, 0xFF);
	p922x_config_interface(chip, 0x0051, P9221_CMD_INDENTIFY_ADAPTER, 0xFF);
	p922x_config_interface(chip, 0x0052, (~P9221_CMD_INDENTIFY_ADAPTER), 0xFF);
	p922x_config_interface(chip, 0x0053, 0xFF, 0xFF);
	p922x_config_interface(chip, 0x0054, 0x00, 0xFF);

	p922x_config_interface(chip, 0x004E, 0x01, 0x01);//BIT0

	return 0;
}

static int p922x_set_tx_charger_fastcharge(struct oppo_p922x_ic *chip)
{
	chg_err("<~WPC~>p922x_set_tx_charger_fastcharge----------->\n");
	p922x_config_interface(chip, 0x0050, 0x48, 0xFF);
	p922x_config_interface(chip, 0x0051, P9221_CMD_INTO_FASTCHAGE, 0xFF);
	p922x_config_interface(chip, 0x0052, (~P9221_CMD_INTO_FASTCHAGE), 0xFF);
	p922x_config_interface(chip, 0x0053, 0xFF, 0xFF);
	p922x_config_interface(chip, 0x0054, 0x00, 0xFF);

	p922x_config_interface(chip, 0x004E, 0x01, 0x01);//BIT0

	return 0;
}

static int p922x_set_tx_charger_usb(struct oppo_p922x_ic *chip)
{
	chg_err("<~WPC~>p922x_set_tx_charger_usb----------->\n");
	p922x_config_interface(chip, 0x0050, 0x48, 0xFF);
	p922x_config_interface(chip, 0x0051, P9221_CMD_INTO_USB_CHARGE, 0xFF);
	p922x_config_interface(chip, 0x0052, (~P9221_CMD_INTO_USB_CHARGE), 0xFF);
	p922x_config_interface(chip, 0x0053, 0xFF, 0xFF);
	p922x_config_interface(chip, 0x0054, 0x00, 0xFF);

	p922x_config_interface(chip, 0x004E, 0x01, 0x01);//BIT0

	return 0;
}

static int p922x_set_tx_charger_normal(struct oppo_p922x_ic *chip)
{
	chg_err("<~WPC~>p922x_set_tx_charger_normal----------->\n");
	p922x_config_interface(chip, 0x0050, 0x48, 0xFF);
	p922x_config_interface(chip, 0x0051, P9221_CMD_INTO_NORMAL_CHARGE, 0xFF);
	p922x_config_interface(chip, 0x0052, (~P9221_CMD_INTO_NORMAL_CHARGE), 0xFF);
	p922x_config_interface(chip, 0x0053, 0xFF, 0xFF);
	p922x_config_interface(chip, 0x0054, 0x00, 0xFF);

	p922x_config_interface(chip, 0x004E, 0x01, 0x01);//BIT0

	return 0;
}
#endif

#ifdef WPC_USE_SP6001
static void p922x_set_rx_charge_voltage(struct oppo_p922x_ic *chip, int vol)
{
	char value_h, value_l;	
	int vout_set,vrect_set;

	if(vol < WPC_CHARGE_VOLTAGE_CHGPUMP_MIN) {
		vol = WPC_CHARGE_VOLTAGE_CHGPUMP_MIN;
		return;
	}
	else if(vol > WPC_CHARGE_VOLTAGE_CHGPUMP_MAX) {
		vol = WPC_CHARGE_VOLTAGE_CHGPUMP_MAX;
	}
	
	chip->p922x_chg_status.charge_voltage = vol;
	chg_err("<~WPC~> p922x_set_rx_charge_voltage: %d\n", vol);

	vout_set = (vol * 3352) >> 16;	
	vrect_set = ((vol + 100) * 2438) >> 16;	

	value_h = (char)(vout_set >> 8); 	
	value_l = (char)(vout_set & 0xFF);	
	p922x_write_reg(chip, WRITE_REG_01, value_h);
	p922x_write_reg(chip, WRITE_REG_02, value_l);

	value_h = (char)(vrect_set >> 8);	
	value_l = (char)(vrect_set & 0xFF);	
	p922x_write_reg(chip, WRITE_REG_03, value_h);
	p922x_write_reg(chip, WRITE_REG_04, value_l);
		
	p922x_write_reg(chip, WRITE_REG_00, 0x80);
	p922x_write_reg(chip, WRITE_REG_TX, 0x55); 
}
#else
static void p922x_set_rx_charge_voltage(struct oppo_p922x_ic *chip, int vol)
{
	char write_data[2] = {0, 0};

	chg_err("<~WPC~> p922x_set_rx_charge_voltage: %d\n", vol);

	chip->p922x_chg_status.charge_voltage = vol;

	write_data[0] = vol & 0x00FF;
	write_data[1] = (vol & 0xFF00) >> 8;

	p922x_write_reg_multi_byte(chip, 0x003E, write_data, 2);
}
#endif

static void p922x_set_rx_charge_current(struct oppo_p922x_ic *chip, int chg_current)
{
	chg_err("<~WPC~> set charge current: %d\n", chg_current);
	chip->p922x_chg_status.charge_current = chg_current;
	mp2650_charging_current_write_fast(chip->p922x_chg_status.charge_current);
}

static void p922x_set_rx_terminate_voltage(struct oppo_p922x_ic *chip, int vol_value)
{
	chg_err("<~WPC~> set terminate voltage: %d\n", vol_value);
	chip->p922x_chg_status.terminate_voltage = vol_value;
	//mp2650_float_voltage_write(vol_value);
}

static void p922x_set_rx_terminate_current(struct oppo_p922x_ic *chip, int curr_value)
{
	chg_err("<~WPC~> set terminate current: %d\n", curr_value);
	chip->p922x_chg_status.terminate_current = curr_value;
	//mp2650_set_termchg_current(curr_value);
}

static int p922x_increase_vc_by_step(struct oppo_p922x_ic *chip, int src_value, int limit_value, int step_value)
{
	int temp_value;
	
	if (src_value >= limit_value) {
		return 0;
	}

	temp_value = src_value + step_value;
	if (temp_value > limit_value) {
		temp_value = limit_value;
	}

	return temp_value;
}

static int p922x_decrease_vc_by_step(struct oppo_p922x_ic *chip, int src_value, int limit_value, int step_value)
{
	int temp_value = 0;
	
	if (src_value <= limit_value) {
		return 0;
	}

	if (src_value >= step_value) {
		temp_value = src_value - step_value;
	}
	
	if (temp_value < limit_value) {
		temp_value = limit_value;
	}

	return temp_value;
}

static void p922x_reset_variables(struct oppo_p922x_ic *chip)
{
	int i;
	chip->quiet_mode_need = 0;
	chip->p922x_chg_status.tx_online = false;
	chip->p922x_chg_status.freq_threshold = 130;
	chip->p922x_chg_status.freq_check_count = 0;
	chip->p922x_chg_status.freq_thr_inc = false;
	chip->p922x_chg_status.is_deviation = false;
	chip->p922x_chg_status.deviation_check_done = false;
	chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_DEFAULT;
	chip->p922x_chg_status.charge_online = false;
	chip->p922x_chg_status.send_message = P9221_CMD_NULL;
	chip->p922x_chg_status.adapter_type = ADAPTER_TYPE_UNKNOW;
	chip->p922x_chg_status.charge_type = WPC_CHARGE_TYPE_DEFAULT;
	chip->p922x_chg_status.dock_version = 0;
	chip->p922x_chg_status.charge_voltage = 0;
	chip->p922x_chg_status.cc_value_limit = WPC_CHARGE_CURRENT_FASTCHG;
	chip->p922x_chg_status.charge_current = 0;
	chip->p922x_chg_status.CEP_ready = false;
	chip->p922x_chg_status.temperature_status = TEMPERATURE_STATUS_UNKNOW;
	//chip->p922x_chg_status.termination_status = WPC_TERMINATION_STATUS_UNDEFINE;
	chip->p922x_chg_status.vrect = 0;
	chip->p922x_chg_status.vout = 0;
	chip->p922x_chg_status.iout = 0;
	chip->p922x_chg_status.fastchg_ing = false;
	chip->p922x_chg_status.epp_working = false;
	chip->p922x_chg_status.idt_fw_updating = false;
	chip->p922x_chg_status.wpc_reach_stable_charge = false;
	chip->p922x_chg_status.wpc_reach_4370mv = false;
	chip->p922x_chg_status.wpc_reach_4500mv = false;
	chip->p922x_chg_status.wpc_ffc_charge = false;
	chip->p922x_chg_status.wpc_skewing_proc = false;
	chip->p922x_chg_status.rx_runing_mode = RX_RUNNING_MODE_BPP;
#ifdef DEBUG_FASTCHG_BY_ADB
	chip->p922x_chg_status.vout_debug_mode = false;
	chip->p922x_chg_status.iout_debug_mode = false;
#endif

	for (i = 0; i < 10; i++) {
		chip->p922x_chg_status.iout_array[i] = 0;
	}
}

static void p922x_init_registers(struct oppo_p922x_ic *chip)
{
#ifndef WPC_USE_SP6001
	char write_data[2] = {0, 0};

	write_data[0] = 0x10;
	write_data[1] = 0x00;
	p922x_write_reg_multi_byte(chip, 0x0038, write_data, 2);

	write_data[0] = 0x30;
	write_data[1] = 0x00;
	p922x_write_reg_multi_byte(chip, 0x0056, write_data, 2);


	write_data[0] = 0x20;
	write_data[1] = 0x00;
	p922x_write_reg_multi_byte(chip, 0x004E, write_data, 2);
#endif
}

//static void p922x_read_debug_registers(struct oppo_p922x_ic *chip)
//{
//	char debug_data[10];
	
//	p922x_read_reg(chip, 0x04B0, debug_data, 10);
//	chg_err("<linshangbo> P9415 DEBUG DATA: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
//			debug_data[0], debug_data[1], debug_data[2], debug_data[3], debug_data[4], debug_data[5], debug_data[6], debug_data[7], debug_data[8], debug_data[9]);

//}

//chip->p922x_chg_status.adapter_type
int p922x_wpc_get_adapter_type(void)
{
    if (!p922x_chip) {
        //chg_err("<~WPC~> p922x_chip is NULL\n");
        return false;
    } else {
        return p922x_chip->p922x_chg_status.adapter_type;
    }
}

bool p922x_wpc_get_normal_charging(void)
{
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return false;
	} 
	
	if(p922x_chip->wireless_mode == WIRELESS_MODE_RX) {
		if(p922x_chip->p922x_chg_status.fastchg_ing) {
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
}

bool p922x_wpc_get_fast_charging(void)
{
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return false;
	} 

	if(p922x_chip->wireless_mode == WIRELESS_MODE_RX) {
		if(p922x_chip->p922x_chg_status.fastchg_ing) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

}

bool p922x_wpc_get_otg_charging(void)
{
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return false;
	} 
	
	if(p922x_chip->wireless_mode == WIRELESS_MODE_TX) {
		return true;
	} else {
		return false;
	}

}

int p922x_wpc_check_vbatt_full(void)
{
	static int batt_full_cnt = 0;
	
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return -1;
	}

	if (!p922x_chip->p922x_chg_status.charge_online) {
		chg_err("<~WPC~> p922x_chip is not online\n");
		return -1;
	}

	if (!p922x_chip->p922x_chg_status.wpc_ffc_charge) {
		chg_err("<~WPC~> It isn't in FFC!\n");
		return -1;
	}

	chg_err("<~WPC~> p922x_wpc_check_vbatt_full\n");

	if (g_oppo_chip->batt_volt >= p922x_chip->p922x_chg_status.terminate_voltage) {
		if ((g_oppo_chip->icharging < 0) 
		    && ((g_oppo_chip->icharging * -1) <= p922x_chip->p922x_chg_status.terminate_current) 
		    && (g_oppo_chip->soc == 100)) {
			batt_full_cnt++;
			if (batt_full_cnt > WPC_BATT_FULL_CNT) {
				batt_full_cnt = 0;
				chg_err("<~WPC~> Battery full!\n");

				//p922x_chip->p922x_chg_status.vbatt_full = true;
				return 1;
			}
		} else if (g_oppo_chip->icharging >= 0) {
			batt_full_cnt++;
			if (batt_full_cnt > (WPC_BATT_FULL_CNT * 2)) {
				batt_full_cnt = 0;
				chg_err("<~WPC~> Battery full by 'icharging > 0'!\n");

				//p922x_chip->p922x_chg_status.vbatt_full = true;
				return 1;
			}
		} else {
			batt_full_cnt = 0;
		}
	} else {
		batt_full_cnt = 0;
	}

	return 0;
}

int p922x_wpc_check_recharge(void)
{
#if 1
	return -1;
#else
	static int recharge_cnt = 0;
	
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return -1;
	}

	if (!p922x_chip->p922x_chg_status.charge_online) {
		chg_err("<~WPC~> p922x_chip is not online\n");
		return -1;
	}

	chg_err("<~WPC~> p922x_wpc_check_recharge!\n");

	if ((g_oppo_chip->batt_volt) < (p922x_chip->p922x_chg_status.terminate_voltage - WPC_RECHARGE_VOLTAGE_OFFSET)) {
		recharge_cnt++;
		if (recharge_cnt > WPC_RECHARGE_CNT) {
			chg_err("<~WPC~> Begin recharge!\n");

			p922x_chip->p922x_chg_status.vbatt_full = false;
			return 1;
		}
	} else {
		recharge_cnt = 0;
	}

	return 0;
#endif
}

void p922x_set_rtx_function(bool is_on)
{
	if (!g_oppo_chip) {
		chg_err("<~WPC~> g_oppo_chip is NULL\n");
		return;
	}

	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return;
	}

	if (p922x_firmware_is_updating() == true) {
		chg_err("<~WPC~> FW is updating, return!\n");
		return;
	}

	if (is_on) {
		if (p922x_chip->p922x_chg_status.wpc_dischg_status != WPC_DISCHG_STATUS_OFF) {
			chg_err("<~WPC~> Rtx function has already enabled!\n");
			return;
		}

		chg_err("<~WPC~> Enable rtx function!\n");		
		mp2650_disable_charging();

		if (oppo_get_wired_chg_present() == true) {
			p922x_set_booster_en_val(1);
			p922x_set_ext2_wireless_otg_en_val(0);
		} else if (oppo_get_wired_otg_online() == true) {
			mp2650_set_mps_otg_voltage(true);
			mp2650_set_mps_otg_current();
			mp2650_wireless_set_mps_otg_en_val(1);
			oppo_set_wrx_otg_value(1);
		} else {
			oppo_set_wrx_en_value(1);
			mp2650_set_mps_otg_voltage(true);
			mp2650_set_mps_otg_current();
			mp2650_wireless_set_mps_otg_en_val(1);
			oppo_set_wrx_otg_value(1);
		}

		mp2650_otg_enable();

		p922x_chip->wireless_mode = WIRELESS_MODE_TX;

		msleep(1000);

#ifdef WPC_USE_SP6001
		p922x_write_reg(p922x_chip, 0x000d, 0x20);
#else
		cancel_delayed_work_sync(&p922x_chip->idt_dischg_work);
		schedule_delayed_work(&p922x_chip->idt_dischg_work, WPC_DISCHG_WAIT_READY_EVENT);
#endif

		p922x_chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_STATUS_ON;
	} else {
		if (p922x_chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_STATUS_OFF) {
			chg_err("<~WPC~> Rtx function has already disabled!\n");
			return;
		}

		chg_err("<~WPC~> Disable rtx function!\n");

#ifdef WPC_USE_SP6001
		p922x_write_reg(p922x_chip, 0x000d, 0x10);
#else
		cancel_delayed_work_sync(&p922x_chip->idt_dischg_work);
#endif
		mp2650_otg_disable();
		if (oppo_get_wired_chg_present() == true) {
			p922x_set_booster_en_val(0);
			p922x_set_ext2_wireless_otg_en_val(1);
		} else if (oppo_get_wired_otg_online() == true) {
			mp2650_wireless_set_mps_otg_en_val(0);
			oppo_set_wrx_otg_value(0);
		} else {
			mp2650_wireless_set_mps_otg_en_val(0);
			oppo_set_wrx_otg_value(0);
			mp2650_enable_charging();
			msleep(100);
			oppo_set_wrx_en_value(0);
		}

		mp2650_set_mps_otg_voltage(false);

		p922x_chip->wireless_mode = WIRELESS_MODE_NULL;
		p922x_chip->p922x_chg_status.wpc_dischg_status =
			WPC_DISCHG_STATUS_OFF;
		p922x_chip->p922x_chg_status.tx_online = false;

		p922x_chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_STATUS_OFF;
	}
}

int p922x_enable_ftm(bool enable)
{
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return -1;
	}

	chg_err("<~WPC~> p922x_enable_ftm: %d \n", enable);

	p922x_chip->p922x_chg_status.ftm_mode = enable;
	return 0;
}

void p922x_reg_print(void)
{
#ifndef WPC_USE_SP6001	
	char debug_data[13];

#if 0
	p922x_read_reg(p922x_chip, 0x4e0, debug_data, 13);
	chg_err("<~WPC~>0x4E0 REG: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X \n", 
			debug_data[0],debug_data[1],debug_data[2],debug_data[3],debug_data[4],debug_data[5],debug_data[6],debug_data[7],
			debug_data[8],debug_data[9],debug_data[10],debug_data[11],debug_data[12]);

	p922x_read_reg(p922x_chip, 0x6e, debug_data, 2);
	chg_err("<~WPC~>0x6E REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);

	p922x_read_reg(p922x_chip, 0x70, debug_data, 2);
	chg_err("<~WPC~>0x70 REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);


	p922x_read_reg(p922x_chip, 0x74, debug_data, 2);
	chg_err("<~WPC~>0x74 REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);


	p922x_read_reg(p922x_chip, 0x78, debug_data, 2);
	chg_err("<~WPC~>0x78 REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);


	p922x_read_reg(p922x_chip, 0x470, debug_data, 2);
	chg_err("<~WPC~>0x470 REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);
#endif

	if (p922x_chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_STATUS_ON) {
		p922x_read_reg(p922x_chip, 0x78, debug_data, 2);
		chg_err("<~WPC~>rtx func 0x78-->[0x%02x]!\n", debug_data[0]);
		chg_err("<~WPC~>rtx func 0x79-->[0x%02x]!\n", debug_data[1]);
		p922x_read_reg(p922x_chip, 0x70, debug_data, 1);
		chg_err("<~WPC~>rtx func 0x70-->[0x%02x]!\n", debug_data[0]);
		p922x_read_reg(p922x_chip, 0x6E, debug_data, 1);
		chg_err("<~WPC~>rtx func 0x6E-->[0x%02x]!\n", debug_data[0]);
	}

#endif
}

void p922x_wpc_print_log(void)
{
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL!\n");
		return;
	}

	p922x_reg_print();

	chg_err("<~WPC~>vbat_en[%d], idt_connect[%d], idt_int[%d], idt_en[%d], \
booster_en[%d], chargepump_en[%d], wrx_en[%d], wrx_otg_en[%d], mps_otg_en[%d], \
5V_en[%d], EXT2_otg_en[%d], EXT1_otg_en[%d]\n", 
							p922x_get_vbat_en_val(),
							p922x_get_idt_con_val(),
							p922x_get_idt_int_val(),
							oppo_get_idt_en_val(),
							p922x_get_booster_en_val(),
							chargepump_get_chargepump_en_val(),
							oppo_get_wrx_en_val(),
							oppo_get_wrx_otg_val(),
							mp2650_wireless_get_mps_otg_en_val(),
							p922x_get_cp_ldo_5v_val(),
							p922x_get_ext2_wireless_otg_en_val(),
							p922x_get_ext1_wired_otg_en_val());

	if (!p922x_chip->p922x_chg_status.charge_online) {
		chg_err("<~WPC~> p922x_chip is not online\n");
		return;
	}

	if ((p922x_chip->p922x_chg_status.charge_status == WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER)
	    || (p922x_chip->p922x_chg_status.charge_status == WPC_CHG_STATUS_FAST_CHARGING_FROM_CHGPUMP)) {
		chg_err("<~WPC~> ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	} else {
		chg_err("<~WPC~> -------------------------------------------------------\
--------------------------------------------------\n");
	}
	
	chg_err("<~WPC~> [Chg sta: %d] [Temp sta: %d] [Term sta: %d] [Adap type: %d] [Chg type: %d] [Dock Ver: %d] \
[Chg Vol: %d] [Chg Curr: %d] [Curr limit: %d]\n", 
			p922x_chip->p922x_chg_status.charge_status,
			p922x_chip->p922x_chg_status.temperature_status,
			p922x_chip->p922x_chg_status.termination_status,
			p922x_chip->p922x_chg_status.adapter_type,
			p922x_chip->p922x_chg_status.charge_type,
			p922x_chip->p922x_chg_status.dock_version,
			p922x_chip->p922x_chg_status.charge_voltage,
			p922x_chip->p922x_chg_status.charge_current,
			p922x_chip->p922x_chg_status.cc_value_limit);
			
	chg_err("<~WPC~> [Temp: %d] [Soc: %d] [Chg curr: %d] [Batt vol: %d]\
[Batt vol max: %d] [Batt vol min: %d] [Term Vol: %d] [Term Curr: %d] [Rechg Vol: %d]\n", 
			g_oppo_chip->temperature, 
			g_oppo_chip->soc,
			g_oppo_chip->icharging,
			g_oppo_chip->batt_volt, 
			g_oppo_chip->batt_volt_max, 
			g_oppo_chip->batt_volt_min,
			p922x_chip->p922x_chg_status.terminate_voltage, 
			p922x_chip->p922x_chg_status.terminate_current,
			p922x_chip->p922x_chg_status.terminate_voltage - WPC_RECHARGE_VOLTAGE_OFFSET);
}

int p922x_wireless_get_vout(void)
{
	if (!p922x_chip) {
		return 0;
	}
	return p922x_chip->p922x_chg_status.vout;
}

#ifdef SUPPORT_SP6001_FW_UPDATE
int sp6001_check_firmware_version(struct oppo_p922x_ic *chip, E_SP6001_AREA download_area)
{
	int addr = 0;
	int addr_h, addr_l;
	int i = 0;
	char read_buf[12];
	int ret;
	char *fw_data = NULL;
	int fw_data_lenth = 0;

	if (download_area == BOOT_AREA) {	
		chg_err("<SP UPDATE> check BOOT_AREA version begin\n");

		addr = 0;
		fw_data_lenth = sizeof(fw_data_boot);
		fw_data = fw_data_boot;
	} else if (download_area == RX_AREA) {
		chg_err("<SP UPDATE> check RX_AREA version begin\n");
		
		addr = 256;
		fw_data_lenth = sizeof(fw_data_rx);
		fw_data = fw_data_rx;
	} else if(download_area == TX_AREA) {
		chg_err("<SP UPDATE> check TX_AREA version begin\n");
		
		addr = 4864;
		fw_data_lenth = sizeof(fw_data_tx);
		fw_data = fw_data_tx;
	}

	/************prepare_for_mtp_read************/
	ret = p922x_write_reg(chip, 0x2017, 0x69);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x96);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x66);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x99);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2018, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x5a);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0xa5);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0017, 0x01);
	I2C_ERROR_HANDLE(ret);

	msleep(10);

	addr += (fw_data_lenth / 4);

	for (i = 0; i < 12; i += 4) {
		/************write_mtp_addr************/
		addr_h = (char)(addr >> 8);
		addr_l = (char)(addr & 0xff);
		ret = p922x_write_reg(chip, 0x0010, addr_h);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_write_reg(chip, 0x0011, addr_l);
		I2C_ERROR_HANDLE(ret);

		addr--;

		/************read pause************/
		ret = p922x_write_reg(chip, 0x0018, 0x02);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_write_reg(chip, 0x0018, 0x00);
		I2C_ERROR_HANDLE(ret);

		/************read data************/
		ret = p922x_read_reg(chip, 0x0013, &read_buf[i + 3], 1);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_read_reg(chip, 0x0014, &read_buf[i + 2], 1);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_read_reg(chip, 0x0015, &read_buf[i + 1], 1);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_read_reg(chip, 0x0016, &read_buf[i + 0], 1);
		I2C_ERROR_HANDLE(ret);
	}

	/***********end************/
	ret = p922x_write_reg(chip, 0x0017, 0x00);
	I2C_ERROR_HANDLE(ret);

	/************exit dtm************/
	ret = p922x_write_reg(chip, 0x2018, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x55);
	I2C_ERROR_HANDLE(ret);

	chg_err("<SP UPDATE> CHIP FW IDEN: 0x%02X 0x%02X 0x%02X 0x%02X\n", 
			read_buf[0], read_buf[1], read_buf[2], read_buf[3]);

	chg_err("<SP UPDATE> CHIP VERSION: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", 
			read_buf[4], read_buf[5], read_buf[6], read_buf[7], read_buf[8], read_buf[9], read_buf[10], read_buf[11]);

	chg_err("<SP UPDATE> FW   VERSION: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", 
			fw_data[fw_data_lenth - 4], fw_data[fw_data_lenth - 3], fw_data[fw_data_lenth - 2], fw_data[fw_data_lenth - 1],
			fw_data[fw_data_lenth - 8], fw_data[fw_data_lenth - 7], fw_data[fw_data_lenth - 6], fw_data[fw_data_lenth - 5]);

	if ((read_buf[0] == 0x99) && (read_buf[1] == 0x99)) {
		chg_err("<SP UPDATE> The firmware is integrated!\n");
	} else {
		chg_err("<SP UPDATE> The firmware is unintegrated!\n");
		return -1;
	}
	
	if ((read_buf[4] == fw_data[fw_data_lenth - 4]) && (read_buf[5] == fw_data[fw_data_lenth - 3]) 
		&& (read_buf[6] == fw_data[fw_data_lenth - 2]) && (read_buf[7] == fw_data[fw_data_lenth - 1])
		&& (read_buf[8] == fw_data[fw_data_lenth - 8]) && (read_buf[9] == fw_data[fw_data_lenth - 7])
		&& (read_buf[10] == fw_data[fw_data_lenth - 6]) && (read_buf[11] == fw_data[fw_data_lenth - 5]))
	{
		chg_err("<SP UPDATE> The firmware is the newest!\n");

		return 0;
	} else {
		chg_err("<SP UPDATE> The firmware needs update!\n");

		return -1;
	}

	return 0;
}

int sp6001_check_firmware(struct oppo_p922x_ic *chip, E_SP6001_AREA download_area)
{
	int addr = 0;
	int addr_h, addr_l;
	int i = 0;
	char read_buf[4] = {0, 0, 0, 0};
	int ret;
	char *fw_data = NULL;
	int fw_data_lenth = 0;

	success_count = 0;
	error_count = 0;

	for (i = 0; i < ERROR_BUFFER_NUM; i++) {
		error_buf[i] = 0;
	}
	
	if(download_area == BOOT_AREA) {
		chg_err("<SP UPDATE> check BOOT_AREA begin\n");
		
		addr = 0;
		fw_data_lenth = sizeof(fw_data_boot);
		fw_data = fw_data_boot;
	} else if(download_area == RX_AREA) {
		chg_err("<SP UPDATE> check RX_AREA begin\n");
		
		addr = 256;
		fw_data_lenth = sizeof(fw_data_rx);
		fw_data = fw_data_rx;
	} else if(download_area == TX_AREA) {
		chg_err("<SP UPDATE> check TX_AREA begin\n");
		
		addr = 4864;
		fw_data_lenth = sizeof(fw_data_tx);
		fw_data = fw_data_tx;
	}

	/************prepare_for_mtp_read************/
	ret = p922x_write_reg(chip, 0x2017, 0x69);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x96);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x66);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x99);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2018, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x5a);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0xa5);
	I2C_ERROR_HANDLE(ret);
	//C2
	ret = p922x_write_reg(chip, 0x0017, 0x01);
	I2C_ERROR_HANDLE(ret);

	msleep(10);

	for (i = 0; i < fw_data_lenth; i += 4) {
		addr_h = (int)(addr >> 8);
		addr_l = (int)(addr & 0xff);
		ret = p922x_write_reg(chip, 0x0010, addr_h);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_write_reg(chip, 0x0011, addr_l);
		I2C_ERROR_HANDLE(ret);

		addr++;

		/************read pause************/
		ret = p922x_write_reg(chip, 0x0018, 0x02);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_write_reg(chip, 0x0018, 0x00);
		I2C_ERROR_HANDLE(ret);

		/************read data************/
		ret = p922x_read_reg(chip, 0x0013, &read_buf[3], 1);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_read_reg(chip, 0x0014, &read_buf[2], 1);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_read_reg(chip, 0x0015, &read_buf[1], 1);
		I2C_ERROR_HANDLE(ret);
		ret = p922x_read_reg(chip, 0x0016, &read_buf[0], 1);
		I2C_ERROR_HANDLE(ret);

		//chg_err("<SP UPDATE> 0x%02X 0x%02X 0x%02X 0x%02X ---- %d\n", read_buf[0],read_buf[1],read_buf[2],read_buf[3], addr-1);
		if ((read_buf[0] == fw_data[i+0]) && (read_buf[1] == fw_data[i+1]) 
			&&(read_buf[2] == fw_data[i+2]) && (read_buf[3] == fw_data[i+3])) {
			success_count++;	
		} else {				
			if (error_count < ERROR_BUFFER_NUM) {
				error_buf[error_count] = i / 4;

				chg_err("<SP UPDATE> find error idx: %d, addr: %d\n", error_count, error_buf[error_count]);

				error_count++;
				if (error_count >= ERROR_BUFFER_NUM) {
					goto CHECK_FINISH;
				}
			}
		}
			
		msleep(20);
	}

CHECK_FINISH:
	ret = p922x_write_reg(chip, 0x0017, 0x00);
	I2C_ERROR_HANDLE(ret);

	/***************exit dtm*****************/
	ret = p922x_write_reg(chip, 0x2018, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x55);
	I2C_ERROR_HANDLE(ret);

	chg_err("<SP UPDATE> check total = %d, success = %d, error: %d \n", fw_data_lenth, success_count*4, error_count);

	if (success_count < 10) {
		return -1;
	} else if (error_count > 0) {
		return -2;
	} else {
		return 0;
	}
}

int sp6001_repair_firmware(struct oppo_p922x_ic *chip, E_SP6001_AREA download_area)
{
	int addr = 0;
	int addr_offset = 0;
	int addr_h, addr_l;
	int m = 0;
	int ret;
	char *fw_data = NULL;

	if (error_count == 0) {
		chg_err("<SP UPDATE> There is no error!\n");
		return 0;
	}

	if(download_area == BOOT_AREA) {
		chg_err("<SP UPDATE> repair BOOT_AREA begin\n");
		
		addr_offset = 0;
		fw_data = fw_data_boot;
	} else if(download_area == RX_AREA) {
		chg_err("<SP UPDATE> repair RX_AREA begin\n");
		
		addr_offset = 256;
		fw_data = fw_data_rx;
	} else if(download_area == TX_AREA) {
		chg_err("<SP UPDATE> repair TX_AREA begin\n");
		
		addr_offset = 4864;
		fw_data = fw_data_tx;
	}
	
	for (m = 0; m < error_count; m++) {
		if (error_buf[m] != 0) {
			/************prepare_for_mtp_write************/
			ret = p922x_write_reg(chip, 0x2017, 0x69);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2017, 0x96);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2017, 0x66);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2017, 0x99);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2018, 0xff);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2019, 0xff);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x0001, 0x5a);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x0003, 0xa5);
			I2C_ERROR_HANDLE(ret);
			//C2
			ret = p922x_write_reg(chip, 0x0017, 0x01);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x1000, 0x20);
			I2C_ERROR_HANDLE(ret);

			msleep(20);
			
			chg_err("<SP UPDATE> repair address:  %d\n", error_buf[m]);

			/************write data************/
			addr = addr_offset + error_buf[m]; //boot area				
			
			addr_h = (int)(addr >> 8);
			addr_l = (int)(addr & 0xff);
			ret = p922x_write_reg(chip, 0x0010, addr_h);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x0011, addr_l);
			I2C_ERROR_HANDLE(ret);
			
			/************enable write************/
			ret = p922x_write_reg(chip, 0x001a, 0x5a);
			I2C_ERROR_HANDLE(ret);
			
			//msleep(2);
			usleep_range(1000, 1100);
			ret = p922x_write_reg(chip, 0x0012, fw_data[error_buf[m] * 4 + 3]);
			I2C_ERROR_HANDLE(ret);
			//msleep(2);
			usleep_range(1000, 1100);
			ret = p922x_write_reg(chip, 0x0012, fw_data[error_buf[m] * 4 + 2]);
			I2C_ERROR_HANDLE(ret);
			//msleep(2);
			usleep_range(1000, 1100);
			ret = p922x_write_reg(chip, 0x0012, fw_data[error_buf[m] * 4 + 1]);
			I2C_ERROR_HANDLE(ret);
			//msleep(2);
			usleep_range(1000, 1100);
			ret = p922x_write_reg(chip, 0x0012, fw_data[error_buf[m] * 4 + 0]);
			I2C_ERROR_HANDLE(ret);
			//msleep(20);			

			ret = p922x_write_reg(chip, 0x001a, 0x00);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x0017, 0x00);
			I2C_ERROR_HANDLE(ret);

			/***************exit dtm*****************/
			ret = p922x_write_reg(chip, 0x2018, 0x00);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2019, 0x00);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x0001, 0x00);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x0003, 0x00);
			I2C_ERROR_HANDLE(ret);
			ret = p922x_write_reg(chip, 0x2017, 0x55);
			I2C_ERROR_HANDLE(ret);

			msleep(50);
		}
	}

	return 0;
}

int sp6001_download_firmware(struct oppo_p922x_ic *chip, E_SP6001_AREA download_area)
{
	int addr = 0;
	int addr_h, addr_l;
	int i = 0;
	int ret;
	char *fw_data = NULL;
	int fw_data_lenth = 0;

	if (download_area == BOOT_AREA) {
		chg_err("<SP UPDATE> download BOOT_AREA begin\n");

		addr = 0;
		fw_data_lenth = sizeof(fw_data_boot);
		fw_data = fw_data_boot;
	} else if(download_area == RX_AREA) {
		chg_err("<SP UPDATE> download RX_AREA begin\n");
		
		addr = 256;
		fw_data_lenth = sizeof(fw_data_rx);
		fw_data = fw_data_rx;
	} else if(download_area == TX_AREA) {
		chg_err("<SP UPDATE> download TX_AREA begin\n");
		
		addr = 4864;
		fw_data_lenth = sizeof(fw_data_tx);
		fw_data = fw_data_tx;
	}

	/************prepare_for_mtp_write************/
	ret = p922x_write_reg(chip, 0x2017, 0x69);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x96);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x66);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x99);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2018, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x5a);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0xa5);
	I2C_ERROR_HANDLE(ret);
	//C2
	ret = p922x_write_reg(chip, 0x0017, 0x01);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x1000, 0x20);
	I2C_ERROR_HANDLE(ret);

	msleep(20);

	/************write_mtp_addr************/
	addr_h = (int)(addr >> 8);
	addr_l = (int)(addr & 0xff);
	ret = p922x_write_reg(chip, 0x0010, addr_h);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0011, addr_l);
	I2C_ERROR_HANDLE(ret);

	/************enable write************/
	ret = p922x_write_reg(chip, 0x001a, 0x5a);
	I2C_ERROR_HANDLE(ret);

	/************write data************/
	chg_err("<SP UPDATE> firmware size: %d\n", fw_data_lenth);
	
	for(i = 0; i < fw_data_lenth; i += 4) {
		//msleep(2);
		usleep_range(1000, 1100);
		ret = p922x_write_reg(chip, 0x0012, fw_data[i + 3]);
		I2C_ERROR_HANDLE(ret);
		//msleep(2);
		usleep_range(1000, 1100);
		ret = p922x_write_reg(chip, 0x0012, fw_data[i + 2]);
		I2C_ERROR_HANDLE(ret);
		//msleep(2);
		usleep_range(1000, 1100);
		ret = p922x_write_reg(chip, 0x0012, fw_data[i + 1]);
		I2C_ERROR_HANDLE(ret);
		//msleep(2);
		usleep_range(1000, 1100);
		ret = p922x_write_reg(chip, 0x0012, fw_data[i + 0]);
		I2C_ERROR_HANDLE(ret);
		//msleep(20);
	}

	ret = p922x_write_reg(chip, 0x001a, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0017, 0x00);
	I2C_ERROR_HANDLE(ret);

	/***************exit dtm*****************/
	ret = p922x_write_reg(chip, 0x2018, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x55);
	I2C_ERROR_HANDLE(ret);

	chg_err("<SP UPDATE> download firmware finish!\n");

	return 0;
}

bool sp6001_set_fw_update_result(struct oppo_p922x_ic *chip, E_SP6001_AREA download_area, bool isOK)
{
	int addr = 0;
	int addr_h, addr_l;
	int ret;
	int fw_data_lenth = 0;

	if (download_area == BOOT_AREA) {
		chg_err("<SP UPDATE> set BOOT_AREA result\n");

		addr = 0;
		fw_data_lenth = sizeof(fw_data_boot);
	} else if(download_area == RX_AREA) {
		chg_err("<SP UPDATE> set RX_AREA result\n");
		
		addr = 256;
		fw_data_lenth = sizeof(fw_data_rx);
	} else if(download_area == TX_AREA) {
		chg_err("<SP UPDATE> set TX_AREA result\n");
		
		addr = 4864;
		fw_data_lenth = sizeof(fw_data_tx);
	}

	/************prepare_for_mtp_write************/
	ret = p922x_write_reg(chip, 0x2017, 0x69);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x96);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x66);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x99);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2018, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0xff);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x5a);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0xa5);
	I2C_ERROR_HANDLE(ret);
	//C2
	ret = p922x_write_reg(chip, 0x0017, 0x01);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x1000, 0x20);
	I2C_ERROR_HANDLE(ret);

	msleep(20);

	/************write_mtp_addr************/
	addr += (fw_data_lenth / 4);
	addr_h = (int)(addr >> 8);
	addr_l = (int)(addr & 0xff);
	ret = p922x_write_reg(chip, 0x0010, addr_h);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0011, addr_l);
	I2C_ERROR_HANDLE(ret);

	/************enable write************/
	ret = p922x_write_reg(chip, 0x001a, 0x5a);
	I2C_ERROR_HANDLE(ret);

	/************write data************/
	if (isOK){
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x99);
		I2C_ERROR_HANDLE(ret);
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x99);
		I2C_ERROR_HANDLE(ret);
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x99);
		I2C_ERROR_HANDLE(ret);
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x99);
		I2C_ERROR_HANDLE(ret);
	} else {
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x00);
		I2C_ERROR_HANDLE(ret);
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x00);
		I2C_ERROR_HANDLE(ret);
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x00);
		I2C_ERROR_HANDLE(ret);
		mdelay(2);
		ret = p922x_write_reg(chip, 0x0012, 0x00);
		I2C_ERROR_HANDLE(ret);
	}
	
	/************end************/
	ret = p922x_write_reg(chip, 0x001a, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0017, 0x00);
	I2C_ERROR_HANDLE(ret);

	/************exit dtm************/
	ret = p922x_write_reg(chip, 0x2018, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2019, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0001, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x0003, 0x00);
	I2C_ERROR_HANDLE(ret);
	ret = p922x_write_reg(chip, 0x2017, 0x55);

	return 0;
}

#define UPDATE_RETRY_MAX		10
static int p922x_check_idt_fw_update(struct oppo_p922x_ic *chip)
{
	int idt_update_retry_cnt = 0;
	int rc = -1;

	chg_err("<SP UPDATE> check sp6001 fw <><><><><><><><>\n");

	if (!chip) {
		chg_err("<SP UPDATE> sp6001 isn't ready!\n");
		return rc;
	}

	if (g_oppo_chip->charger_exist == 1) {
		chg_err("<SP UPDATE>g_oppo_chip->charger_exist == 1, return!\n");
		chip->p922x_chg_status.check_fw_update = true;
		return 0;
	}

	if (p922x_chip->p922x_chg_status.charge_online) {
		chg_err("<SP UPDATE>p922x_chip->p922x_chg_status.charge_online == 1, return!\n");
		chip->p922x_chg_status.check_fw_update = true;
		return 0;
	}
	
	chip->p922x_chg_status.idt_fw_updating = true;
	mp2650_disable_charging();
	mp2650_set_mps_otg_voltage(false);
	mp2650_set_mps_otg_current();
	mp2650_otg_enable();

	mp2650_wireless_set_mps_otg_en_val(1);
	oppo_set_wrx_en_value(1);
	oppo_set_wrx_otg_value(1);
	oppo_wireless_set_otg_en_val(0);
	msleep(1000);

	disable_irq(chip->idt_con_irq);

	rc = sp6001_check_firmware_version(chip, BOOT_AREA);
	if (rc == -1) {	
		msleep(50);
		sp6001_download_firmware(chip, BOOT_AREA);
		
		idt_update_retry_cnt = 0;
		while(idt_update_retry_cnt < UPDATE_RETRY_MAX) {
			chg_err("<SP UPDATE> check BOOT_AREA cnt: %d!\n", idt_update_retry_cnt);
			msleep(50);
			rc = sp6001_check_firmware(chip, BOOT_AREA);
			if (rc == -3) {
				chg_err("<SP UPDATE> i2c error, exit BOOT_AREA update!\n");
				idt_update_retry_cnt = UPDATE_RETRY_MAX;
				break;
			} else if (rc == -2) {
				chg_err("<SP UPDATE> do repair!\n");
				msleep(50);
				sp6001_repair_firmware(chip, BOOT_AREA);
			} else if (rc == -1) {
				chg_err("<SP UPDATE> do reprogram!\n");
				msleep(50);
				sp6001_download_firmware(chip, BOOT_AREA);
			} else {
				chg_err("<SP UPDATE> do BOOT_AREA ok!\n");
				msleep(50);
				sp6001_set_fw_update_result(chip, BOOT_AREA, true);
				break;
			}

			idt_update_retry_cnt++;
		}
	}

	if (idt_update_retry_cnt >= UPDATE_RETRY_MAX) {
		chg_err("<SP UPDATE> do BOOT_AREA fail!\n");
		msleep(50);
		sp6001_set_fw_update_result(chip, BOOT_AREA, false);
	}
	
	rc = sp6001_check_firmware_version(chip, RX_AREA);
	if (rc == -1) {
		msleep(50);
		sp6001_download_firmware(chip, RX_AREA);
		
		idt_update_retry_cnt = 0;
		while(idt_update_retry_cnt < UPDATE_RETRY_MAX) {
			chg_err("<SP UPDATE> check RX_AREA cnt: %d!\n", idt_update_retry_cnt);
			msleep(50);
			rc = sp6001_check_firmware(chip, RX_AREA);
			if (rc == -3) {
				chg_err("<SP UPDATE> i2c error, exit RX_AREA update!\n");
				idt_update_retry_cnt = UPDATE_RETRY_MAX;
				break;
			} else if (rc == -2) {
				chg_err("<SP UPDATE> do repair!\n");
				msleep(50);
				sp6001_repair_firmware(chip, RX_AREA);
			} else if (rc == -1) {
				chg_err("<SP UPDATE> do reprogram!\n");
				msleep(50);
				sp6001_download_firmware(chip, RX_AREA);
			} else {
				chg_err("<SP UPDATE> do RX_AREA ok!\n");
				msleep(50);
				sp6001_set_fw_update_result(chip, RX_AREA, true);
				break;
			}

			idt_update_retry_cnt++;
		}
	}

	if (idt_update_retry_cnt >= UPDATE_RETRY_MAX) {
		chg_err("<SP UPDATE> do RX_AREA fail!\n");
		msleep(50);
		sp6001_set_fw_update_result(chip, RX_AREA, false);
	}

	rc = sp6001_check_firmware_version(chip, TX_AREA);
	if (rc == -1) {
		msleep(50);
		sp6001_download_firmware(chip, TX_AREA);
		
		idt_update_retry_cnt = 0;
		while(idt_update_retry_cnt < UPDATE_RETRY_MAX) {
			chg_err("<SP UPDATE> check TX_AREA cnt: %d!\n", idt_update_retry_cnt);
			msleep(50);
			rc = sp6001_check_firmware(chip, TX_AREA);
			if (rc == -3) {
				chg_err("<SP UPDATE> i2c error, exit TX_AREA update!\n");
				idt_update_retry_cnt = UPDATE_RETRY_MAX;
				break;
			} else if (rc == -2) {
				chg_err("<SP UPDATE> do repair!\n");
				msleep(50);
				sp6001_repair_firmware(chip, TX_AREA);
			} else if (rc == -1) {
				chg_err("<SP UPDATE> do reprogram!\n");
				msleep(50);
				sp6001_download_firmware(chip, TX_AREA);
			} else {
				chg_err("<SP UPDATE> do TX_AREA ok!\n");
				msleep(50);
				sp6001_set_fw_update_result(chip, TX_AREA, true);
				break;
			}

			idt_update_retry_cnt++;
		}
	}

	if (idt_update_retry_cnt >= UPDATE_RETRY_MAX) {
		chg_err("<SP UPDATE> do TX_AREA fail!\n");
		msleep(50);
		sp6001_set_fw_update_result(chip, TX_AREA, false);
	}

	mp2650_otg_disable();
	mp2650_otg_wait_vbus_decline();

	mp2650_wireless_set_mps_otg_en_val(0);
	oppo_set_wrx_en_value(0);
	oppo_set_wrx_otg_value(0);

	enable_irq(chip->idt_con_irq);

	chip->p922x_chg_status.idt_fw_updating = false;

	return 0;
}

#else
#ifdef NO_FW_UPGRADE_CRC
static int p922x_MTP(struct oppo_p922x_ic *chip)
{
	int rc;
	int i, j;
	int fw_size;
	unsigned char *fw_data;
	unsigned char write_ack;
	unsigned short int StartAddr;
	unsigned short int CheckSum;
	unsigned short int CodeLength;

	chg_err("<IDT UPDATE>--1--!\n");
	// configure the system
	rc = __p922x_write_reg(chip, 0x3000, 0x5a); // write key
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3000 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>--2--!\n");
	rc = __p922x_write_reg(chip, 0x3040, 0x10); // halt M0
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3000 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>--3--!\n");
	rc = p922x_write_reg_multi_byte(chip, 0x1c00, MTPBootloader9320, sizeof(MTPBootloader9320));  // load provided by IDT array
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x1c00 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>--4--!\n");
	rc = __p922x_write_reg(chip, 0x400, 0); // initialize buffer
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x400 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>--5--!\n");
	rc = __p922x_write_reg(chip, 0x3048, 0x80); // map RAM address 0x1c00 to OTP 0x0000
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3048 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>--6--!\n");
	rc = __p922x_write_reg(chip, 0x3040, 0x80); // run M0
	// this global variable is used by the i2c driver to block ACK error message

	msleep(100);

	fw_size = sizeof(idt_firmware);
	chg_err("<IDT UPDATE>The idt firmware size: %d!\n", fw_size);

	// program pages of 128 bytes
	fw_data = kzalloc(144, GFP_KERNEL);
	if (!fw_data) {
		chg_err("<IDT UPDATE>can't alloc memory!\n");
		return -1;
	}

	for (i = 0; i < fw_size; i += 128) {
		chg_err("<IDT UPDATE>Begin to write chunk %d!\n", i);
		
		StartAddr = i;
		CheckSum = StartAddr;
		CodeLength = 128;

		memcpy(fw_data + 8, idt_firmware + i, 128);

		j = fw_size - i;
		if (j < 128)
		{
			j = ((j + 15) / 16) * 16;
			CodeLength = (unsigned short int)j;
		}
		else
		{
			j = 128;
		}
		
		j -= 1;
		for (; j >= 0; j--) {
			CheckSum += fw_data[j + 8]; // add the non zero values
		}
		
		CheckSum += CodeLength; // finish calculation of the check sum

		memcpy(fw_data + 2, (char *)&StartAddr, 2);
		memcpy(fw_data + 4, (char *)&CodeLength, 2);
		memcpy(fw_data + 6, (char *)&CheckSum, 2);
		
		//typedef struct { // write to structure at address 0x400
		// u16 Status;
		// u16 StartAddr;
		// u16 CodeLength;
		// u16 DataChksum;
		// u8 DataBuf[128];
		//} P9220PgmStrType;
		// read status is guaranteed to be != 1 at this point

		rc = p922x_write_reg_multi_byte(chip, 0x400, fw_data, ((CodeLength + 8 + 15) / 16) * 16); 
		if (rc != 0) {
			chg_err("<IDT UPDATE>ERROR: Write fw data error!\n");
			goto MTP_ERROR;
		}	

		rc = __p922x_write_reg(chip, 0x400, 0x01);
		if (rc != 0) {
			chg_err("<IDT UPDATE>ERROR: on OTP buffer validation\n");
			goto MTP_ERROR;
		}

		do {
			msleep(20);

			rc = p922x_read_reg(chip, 0x400, &write_ack, 1);
			if (rc != 0) {
				chg_err("<IDT UPDATE>ERROR: on reading OTP buffer status\n");
				goto MTP_ERROR;
			}
		} while ((write_ack & 0x01) != 0);
		
		// check status
		if (write_ack != 2) // not OK
		{
			if (write_ack == 4) {
				chg_err("<IDT UPDATE>ERROR: WRITE ERR\n");
			}
			else if (write_ack == 8) {
				chg_err("<IDT UPDATE>ERROR: CHECK SUM ERR\n");
			}
			else {
				chg_err("<IDT UPDATE>ERROR: UNKNOWN ERR\n");
			}
			
			goto MTP_ERROR;
		}
	}


	// restore system
	rc = __p922x_write_reg(chip, 0x3000, 0x5a); // write key
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3000 reg error!\n");
		return rc;
	}

	rc = __p922x_write_reg(chip, 0x3048, 0x00); // remove code remapping
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3048 reg error!\n");
		return rc;
	}
	
	chg_err("<IDT UPDATE>OTP Programming finished\n");

	kfree(fw_data);
	return 0;

MTP_ERROR:
	kfree(fw_data);
	return -1;
}
#else
static int p922x_load_bootloader(struct oppo_p922x_ic *chip)
{
	int rc = 0;
	// configure the system
	rc = __p922x_write_reg(chip, 0x3000, 0x5a); // write key
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3000 reg error!\n");
		return rc;
	}

	rc = __p922x_write_reg(chip, 0x3004, 0x00); // set HS clock
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3004 reg error!\n");
		return rc;
	}

	rc = __p922x_write_reg(chip, 0x3008, 0x09); // set AHB clock
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3008 reg error!\n");
		return rc;
	}

	rc = __p922x_write_reg(chip, 0x300C, 0x05); // configure 1us pulse
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x300c reg error!\n");
		return rc;
	}

	rc = __p922x_write_reg(chip, 0x300D, 0x1d); // configure 500ns pulse
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x300d reg error!\n");
		return rc;
	}

	rc = __p922x_write_reg(chip, 0x3040, 0x11); // Enable MTP access via I2C
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3040 reg error!\n");
		return rc;
	}

	msleep(20);

	chg_err("<IDT UPDATE>-b-2--!\n");
	rc = __p922x_write_reg(chip, 0x3040, 0x10); // halt microcontroller M0
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3040 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>-b-3--!\n");
	rc = p922x_write_reg_multi_byte(
		chip, 0x0800, MTPBootloader9320,
		sizeof(MTPBootloader9320)); // load provided by IDT array
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x1c00 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>-b-4--!\n");
	rc = __p922x_write_reg(chip, 0x400, 0); // initialize buffer
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x400 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>-b-5--!\n");
	rc = __p922x_write_reg(chip, 0x3048, 0xD0); // map RAM address 0x1c00 to OTP 0x0000
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3048 reg error!\n");
		return rc;
	}

	chg_err("<IDT UPDATE>-b-6--!\n");
	rc = __p922x_write_reg(chip, 0x3040, 0x80); // run M0

	return 0;
}

static int p922x_load_fw(struct oppo_p922x_ic *chip, unsigned char *fw_data, int CodeLength)
{
	unsigned char write_ack = 0;
	int rc = 0;

	rc = p922x_write_reg_multi_byte(chip, 0x400, fw_data,
							((CodeLength + 8 + 15) / 16) * 16);
	if (rc != 0) {
		chg_err("<IDT UPDATE>ERROR: write multi byte data error!\n");
		goto LOAD_ERR;
	}
	rc = __p922x_write_reg(chip, 0x400, 0x01);
	if (rc != 0) {
		chg_err("<IDT UPDATE>ERROR: on OTP buffer validation\n");
		goto LOAD_ERR;
	}

	do {
		msleep(20);
		rc = p922x_read_reg(chip, 0x401, &write_ack, 1);
		if (rc != 0) {
			chg_err("<IDT UPDATE>ERROR: on reading OTP buffer status\n");
			goto LOAD_ERR;
		}
	} while ((write_ack & 0x01) != 0);

	// check status
	if (write_ack != 2) { // not OK
		if (write_ack == 4)
			chg_err("<IDT UPDATE>ERROR: WRITE ERR\n");
		else if (write_ack == 8)
			chg_err("<IDT UPDATE>ERROR: CHECK SUM ERR\n");
		else
			chg_err("<IDT UPDATE>ERROR: UNKNOWN ERR\n");

		rc = -1;
	}
LOAD_ERR:
	return rc;
}

static int p922x_MTP(struct oppo_p922x_ic *chip, unsigned char *fw_buf, int fw_size)
{
	int rc;
	int i, j;
	unsigned char *fw_data;
	unsigned char write_ack;
	unsigned short int StartAddr;
	unsigned short int CheckSum;
	unsigned short int CodeLength;
	// pure fw size not contains last 128 bytes fw version.
	int pure_fw_size = fw_size - 128;

	chg_err("<IDT UPDATE>--1--!\n");

	rc = p922x_load_bootloader(chip);
	if (rc != 0) {
		chg_err("<IDT UPDATE>Update bootloader 1 error!\n");
		return rc;
	}

	msleep(100);

	chg_err("<IDT UPDATE>The idt firmware size: %d!\n", fw_size);

	// program pages of 128 bytes
	// 8-bytes header, 128-bytes data, 8-bytes padding to round to 16-byte boundary
	fw_data = kzalloc(144, GFP_KERNEL);
	if (!fw_data) {
		chg_err("<IDT UPDATE>can't alloc memory!\n");
		return -EINVAL;
	}

	//ERASE FW VERSION(the last 128 byte of the MTP)
	memset(fw_data, 0x00, 144);
	StartAddr = pure_fw_size;
	CheckSum = StartAddr;
	CodeLength = 128;
	for (j = 127; j >= 0; j--)
		CheckSum += fw_data[j + 8]; // add the non zero values.

	CheckSum += CodeLength; // finish calculation of the check sum
	memcpy(fw_data + 2, (char *)&StartAddr, 2);
	memcpy(fw_data + 4, (char *)&CodeLength, 2);
	memcpy(fw_data + 6, (char *)&CheckSum, 2);
	rc = p922x_load_fw(chip, fw_data, CodeLength);
	if (rc < 0) { // not OK
		chg_err("<IDT UPDATE>ERROR: erase fw version ERR\n");
		goto MTP_ERROR;
	}

	// upgrade fw
	memset(fw_data, 0x00, 144);
	for (i = 0; i < pure_fw_size; i += 128) {
		chg_err("<IDT UPDATE>Begin to write chunk %d!\n", i);

		StartAddr = i;
		CheckSum = StartAddr;
		CodeLength = 128;

		memcpy(fw_data + 8, fw_buf + i, 128);

		j = pure_fw_size - i;
		if (j < 128) {
			j = ((j + 15) / 16) * 16;
			CodeLength = (unsigned short int)j;
		} else {
			j = 128;
		}

		j -= 1;
		for (; j >= 0; j--)
			CheckSum += fw_data[j + 8]; // add the non zero values

		CheckSum += CodeLength; // finish calculation of the check sum

		memcpy(fw_data + 2, (char *)&StartAddr, 2);
		memcpy(fw_data + 4, (char *)&CodeLength, 2);
		memcpy(fw_data + 6, (char *)&CheckSum, 2);

		//typedef struct { // write to structure at address 0x400
		// u16 Status;
		// u16 StartAddr;
		// u16 CodeLength;
		// u16 DataChksum;
		// u8 DataBuf[128];
		//} P9220PgmStrType;
		// read status is guaranteed to be != 1 at this point

		rc = p922x_load_fw(chip, fw_data, CodeLength);
		if (rc < 0) { // not OK
			chg_err("<IDT UPDATE>ERROR: write chunk %d ERR\n", i);
			goto MTP_ERROR;
		}
	}

	// Verify
	rc = p922x_load_bootloader(chip);
	if (rc != 0) {
		chg_err("<IDT UPDATE>Update bootloader 2 error!\n");
		return rc;
	}
	msleep(100);
	rc = __p922x_write_reg(chip, 0x402, 0x00); // write start address
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x402 reg error!\n");
		return rc;
	}
	rc = __p922x_write_reg(chip, 0x404, pure_fw_size & 0xff); // write FW length low byte
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x404 reg error!\n");
		return rc;
	}
	rc = __p922x_write_reg(chip, 0x405, (pure_fw_size >> 8) & 0xff); // write FW length high byte
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x405 reg error!\n");
		return rc;
	}

	// write CRC from FW release package
	fw_data[0] = fw_buf[pure_fw_size + 0x08];
	fw_data[1] = fw_buf[pure_fw_size + 0x09];
	p922x_write_reg_multi_byte(chip, 0x406, fw_data, 2);

	rc = __p922x_write_reg(chip, 0x400, 0x11);
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x406 reg error!\n");
		return rc;
	}
	do {
		msleep(20);
		rc = p922x_read_reg(chip, 0x401, &write_ack, 1);
		if (rc != 0) {
			chg_err("<IDT UPDATE>ERROR: on reading OTP buffer status\n");
			goto MTP_ERROR;
		}
	} while ((write_ack & 0x01) != 0);
	// check status
	if (write_ack != 2) { // not OK
		if (write_ack == 4)
			chg_err("<IDT UPDATE>ERROR: CRC WRITE ERR\n");
		else if (write_ack == 8)
			chg_err("<IDT UPDATE>ERROR: CRC CHECK SUM ERR\n");
		else
			chg_err("<IDT UPDATE>ERROR: CRC UNKNOWN ERR\n");

		goto MTP_ERROR;
	}

	//Program FW VERSION(the last 128 byte of the MTP)
	// typedef struct {		// base address: 0x5f80
	//	 // Version		8 Byte
	//	 u16 ChipType;
	//	 u8	CustomerCode;
	//	 u8	empty_a;
	//	 u32 EPRFWRev;
	//	 u16 crc16;
	// } FwSettingType;
	memset(fw_data, 0x00, 144);
	StartAddr = pure_fw_size;
	CheckSum = StartAddr;
	CodeLength = 128;
	memcpy(fw_data + 8, fw_buf + StartAddr, 128);
	j = 127;
	for (; j >= 0; j--)
		CheckSum += fw_data[j + 8]; // add the non zero values.

	CheckSum += CodeLength; // finish calculation of the check sum
	memcpy(fw_data + 2, (char *)&StartAddr, 2);
	memcpy(fw_data + 4, (char *)&CodeLength, 2);
	memcpy(fw_data + 6, (char *)&CheckSum, 2);

	rc = p922x_load_fw(chip, fw_data, CodeLength);
	if (rc < 0) { // not OK
		chg_err("<IDT UPDATE>ERROR: erase fw version ERR\n");
		goto MTP_ERROR;
	}

	// restore system
	rc = __p922x_write_reg(chip, 0x3000, 0x5a); // write key
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3000 reg error!\n");
		goto MTP_ERROR;
	}

	rc = __p922x_write_reg(chip, 0x3048, 0x00); // remove code remapping
	if (rc != 0) {
		chg_err("<IDT UPDATE>Write 0x3048 reg error!\n");
		goto MTP_ERROR;
	}

	chg_err("<IDT UPDATE>OTP Programming finished\n");

	kfree(fw_data);
	return 0;

MTP_ERROR:
	kfree(fw_data);
	return -EINVAL;
}
#endif // NO_FW_UPGRADE_CRC

static int p922x_check_idt_fw_update(struct oppo_p922x_ic *chip)
{
	static int idt_update_retry_cnt = 0;
	int rc = -1;
	char temp[4] = {0, 0, 0, 0};
#ifndef NO_FW_UPGRADE_CRC
	unsigned char *fw_buf;
	int fw_size;
	int fw_ver_start_addr = 0;
#endif

	chg_err("<IDT UPDATE> check idt fw <><><><><><><><>\n");

	if (!chip) {
		chg_err("<IDT UPDATE> p922x isn't ready!\n");
		return rc;
	}

	if (g_oppo_chip->charger_exist == 1) {
		chg_err("<IDT UPDATE>g_oppo_chip->charger_exist == 1, return!\n");
		chip->p922x_chg_status.check_fw_update = true;
		return 0;
	}

	if (p922x_chip->p922x_chg_status.charge_online) {
		chg_err("<SP UPDATE>p922x_chip->p922x_chg_status.charge_online == 1, return!\n");
		chip->p922x_chg_status.check_fw_update = true;
		return 0;
	}

	chip->p922x_chg_status.idt_fw_updating = true;
	mp2650_disable_charging();
	mp2650_set_mps_otg_voltage(false);
	mp2650_set_mps_otg_current();
	mp2650_otg_enable();

	mp2650_wireless_set_mps_otg_en_val(1);
	oppo_set_wrx_en_value(1);
	oppo_set_wrx_otg_value(1);
	oppo_wireless_set_otg_en_val(0);
	msleep(1000);

	rc = p922x_read_reg(chip, 0x001C, temp, 4);
	if (rc) {
		chg_err("<IDT UPDATE>Couldn't read 0x%04x rc = %x\n", 0x001C, rc);
	} else {
		chg_err("<IDT UPDATE>The idt fw version: %02x %02x %02x %02x\n", temp[0], temp[1], temp[2], temp[3]);
#ifdef NO_FW_UPGRADE_CRC
		chg_err("<IDT UPDATE>The new fw version: %02x %02x %02x %02x\n", idt_firmware[0x130C], idt_firmware[0x130D], idt_firmware[0x130E], idt_firmware[0x130F]);

		if ((temp[0] != idt_firmware[0x130C]) || (temp[1] != idt_firmware[0x130D])
			|| (temp[2] != idt_firmware[0x130E]) || (temp[3] != idt_firmware[0x130F])) {
#else
		fw_buf = idt_firmware;
		fw_size = ARRAY_SIZE(idt_firmware);
	
		fw_ver_start_addr = fw_size - 128;
		chg_err("<IDT UPDATE>The new fw version: %02x %02x %02x %02x\n",
				fw_buf[fw_ver_start_addr + 0x04], fw_buf[fw_ver_start_addr + 0x05],
				fw_buf[fw_ver_start_addr + 0x06], fw_buf[fw_ver_start_addr + 0x07]);
			
		if ((temp[0] != fw_buf[fw_ver_start_addr + 0x04]) || (temp[1] != fw_buf[fw_ver_start_addr + 0x05]) 
			|| (temp[2] != fw_buf[fw_ver_start_addr + 0x06]) || (temp[3] != fw_buf[fw_ver_start_addr + 0x07])) {
#endif
			chg_err("<IDT UPDATE>Need update the idt fw!\n");

			chip->p922x_chg_status.charge_current = 0;
			mp2650_charging_current_write_fast(chip->p922x_chg_status.charge_current);
#ifdef NO_FW_UPGRADE_CRC
			if (p922x_MTP(chip) == 0) {
#else
			if (p922x_MTP(chip, fw_buf, fw_size) == 0) {					
#endif
				chip->p922x_chg_status.check_fw_update = true;
			} else {
				idt_update_retry_cnt++;					
				if (idt_update_retry_cnt > 5) {
					chip->p922x_chg_status.check_fw_update = true;
				} else {
					chg_err("<IDT UPDATE>Update faile, Retry %d!\n", idt_update_retry_cnt);
					rc = -1;
				}
			}
		} else {
			chg_err("<IDT UPDATE>No Need update the idt fw!\n");
			chip->p922x_chg_status.check_fw_update = true;
		}
	}

	mp2650_otg_disable();
	mp2650_otg_wait_vbus_decline();

	mp2650_wireless_set_mps_otg_en_val(0);
	oppo_set_wrx_en_value(0);
	oppo_set_wrx_otg_value(0);
	chip->p922x_chg_status.idt_fw_updating = false;

	return rc;
}
#endif
bool p922x_wpc_get_fw_updating(void)
{
	if (!p922x_chip) {
		chg_err("<~WPC~> p922x_chip is NULL\n");
		return false;
	} else {
		return p922x_chip->p922x_chg_status.idt_fw_updating;
	}
}

bool p922x_firmware_is_updating(void)
{
	if (!p922x_chip) {
		return 0;
	}
	
	return p922x_chip->p922x_chg_status.idt_fw_updating;
}

#ifdef WPC_USE_SP6001
int p922x_get_vrect_iout(struct oppo_p922x_ic * chip)
{
	char vrect_value = 0;
	char vout_value = 0;
	char iout_value = 0;
	int i;
	int temp = 0;

	p922x_read_reg(chip, READ_REG_VRECT, &vrect_value, 1);
	chip->p922x_chg_status.vrect = ((int)vrect_value * 27500) >> 8;

	p922x_read_reg(chip, READ_REG_VOUT, &vout_value, 1);
	chip->p922x_chg_status.vout = ((int)vout_value * 22500) >> 8;

	for (i = 1; i < 10; i++) {
		chip->p922x_chg_status.iout_array[i] = chip->p922x_chg_status.iout_array[i - 1];
	}
  
	p922x_read_reg(chip, READ_REG_IOUT, &iout_value, 1);
	chip->p922x_chg_status.iout_array[0] = ((int)iout_value * 2500) >> 8;

	temp = 0;
	for (i = 0; i < 10; i++) {
		temp = temp + chip->p922x_chg_status.iout_array[i];
	}
  
	chip->p922x_chg_status.iout = temp / 10;
  
	chg_err("<~WPC~> Vout:%d,  Vrect:%d,	Iout:%d\n", chip->p922x_chg_status.vout, chip->p922x_chg_status.vrect, chip->p922x_chg_status.iout);  //just debug
	
	return 0;
}
#else
static int p922x_get_vrect_iout(struct oppo_p922x_ic * chip)
{
	char val_buf[5] = {0,0,0,0,0};
	int i;
	int temp = 0;

#if 0
	char debug_data[3];
	
	p922x_read_reg(chip, 0x4E0, debug_data, 2);
	chg_err("<~WPC~>0x4E0 REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);
	
	p922x_read_reg(chip, 0x4E2, debug_data, 2);
	chg_err("<~WPC~>0x4E2 REG: 0x%02X 0x%02X\n", debug_data[0], debug_data[1]);
#endif

	p922x_read_reg(chip, 0x003C, val_buf, 2);
	chip->p922x_chg_status.vout = val_buf[0] | val_buf[1] << 8;
	chip->p922x_chg_status.vout = chip->p922x_chg_status.vout * 21000 / 4095;
	
	p922x_read_reg(chip, 0x0040, val_buf, 2);
	chip->p922x_chg_status.vrect = val_buf[0] | val_buf[1] << 8;
	chip->p922x_chg_status.vrect = chip->p922x_chg_status.vrect * 26250 / 4095;

	for (i = 1; i < 10; i++) {
		chip->p922x_chg_status.iout_array[i] = chip->p922x_chg_status.iout_array[i - 1];
	}
	
	//p922x_read_reg(chip, 0x007a, val_buf, 2);
	//chg_err("tongfeng test 0x007a[0] [0x%x]----0x007a[1] [0x%x]\n", val_buf[0], val_buf[1]);

	p922x_read_reg(chip, 0x0044, val_buf, 2);
	//chg_err("tongfeng test 0x0044[0] [0x%x]----0x0044[1] [0x%x]\n", val_buf[0], val_buf[1]);

	chip->p922x_chg_status.iout_array[0] = val_buf[0] | val_buf[1] << 8;
	temp = 0;
	for (i = 0; i < 10; i++) {
		temp = temp + chip->p922x_chg_status.iout_array[i];
	}

	chip->p922x_chg_status.iout = temp / 10;

	chg_err("<~WPC~> Vout:%d,  Vrect:%d,  Iout:%d\n", chip->p922x_chg_status.vout, chip->p922x_chg_status.vrect, chip->p922x_chg_status.iout);  //just debug

	//p922x_read_debug_registers(chip);

	return 0;
}
#endif

static void p922x_begin_CEP_detect(struct oppo_p922x_ic * chip)
{
	chip->p922x_chg_status.CEP_ready = false;
	schedule_delayed_work(&chip->p922x_CEP_work, P922X_CEP_INTERVAL);
}

static void p922x_reset_CEP_flag(struct oppo_p922x_ic * chip)
{
	chip->p922x_chg_status.CEP_ready = false;
}

static int p922x_get_CEP_flag(struct oppo_p922x_ic * chip)
{
	if (chip->p922x_chg_status.CEP_ready == false) {
		chg_err("<~WPC~> CEP value isn't ready!\n");
		return -1;
	}
	
	//-2 <= CEP <= 2
	if ((chip->p922x_chg_status.CEP_value == 0) || (chip->p922x_chg_status.CEP_value == 1) 
		|| (chip->p922x_chg_status.CEP_value == 2) || (chip->p922x_chg_status.CEP_value == 0xFF)  
		|| (chip->p922x_chg_status.CEP_value == 0xFE)) {
		return 0;
	} else {
		return 1;
	}
}

static void p922x_print_CEP_flag(struct oppo_p922x_ic * chip)
{
	if ((p922x_chip->p922x_chg_status.charge_online) && (chip->p922x_chg_status.CEP_ready)) {
		chg_err("<~WPC~> CEP value = %d\n", chip->p922x_chg_status.CEP_value);
	}
}

static int p922x_detect_CEP(struct oppo_p922x_ic * chip)
{
	int rc = -1;
	char temp = 0;

#ifdef WPC_USE_SP6001
	rc = p922x_read_reg(chip, READ_REG_CEP, &temp, 1);
#else
	rc = p922x_read_reg(chip, 0x0033, &temp, 1);
#endif
	if (rc) {
		chip->p922x_chg_status.CEP_ready = false;
		
		chg_err("Couldn't read CEP rc = %x\n", rc);
		return rc;
	}

	chip->p922x_chg_status.CEP_ready = true;
	chip->p922x_chg_status.CEP_value = temp;
		
	return 0;
}
#ifndef FASTCHG_TEST_BY_TIME
static int p922x_get_cep_val(struct oppo_p922x_ic *chip, signed char *val)
{
	int rc = -1;
	char temp = 0;

#ifdef WPC_USE_SP6001
	rc = p922x_read_reg(chip, READ_REG_CEP, &temp, 1);
#else
	rc = p922x_read_reg(chip, 0x0033, &temp, 1);
#endif
	if (rc) {
		chg_err("Couldn't read CEP rc = %x\n", rc);
		return rc;
	}

	*val = (signed char)temp;
	chg_err("cep = %d\n", *val);

	return rc;
}

static int p922x_get_work_freq(struct oppo_p922x_ic *chip, int *val)
{
	int rc;
	char temp;

	rc = p922x_read_reg(chip, 0x5e, &temp, 1);
	if (rc) {
		chg_err("Couldn't read rx freq val, rc = %d\n", rc);
		return rc;
	}
	*val = (int)temp;
	return rc;
}

#endif
bool p922x_wireless_charge_start(void)
{
	if (!p922x_chip) {
		return 0;
	}
	return p922x_chip->p922x_chg_status.charge_online;
}

void p922x_set_wireless_charge_stop(void)
{
	/*ATTENTION: This function can't be used in idt_connect_int_work, p922x_task_work, idt_event_int_work!*/

	if (!p922x_chip) {
		return;
	}

	if (p922x_chip->p922x_chg_status.charge_online == true) {
		cancel_delayed_work_sync(&p922x_chip->idt_connect_int_work);
		cancel_delayed_work_sync(&p922x_chip->p922x_task_work);
		cancel_delayed_work_sync(&p922x_chip->idt_event_int_work);

        p922x_reset_variables(p922x_chip);
        
		p922x_chip->p922x_chg_status.charge_online = false;
	}
}

void p922x_ready_for_mp2650_charge(struct oppo_p922x_ic *chip)
{
	p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_200MA);
	mp2650_input_current_limit_without_aicl(WPC_CHARGE_CURRENT_LIMIT_300MA);
	mp2650_enable_charging();
    //chargepump_set_for_otg(0);
	msleep(100);	
	chargepump_disable();
	p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_FASTCHG);			
	mp2650_input_current_limit_without_aicl(1200);
	p922x_set_rx_charge_voltage(chip, WPC_CHARGE_VOLTAGE_CHGPUMP_TO_CHARGER);
}

#ifdef DEBUG_FASTCHG_BY_ADB
static int p922x_fastcharge_debug_by_adb(struct oppo_p922x_ic *chip)
{
	int iout_max_value;
	int iout_min_value;
	int temp_value;

	if (!chip->p922x_chg_status.iout_debug_mode && !chip->p922x_chg_status.vout_debug_mode) {
		return -1;
	}

	if (chip->p922x_chg_status.vout_debug_mode) {
		chg_err("<~WPC~> Vout debug mode is running...\n");
			
		return 0;
	}

	chg_err("<~WPC~> Iout debug mode is running...\n");

	if (p922x_get_CEP_flag(chip) != 0) {
		return 0;
	}
	
	iout_max_value = chip->p922x_chg_status.fastchg_current + WPC_CHARGE_CURRENT_OFFSET;
	if (chip->p922x_chg_status.fastchg_current > WPC_CHARGE_CURRENT_OFFSET) {
		iout_min_value = chip->p922x_chg_status.fastchg_current - WPC_CHARGE_CURRENT_OFFSET;
	} else {
		iout_min_value = 0;
	}
	
	if (chip->p922x_chg_status.iout > iout_max_value) {
		chg_err("<~WPC~> The Iout > %d.\n", iout_max_value);
	
		if (chip->p922x_chg_status.charge_voltage > WPC_CHARGE_VOLTAGE_CHGPUMP_MIN) {
			temp_value = chip->p922x_chg_status.iout - iout_max_value;
			if (chip->p922x_chg_status.iout > 2100) {
				p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage - 200);
			} else if (temp_value > 50) {
				p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage - 100);
			} else {
				p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage - 20);
			}
	
			p922x_reset_CEP_flag(chip);
		}
	} else if (chip->p922x_chg_status.iout < iout_min_value) {
		chg_err("<~WPC~> The Iout < %d.\n", iout_min_value);
	
		if (chip->p922x_chg_status.charge_voltage < WPC_CHARGE_VOLTAGE_CHGPUMP_MAX) {
			temp_value = iout_min_value - chip->p922x_chg_status.iout;
			if ((temp_value > 100) && (chip->p922x_chg_status.iout < 1800)) {
				p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage + 100);
			} else if ((temp_value > 50) && (chip->p922x_chg_status.iout < 1800)) {
				p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage + 50);
			} else {
				p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage + 20);
			}
	
			p922x_reset_CEP_flag(chip);
		}
	} else {
		chg_err("<~WPC~> The Iout is OK锛乗n");
	}

	return 0;

}
#endif

#ifdef FASTCHG_TEST_BY_TIME
static void p922x_fastcharge_test_40w(struct oppo_p922x_ic *chip)
{
	int rc;
	unsigned long now_seconds = 0;
	
	chg_err("<~WPC~> ~~~~~~~~40W fastcharg test~~~~~~~~\n"); 

	if (g_oppo_chip->soc > 75) {
		if (enable_test) {
			chg_err("<~WPC~> soc > 75\n");
			chg_err("<~WPC~> stop fastchg\n");
			chip->p922x_chg_status.fastchg_current = 50;
			enable_test = false;
		}
		
		return;
	} else if (g_oppo_chip->soc <= 5) {
		if (!enable_test) {
			chg_err("<~WPC~> soc < 5\n");
			chg_err("<~WPC~> start fastchg\n");

			pre_seconds = 0;
			is_fastchging = false;
			enable_test = true;
		}
	} else {
		if (!enable_test) {
			chg_err("<~WPC~> keep stop fastchg\n");
			return;
		}
	}
	
	rc = get_rtc_time(&now_seconds);
	if (rc < 0) {
		pr_err("Failed to get RTC time, rc=%d\n", rc);
	}
		
	chg_err("<~WPC~> seconds now: %d\n", now_seconds);
	
	if (is_fastchging) {
		if((now_seconds - pre_seconds) > 180) {   //3min
			pre_seconds = now_seconds;
			chg_err("<~WPC~> stop fastchg time: %d\n", pre_seconds);

			chip->p922x_chg_status.fastchg_current = 50;

			is_fastchging = false;
		}
	} else {
		if(pre_seconds == 0) {
			pre_seconds = now_seconds;
			chg_err("<~WPC~> start fastchg time: %d\n", pre_seconds);

			chip->p922x_chg_status.fastchg_current = 2000;

			is_fastchging = true;
		} else {
			if((now_seconds - pre_seconds) > 420) {   //7min
				pre_seconds = now_seconds;
				chg_err("<~WPC~> restart fastchg time: %d\n", pre_seconds);

				chip->p922x_chg_status.fastchg_current = 2000;

				is_fastchging = true;
			}
		}
	}
}

#else
static void p922x_fastcharge_current_adjust_40w(struct oppo_p922x_ic *chip, bool is_init)
{
	static int adjust_current_delay = 0;

	if (is_init) {
		adjust_current_delay = 0;
	}
	
	chg_err("<~WPC~> ~~~~~~~~40W fastcharg current adjust(delay %d)~~~~~~~~\n", adjust_current_delay);

	if(adjust_current_delay > 0) {
		adjust_current_delay--;
		return;
	} 

	if ((g_oppo_chip->soc >= 80) && (chip->p922x_chg_status.fastcharge_level < FASTCHARGE_LEVEL_5)) {
		chg_err("<~WPC~> g_oppo_chip->soc >= 80\n");
		chg_err("<~WPC~> Turn to %dMA charge\n", fasctchg_current[FASTCHARGE_LEVEL_5]);  
		chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_5];
		chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_5;
		adjust_current_delay = WPC_ADJUST_CV_DELAY;
		goto ADJUST_FINISH;
	}

	if (g_oppo_chip->batt_volt >= 4500) {
		chg_err("<~WPC~> batt_volt[%d]\n", g_oppo_chip->batt_volt);
		chip->p922x_chg_status.wpc_reach_4500mv = true;

		if (chip->p922x_chg_status.fastcharge_level < FASTCHARGE_LEVEL_3) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_3]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_3];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_3;
			adjust_current_delay = WPC_ADJUST_CV_DELAY;
			goto ADJUST_FINISH;
		} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_3) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_5]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_5];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_5;
			adjust_current_delay = WPC_ADJUST_CV_DELAY;
			goto ADJUST_FINISH;
		} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_5) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_7]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_7];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_7;
			adjust_current_delay = WPC_ADJUST_CV_DELAY;
			goto ADJUST_FINISH;
		} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_7) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_9]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_9];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_9;
			adjust_current_delay = WPC_ADJUST_CV_DELAY;
			goto ADJUST_FINISH;
		} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_9) {
			chg_err("<~WPC~> batt_volt * 4 + 600: %d\n", g_oppo_chip->batt_volt * 4 + 600);
			p922x_set_rx_charge_voltage(chip, g_oppo_chip->batt_volt * 4 + 600);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_CEP_ZERO;
			return;
		}
	}
	
	if (chip->p922x_chg_status.wpc_reach_4500mv) {
		return;
	}
	
	if (g_oppo_chip->temperature >= 420) {
		chg_err("<~WPC~> The tempearture is >= 42. fastchg current: %d\n", fasctchg_current[FASTCHARGE_LEVEL_3]);

		if (chip->p922x_chg_status.fastcharge_level < FASTCHARGE_LEVEL_3) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_3]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_3];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_3;
			adjust_current_delay = 70;
		} else if(chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_3) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_4]");  
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_4];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_4;
			adjust_current_delay = 70;
		} else if(chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_4) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_5]");  
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_5];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_5;
			adjust_current_delay = 70;
		}else {
			chg_err("<~WPC~> batt_volt * 4 + 600: %d\n", g_oppo_chip->batt_volt * 4 + 600);
			p922x_set_rx_charge_voltage(chip, g_oppo_chip->batt_volt * 4 + 600);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_CEP_ZERO;
			return;
		}
	} else if (g_oppo_chip->temperature >= 390) {
		chg_err("<~WPC~> The tempearture is >= 39. fastcharge_level: %d\n", chip->p922x_chg_status.fastcharge_level);
		if (chip->p922x_chg_status.fastcharge_level > FASTCHARGE_LEVEL_2) {
			////
		} else if (chip->p922x_chg_status.fastcharge_level != FASTCHARGE_LEVEL_2) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_2]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_2];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_2;
			adjust_current_delay = WPC_ADJUST_CV_DELAY;
		}
	} else {
		chg_err("<~WPC~> The tempearture is < 39. fastcharge_level: %d\n", chip->p922x_chg_status.fastcharge_level);
		if (chip->p922x_chg_status.fastcharge_level > FASTCHARGE_LEVEL_1) {
			////
		} else if (chip->p922x_chg_status.fastcharge_level != FASTCHARGE_LEVEL_1) {
			chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_1]"); 
			chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_1];
			chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_1;
			adjust_current_delay = WPC_ADJUST_CV_DELAY;
		}
	}

ADJUST_FINISH:
	if (chip->p922x_chg_status.fastchg_current > chip->p922x_chg_status.fastchg_current_limit) {		
		chg_err("<~WPC~> charge_current[%d] > charge_current_limit[%d]\n", 
				chip->p922x_chg_status.fastchg_current, chip->p922x_chg_status.fastchg_current_limit);
		chip->p922x_chg_status.fastchg_current = chip->p922x_chg_status.fastchg_current_limit;
	}
}
#endif

static void p922x_fastcharge_skewing_proc_40w(struct oppo_p922x_ic *chip, bool is_init)
{
	static int skewing_proc_delay = 0;

	if (is_init) {
		skewing_proc_delay = 0;
		return;
	}
	
	chg_err("<~WPC~> ~~~~~~~~40W fastcharg skewing proc(delay %d)~~~~~~~~\n", skewing_proc_delay);

	if(skewing_proc_delay > 0) {
		skewing_proc_delay--;
		return;
	} 

	if (chip->p922x_chg_status.fastcharge_level >= FASTCHARGE_LEVEL_4) {
		chg_err("<~WPC~> fastchg_current_max = %dmA\n", chip->p922x_chg_status.fastchg_current);	
		chg_err("<~WPC~> Turn to MP2650 control!");
		chip->p922x_chg_status.wpc_ffc_charge = false;
		p922x_ready_for_mp2650_charge(chip);
		chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER;
	} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_1){
		chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_2]"); 
		chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_2];
		chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_2;
		skewing_proc_delay = 70;
	} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_2){
		chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_3]"); 
		chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_3];
		chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_3;
		skewing_proc_delay = 70;
	} else if (chip->p922x_chg_status.fastcharge_level == FASTCHARGE_LEVEL_3){
		chg_err("<~WPC~> Turn to %dMA charge\n, fasctchg_current[FASTCHARGE_LEVEL_4]"); 
		chip->p922x_chg_status.fastchg_current = fasctchg_current[FASTCHARGE_LEVEL_4];
		chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_4;
		skewing_proc_delay = 70;
	}

	if (chip->p922x_chg_status.fastchg_current > chip->p922x_chg_status.fastchg_current_limit) {
		chg_err("<~WPC~> charge_current[%d] > charge_current_limit[%d]\n", 
				chip->p922x_chg_status.fastchg_current, chip->p922x_chg_status.fastchg_current_limit);
		chip->p922x_chg_status.fastchg_current = chip->p922x_chg_status.fastchg_current_limit;
	}
}


//extern void smblib_rerun_apsd_for_wpc(void);

static int p922x_RXTX_message_process(struct oppo_p922x_ic *chip)
{
#ifdef WPC_USE_SP6001
	send_msg_timer++;
	if (send_msg_timer > 2) {
		send_msg_timer = 0;

		if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_UNKNOW) {
			sp6001_send_msg_ask_adapter_type(chip);
		} else if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_FASTCHAGE) {
			sp6001_send_msg_enter_fastchg_mode(chip);
		} else if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_USB_CHARGE) {
			sp6001_send_msg_enter_usb_mode(chip);
		} else if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_NORMAL_CHARGE) {
			sp6001_send_msg_enter_normal_mode(chip);
		} 
	}
#else
	send_msg_timer++;
	if (send_msg_timer > 2) {
		send_msg_timer = 0;

		if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_UNKNOW) {
			p922x_set_tx_charger_dect(chip);
		} else if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_FASTCHAGE) {
			p922x_set_tx_charger_fastcharge(chip);
		} else if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_USB_CHARGE) {
			p922x_set_tx_charger_usb(chip);
		} else if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_NORMAL_CHARGE) {
			p922x_set_tx_charger_normal(chip);
		}
	}
#endif

	switch (chip->p922x_chg_status.tx_command) { 
	case P9237_RESPONE_ADAPTER_TYPE:
		chip->p922x_chg_status.adapter_type = (chip->p922x_chg_status.tx_data & 0x07);
		chip->p922x_chg_status.dock_version = (chip->p922x_chg_status.tx_data & 0xF8) >> 3;
		
		chg_err("<~WPC~> get adapter type = 0x%02X\n", chip->p922x_chg_status.adapter_type);
		chg_err("<~WPC~> get dock hw version = 0x%02X\n", chip->p922x_chg_status.dock_version);

		if ((chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_VOOC)
			|| (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_SVOOC)){
			//chip->p922x_chg_status.fastchg_ing = true;
		} else if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_USB) {
            //mdelay(200);
            //smblib_rerun_apsd_for_wpc();
		}
		break;
		
	case P9237_RESPONE_INTO_FASTCHAGE:
		chip->p922x_chg_status.charge_type = WPC_CHARGE_TYPE_FAST;
		chg_err("<~WPC~> enter charge type = WPC_CHARGE_TYPE_FAST\n");
		if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_FASTCHAGE) {
			chip->p922x_chg_status.send_message = P9221_CMD_NULL;
		}
		break;
		
	case P9237_RESPONE_INTO_USB_CHARGE:
		chip->p922x_chg_status.charge_type = WPC_CHARGE_TYPE_USB;
		chg_err("<~WPC~> enter charge type = WPC_CHARGE_TYPE_USB\n");
		if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_USB_CHARGE) {
			chip->p922x_chg_status.send_message = P9221_CMD_NULL;
		}
		break;
		
	case P9237_RESPONE_INTO_NORMAL_CHARGER:
		chip->p922x_chg_status.charge_type = WPC_CHARGE_TYPE_NORMAL;
		chg_err("<~WPC~> enter charge type = WPC_CHARGE_TYPE_NORMAL\n");
		if (chip->p922x_chg_status.send_message == P9221_CMD_INTO_NORMAL_CHARGE) {
			chip->p922x_chg_status.send_message = P9221_CMD_NULL;
		}
		break;

	case P9237_RESPONE_READY_FOR_EPP:
		chip->p922x_chg_status.adapter_type = ADAPTER_TYPE_EPP;
		chg_err("<~WPC~> adapter_type = ADAPTER_TYPE_EPP\n");
		break;

	case P9237_RESPONE_WORKING_IN_EPP:
		chip->p922x_chg_status.epp_working = true;		
		chg_err("<~WPC~> chip->p922x_chg_status.epp_working = true\n");
		break;
		
	default:
		break;
	}

	chip->p922x_chg_status.tx_command = P9237_RESPONE_NULL;
	return 0;
}

static int p922x_temperature_and_terminate_process(struct oppo_p922x_ic *chip)
{
	if ((g_oppo_chip->temperature >= WPC_CHARGE_FFC_TEMP_MIN) 
		&& (g_oppo_chip->temperature < WPC_CHARGE_FFC_TEMP_MED)) {
		if (chip->p922x_chg_status.temperature_status != TEMPERATURE_STATUS_FFC_1) {
			chg_err("<~WPC~> temperature_status change to TEMPERATURE_STATUS_FFC_1\n");
			chip->p922x_chg_status.temperature_status = TEMPERATURE_STATUS_FFC_1;
			p922x_set_rx_terminate_voltage(chip, WPC_TERMINATION_VOLTAGE_FFC1);
			p922x_set_rx_terminate_current(chip, WPC_TERMINATION_CURRENT_FFC1);

		} 
	} else if ((g_oppo_chip->temperature >= WPC_CHARGE_FFC_TEMP_MED) 
				&& (g_oppo_chip->temperature < WPC_CHARGE_FFC_TEMP_MAX)) {
		if (chip->p922x_chg_status.temperature_status != TEMPERATURE_STATUS_FFC_2) {
			chg_err("<~WPC~> temperature_status change to TEMPERATURE_STATUS_FFC_2\n");
			chip->p922x_chg_status.temperature_status = TEMPERATURE_STATUS_FFC_2;
			p922x_set_rx_terminate_voltage(chip, WPC_TERMINATION_VOLTAGE_FFC2);
			p922x_set_rx_terminate_current(chip, WPC_TERMINATION_CURRENT_FFC2);
		}  
	} else {
		if (chip->p922x_chg_status.temperature_status != TEMPERATURE_STATUS_OTHER) {
			chg_err("<~WPC~> temperature_status change to TEMPERATURE_STATUS_OTHER\n");
			chip->p922x_chg_status.temperature_status = TEMPERATURE_STATUS_OTHER;
			p922x_set_rx_terminate_voltage(chip, WPC_TERMINATION_VOLTAGE_OTHER);
			p922x_set_rx_terminate_current(chip, WPC_TERMINATION_CURRENT_OTHER);
		} 
	}

	return 0;
}

static int p922x_charge_status_process(struct oppo_p922x_ic *chip)
{
	static int work_delay = 0;
	static int batt_vol_4450_cnt = 0;
	static int cep_nonzero_cnt = 0;
	static int cep_zero_cnt = 0;
	static int target_voltage;
	int temp_value = 0;
	int iout_max_value;
	int iout_min_value;
	static bool flag_temperature_abnormal = false;
#ifndef FASTCHG_TEST_BY_TIME
	int work_freq = 0;
	int freq_thr = 0;
	int ret = 0;
	signed char cep_val = 0;
#endif

#ifndef FASTCHG_TEST_BY_TIME
	if ((g_oppo_chip->temperature < WPC_CHARGE_TEMP_MIN) || (g_oppo_chip->temperature > WPC_CHARGE_TEMP_MAX)) {
		chg_err("<~WPC~> The temperature is abnormal, stop charge!\n");
		if (chip->p922x_chg_status.charge_current != WPC_CHARGE_CURRENT_ZERO) {
			p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_ZERO);
		}

		if (chip->p922x_chg_status.charge_voltage != WPC_CHARGE_VOLTAGE_DEFAULT) {
			p922x_set_rx_charge_voltage(chip, WPC_CHARGE_VOLTAGE_DEFAULT);
		}

		return 0;
	}
#endif

	p922x_print_CEP_flag(chip);

	if (work_delay > 0) {
		work_delay--;
		return 0;
	}

	switch (chip->p922x_chg_status.charge_status) {
	case WPC_CHG_STATUS_DEFAULT:
		if (chip->p922x_chg_status.rx_runing_mode == RX_RUNNING_MODE_EPP) {
			chg_err("<~WPC~> Change to EPP charge\n");  
			chargepump_disable();
			mp2650_set_vindpm_vol(8700);
			mp2650_disable_buck_switch();
			mp2650_enable_charging();

			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_EPP;
			break;
		}

		if (chip->p922x_chg_status.charge_current != WPC_CHARGE_CURRENT_DEFAULT) {
			p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_DEFAULT);
		}

		if (chip->p922x_chg_status.charge_voltage != WPC_CHARGE_VOLTAGE_DEFAULT) {
			p922x_set_rx_charge_voltage(chip, WPC_CHARGE_VOLTAGE_DEFAULT);
		}

#ifdef FASTCHG_TEST_BY_TIME
		if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_VOOC) {
			chip->p922x_chg_status.fastchg_current_limit = 2000;
			chargepump_set_for_EPP();
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_VOOC;
			break;
		} else if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_SVOOC) {
			chip->p922x_chg_status.fastchg_current_limit = 2000;
			chargepump_set_for_EPP();
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_SVOOC;
			break;
		} else if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_EPP) {
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_EPP;
			break;
		}
#else
		/*deviation check*/
		if ((chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_VOOC
				|| chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_SVOOC)
				&& !chip->p922x_chg_status.deviation_check_done) {
			if (g_oppo_chip->batt_volt_max > 4100) {
				freq_thr = chip->p922x_chg_status.freq_threshold + 2;
			} else if (g_oppo_chip->batt_volt_max > 3700) {
				freq_thr = chip->p922x_chg_status.freq_threshold + (g_oppo_chip->batt_volt_max - 3700) / 200;
			} else {
				freq_thr = chip->p922x_chg_status.freq_threshold;
			}
			ret = p922x_get_cep_val(chip, &cep_val);
			if (ret != 0) {
				chg_err("<~WPC~> p922x_get_cep_val error, return\n");
				return ret;
			}
			if (abs(cep_val) < 6) {
				ret = p922x_get_work_freq(chip, &work_freq);
				if (ret != 0) {
					chg_err("<~WPC~> p922x_get_work_freq error, return\n");
					return ret;
				}

				if (chip->p922x_chg_status.freq_check_count < 5) {
					chg_err("<~WPC~> work_freq=%d, freq_thr=%d\n", work_freq, freq_thr);
					if (work_freq > freq_thr)
						goto freq_check_done;
					/*
					 * When the frequency still cannot exceed the threshold
					 * value and is close to the threshold value after 5
					 * consecutive detections, the subsequent threshold
					 * value needs to be appropriately increased. Because
					 * when the mobile phone is moved, the frequency change
					 * will be greater than the frequency fluctuation, which
					 * can optimize the misrecognition caused by the frequency
					 * fluctuation.
					 */
					chip->p922x_chg_status.freq_check_count++;
					if ((chip->p922x_chg_status.freq_check_count >= 5)
							&& (freq_thr - work_freq < 2)) {
						chg_err("<~WPC~> Increase the frequency threshold\n");
						chip->p922x_chg_status.freq_thr_inc = true;
					}
					break;
				}
				if (chip->p922x_chg_status.freq_thr_inc)
					freq_thr += 2;

				if (work_freq > freq_thr) {
freq_check_done:
					chip->p922x_chg_status.deviation_check_done = true;
					chip->p922x_chg_status.is_deviation = false;
					chg_err("<~WPC~> deviation_check_done, phone location is correct\n");
				} else {
					chip->p922x_chg_status.is_deviation = true;
					chg_err("<~WPC~> phone location is deviation, work_freq=%d, freq_thr=%d\n", work_freq, freq_thr);
					break;
				}
			} else {
				chg_err("<~WPC~> cep is error, can't start work freq check\n");
				break;
			}
		} else {
			chg_err("<~WPC~> phone location is correct\n");
		}

		if((g_oppo_chip->temperature >= WPC_FASTCHG_TEMP_MIN) && (g_oppo_chip->temperature <= WPC_FASTCHG_TEMP_MAX)) {
			if ((g_oppo_chip->batt_volt_min > P922X_PRE2CC_CHG_THD_LO)
				&& (g_oppo_chip->batt_volt_max < P922X_CC2CV_CHG_THD_HI)
				&& (g_oppo_chip->soc <= 90)) {
				if (g_oppo_chip->soc >= 50) {
					chip->p922x_chg_status.fastchg_current_limit = 1000;
				} else {
					chip->p922x_chg_status.fastchg_current_limit = 2000;
				}
				
				if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_VOOC) {                    
					chargepump_set_for_EPP();
					chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_VOOC;
					break;
				} else if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_SVOOC) {
					chargepump_set_for_EPP();
					chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_SVOOC;
					break;
				} else if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_EPP) {
					chargepump_disable();
					mp2650_set_vindpm_vol(8700);
					mp2650_disable_buck_switch();
					mp2650_enable_charging();
					
					chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_EPP;
					break;
				}
			}
		}
#endif
		break;

	case WPC_CHG_STATUS_READY_FOR_EPP:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_READY_FOR_EPP..........\n");		
		p922x_set_rx_charge_current(chip, 1200);
		mp2650_input_current_limit_without_aicl(1100);
		chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_EPP_1A;
		break;

	case WPC_CHG_STATUS_EPP_1A:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_EPP_1A..........\n");
#ifndef WPC_USE_SP6001
		chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_EPP_WORKING;
#else
		if (chip->p922x_chg_status.epp_working) {
			p922x_set_rx_charge_current(chip, 1200);
			mp2650_input_current_limit_without_aicl(1200);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_EPP_WORKING;
		}
#endif
		break;
		
	case WPC_CHG_STATUS_EPP_WORKING:
		break;

	case WPC_CHG_STATUS_READY_FOR_VOOC:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_READY_FOR_VOOC..........\n");
		mp2650_enable_charging();
		p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_FASTCHG_INT);
		p922x_set_rx_charge_voltage(chip, WPC_CHARGE_VOLTAGE_VOOC_INIT);
		mp2650_input_current_limit_without_aicl(WPC_CHARGE_CURRENT_LIMIT_300MA);
		chip->p922x_chg_status.send_message = P9221_CMD_INTO_FASTCHAGE;
		p922x_set_tx_charger_fastcharge(chip);
		p922x_set_cp_ldo_5v_val(1);
		send_msg_timer = 0;
		p922x_begin_CEP_detect(chip);

		chip->p922x_chg_status.fastchg_ing = true;
		chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_FOR_TX_INTO_FASTCHG;
		break;

	case WPC_CHG_STATUS_READY_FOR_SVOOC:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_READY_FOR_SVOOC..........\n");
		mp2650_enable_charging();
		p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_FASTCHG_INT);
		p922x_set_rx_charge_voltage(chip, WPC_CHARGE_VOLTAGE_SVOOC_INIT);
		mp2650_input_current_limit_without_aicl(WPC_CHARGE_CURRENT_LIMIT_300MA);
		chip->p922x_chg_status.send_message = P9221_CMD_INTO_FASTCHAGE;
		p922x_set_tx_charger_fastcharge(chip);
		p922x_set_cp_ldo_5v_val(1);

		send_msg_timer = 0;
		p922x_begin_CEP_detect(chip);
		chip->p922x_chg_status.fastchg_ing = true;
		chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_FOR_TX_INTO_FASTCHG;
		break;

	case WPC_CHG_STATUS_WAITING_FOR_TX_INTO_FASTCHG:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_WAITING_FOR_TX_INTO_FASTCHG..........\n");
		if ((p922x_get_CEP_flag(chip) == 0) && (chip->p922x_chg_status.charge_type == WPC_CHARGE_TYPE_FAST)) {
			if (chip->p922x_chg_status.ftm_mode) {
				chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_READY_FOR_FTM;
			} else {
				if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_VOOC) {
					target_voltage = WPC_CHARGE_VOLTAGE_FASTCHG_MAX;
				} else {
					target_voltage = g_oppo_chip->batt_volt * 4 + 300;
				}

				chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_INCREASE_VOLTAGE;
			}
		} 
		break;

	case WPC_CHG_STATUS_INCREASE_VOLTAGE:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_INCREASE_VOLTAGE..........\n");
		if (p922x_get_CEP_flag(chip) == 0) {
			chg_err("<~WPC~> charge_voltage[%d], target_voltage[%d]\n", chip->p922x_chg_status.charge_voltage, target_voltage);
			temp_value = p922x_increase_vc_by_step(chip, 
													chip->p922x_chg_status.charge_voltage, 
													target_voltage,
													WPC_CHARGE_VOLTAGE_CHANGE_STEP_1V);
			if (temp_value != 0) {
				p922x_set_rx_charge_voltage(chip, temp_value);
				p922x_reset_CEP_flag(chip);
			} else {
				if (chip->p922x_chg_status.adapter_type == ADAPTER_TYPE_FASTCHAGE_VOOC) {
					chg_err("<~WPC~> Go to mp2650 charge \n");
					flag_temperature_abnormal = false;
					cep_zero_cnt = 0;
					cep_nonzero_cnt = 0;
					chip->p922x_chg_status.wpc_ffc_charge = true;
					chip->p922x_chg_status.has_reach_max_temperature = false;

					p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_FASTCHG);
					mp2650_input_current_limit_without_aicl(1200);
					chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER;
				} else {
					chargepump_enable();
					chargepump_set_for_LDO();
					chargepump_enable_watchdog();

					flag_temperature_abnormal = false;
					batt_vol_4450_cnt = 0;
					cep_zero_cnt = 0;
					cep_nonzero_cnt = 0;
#ifdef FASTCHG_TEST_BY_TIME
					is_fastchging = false;
					pre_seconds = 0;
					enable_test = true;
#endif
					chip->p922x_chg_status.wpc_ffc_charge = true;
					chip->p922x_chg_status.has_reach_max_temperature = false;
					chip->p922x_chg_status.fastcharge_level = FASTCHARGE_LEVEL_UNKNOW;
#ifndef FASTCHG_TEST_BY_TIME
					p922x_fastcharge_current_adjust_40w(chip, true);
#endif
					p922x_fastcharge_skewing_proc_40w(chip, true);
					chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FAST_CHARGING_FROM_CHGPUMP;
				}
			}
		}
		break;

	case WPC_CHG_STATUS_FAST_CHARGING_FROM_CHGPUMP:
		chargepump_kick_watchdog();
		
#ifdef DEBUG_FASTCHG_BY_ADB
		if (p922x_fastcharge_debug_by_adb(chip) == 0) {
			break;
		}
#endif

#ifdef FASTCHG_TEST_BY_TIME
		p922x_fastcharge_test_40w(chip);
#else		
		if ((g_oppo_chip->batt_volt >= 4500) && (!chip->p922x_chg_status.wpc_ffc_charge)) {
			chg_err("<~WPC~> batt_volt[%d] >= 4.5V, Not in FFC\n", g_oppo_chip->batt_volt);
			chg_err("<~WPC~> Turn to MP2650 control!");
			
			cep_nonzero_cnt = 0;
			p922x_set_rx_charge_voltage(chip, g_oppo_chip->batt_volt * 4 + 600);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_CEP_ZERO;
			break;
		} else if (chip->p922x_chg_status.wpc_ffc_charge && (g_oppo_chip->batt_volt >= 4500) 
					&& ((g_oppo_chip->icharging < 0) && ((-1 * g_oppo_chip->icharging) < 900))) {
			chg_err("<~WPC~> batt_volt[%d] >= 4.5V, batt_cur[%d] < 900ma, In FFC\n", g_oppo_chip->batt_volt, (-1 * g_oppo_chip->icharging));
			chg_err("<~WPC~> Turn to MP2650 control!");
			
			cep_nonzero_cnt = 0;
			p922x_set_rx_charge_voltage(chip, g_oppo_chip->batt_volt * 4 + 600);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_CEP_ZERO;
			break;
		} else if (!chip->p922x_chg_status.wpc_ffc_charge && g_oppo_chip->batt_volt >= 4370 ) {

			chg_err("<~WPC~> batt_volt[%d], Not in FFC\n", g_oppo_chip->batt_volt);
			chg_err("<~WPC~> Turn to MP2650 control!");

			cep_nonzero_cnt = 0;
			p922x_set_rx_charge_voltage(chip, g_oppo_chip->batt_volt * 4 + 600);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_CEP_ZERO;

			break;
		} else if (((g_oppo_chip->temperature < WPC_CHARGE_FFC_TEMP_MIN) || (g_oppo_chip->temperature > WPC_CHARGE_FFC_TEMP_MAX)) && (g_oppo_chip->batt_volt > 4370)) {

			chg_err("<~WPC~> batt_volt[%d], temperature[%d]\n", g_oppo_chip->batt_volt, g_oppo_chip->temperature);
			chg_err("<~WPC~> Turn to MP2650 control!");

			cep_nonzero_cnt = 0;			
			chip->p922x_chg_status.wpc_ffc_charge = false;
			p922x_set_rx_charge_voltage(chip, g_oppo_chip->batt_volt * 4 + 600);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_WAITING_CEP_ZERO;

			break;
		}

		if (chip->p922x_chg_status.wpc_reach_stable_charge) {
			if(!chip->p922x_chg_status.wpc_ffc_charge) {
				if (!chip->p922x_chg_status.wpc_reach_4370mv && (g_oppo_chip->batt_volt >= 4370)) {
					chip->p922x_chg_status.wpc_reach_4370mv = true;
					if ((g_oppo_chip->icharging < 0) && ((-1 * g_oppo_chip->icharging) < WPC_CHARGE_CURRENT_FFC_TO_CV)) {
						chg_err("<~WPC~> batt_volt >= 4370, icharging < 1A\n");
						chip->p922x_chg_status.wpc_ffc_charge = false;
					} else {
						chip->p922x_chg_status.wpc_ffc_charge = true;
					}
				}
			}
		}
		
		if ((p922x_get_CEP_flag(chip) == 0) && (chip->p922x_chg_status.wpc_skewing_proc == false)) {
			cep_nonzero_cnt = 0; 

			chg_err("<~WPC~> ^-^ chargepump fastcharge ^-^\n");
			p922x_fastcharge_current_adjust_40w(chip, false);
		} else {
			if (p922x_get_CEP_flag(chip) == 0) {
				cep_nonzero_cnt = 0;
				cep_zero_cnt++;
				if (cep_zero_cnt >= 10) {
					cep_zero_cnt = 0;
					chip->p922x_chg_status.wpc_skewing_proc = false;
					chg_err("<~WPC~>  wpc_skewing_proc = false\n");
				}
				
			} else {
				cep_zero_cnt = 0;
				cep_nonzero_cnt++;
				if (cep_nonzero_cnt >= 3) {
					cep_nonzero_cnt = 0;
					chip->p922x_chg_status.wpc_skewing_proc = true;
					chg_err("<~WPC~>  wpc_skewing_proc = true\n");
				} 
			}

			if (chip->p922x_chg_status.wpc_skewing_proc == true) {
				p922x_fastcharge_skewing_proc_40w(chip, false);
			}
		}
#endif

		iout_max_value = chip->p922x_chg_status.fastchg_current + WPC_CHARGE_CURRENT_OFFSET;
		if (chip->p922x_chg_status.fastchg_current > WPC_CHARGE_CURRENT_OFFSET) {
			iout_min_value = chip->p922x_chg_status.fastchg_current - WPC_CHARGE_CURRENT_OFFSET;
		} else {
			iout_min_value = 0;
		}
		
		if (chip->p922x_chg_status.iout > iout_max_value) {
			chg_err("<~WPC~> The Iout > %d.\n", iout_max_value);
		
			if (chip->p922x_chg_status.charge_voltage > WPC_CHARGE_VOLTAGE_CHGPUMP_MIN) {
				temp_value = chip->p922x_chg_status.iout - iout_max_value;
				if (chip->p922x_chg_status.iout > 2100) {
					p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage - 200);
				} else if (temp_value > 50) {
					p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage - 100);
				} else {
					p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage - 20);
				}
		
				p922x_reset_CEP_flag(chip);
				work_delay = 0; 
				break;
			}
		} else if (chip->p922x_chg_status.iout < iout_min_value) {
			chg_err("<~WPC~> The Iout < %d.\n", iout_min_value);
		
			if (chip->p922x_chg_status.charge_voltage < WPC_CHARGE_VOLTAGE_CHGPUMP_MAX) {
				temp_value = iout_min_value - chip->p922x_chg_status.iout;
				if ((temp_value > 100) && (chip->p922x_chg_status.iout < 1800)) {
					p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage + 100);
				} else if ((temp_value > 50) && (chip->p922x_chg_status.iout < 1800)) {
					p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage + 50);
				} else {
					p922x_set_rx_charge_voltage(chip, chip->p922x_chg_status.charge_voltage + 20);
				}
		
				p922x_reset_CEP_flag(chip);
		
				if (chip->p922x_chg_status.iout > 1500) {
					work_delay = 3;
				} else {
					work_delay = 0;
				}
				break;
			}
		} else {
			if (!chip->p922x_chg_status.wpc_reach_stable_charge) {
				chg_err("<~WPC~> set wpc_reach_stable_charge = true\n");
				chip->p922x_chg_status.wpc_reach_stable_charge = true;

				if (g_oppo_chip->batt_volt >= 4370) {
					chg_err("<~WPC~> batt_volt >= 4370, wpc_ffc_charge = false\n");
					chip->p922x_chg_status.wpc_reach_4370mv = true;
					chip->p922x_chg_status.wpc_ffc_charge = false;
				} 
			}
		}
		break;

	case WPC_CHG_STATUS_WAITING_CEP_ZERO:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_WAITING_CEP_ZERO..........\n");
		
		if (p922x_get_CEP_flag(chip) == 0) {
			chg_err("<~WPC~> Turn to mp2650 charge \n");

			p922x_ready_for_mp2650_charge(chip);
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER;
		}
	    break;
	    
	case WPC_CHG_STATUS_INCREASE_CURRENT:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_INCREASE_CURRENT..........\n");
		temp_value = p922x_increase_vc_by_step(chip, 
												chip->p922x_chg_status.charge_current, 
												chip->p922x_chg_status.cc_value_limit, 
												WPC_CHARGE_CURRENT_CHANGE_STEP_200MA);
		if (temp_value != 0) {
			p922x_set_rx_charge_current(chip, temp_value);
			work_delay = WPC_INCREASE_CURRENT_DELAY; 
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_ADJUST_VOL_AFTER_INC_CURRENT;
		} else {
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER;
		}
		break;

	case WPC_CHG_STATUS_ADJUST_VOL_AFTER_INC_CURRENT:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_ADJUST_VOL_AFTER_INC_CURRENT..........\n");
		if (chip->p922x_chg_status.iout > WPC_CHARGE_IOUT_HIGH_LEVEL) {
			chg_err("<~WPC~>  IDT Iout > 1050mA!\n");		
			temp_value = p922x_increase_vc_by_step(chip, 
													chip->p922x_chg_status.charge_voltage, 
													WPC_CHARGE_VOLTAGE_FASTCHG_MAX, 
													WPC_CHARGE_VOLTAGE_CHANGE_STEP_1V);
			if (temp_value != 0) {
				p922x_set_rx_charge_voltage(chip, temp_value);
			} else {					
				chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER;
			}
		} else {
			chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_INCREASE_CURRENT;
		}
		break;

	case WPC_CHG_STATUS_FAST_CHARGING_FROM_CHARGER:
		if (g_oppo_chip->temperature >= 425) {
			chg_err("<~WPC~> The tempearture is >= 42.5\n");
			flag_temperature_abnormal = true;
			
			temp_value = p922x_decrease_vc_by_step(chip, 
													chip->p922x_chg_status.charge_current, 
													WPC_CHARGE_CURRENT_DEFAULT, 
													WPC_CHARGE_CURRENT_CHANGE_STEP_200MA);
			if (temp_value != 0) {
				p922x_set_rx_charge_current(chip, temp_value);

				work_delay = WPC_ADJUST_CV_DELAY;
				break;
			}
		} else if (g_oppo_chip->temperature >= 410) {
			chg_err("<~WPC~> The tempearture is >= 41\n");
			if (chip->p922x_chg_status.charge_current > 1200) {
				p922x_set_rx_charge_current(chip, 1200);
				work_delay = WPC_ADJUST_CV_DELAY;
				break;
			}
		} else if (g_oppo_chip->temperature < 370) {
			if (flag_temperature_abnormal) {
				flag_temperature_abnormal = false;

				if (chip->p922x_chg_status.charge_current < WPC_CHARGE_CURRENT_FASTCHG) {
					chg_err("<~WPC~> The tempearture is < 37. Icrease current.\n");
					chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_INCREASE_CURRENT;
					break;
				}
			}
		} 

		if ((chip->p922x_chg_status.charge_current <= WPC_CHARGE_CURRENT_DEFAULT) 
			&& (chip->p922x_chg_status.charge_voltage > WPC_CHARGE_VOLTAGE_FASTCHG)) {
			chg_err("<~WPC~> The charge current <= 500mA. The charge voltage turn to 12V\n");
			p922x_set_rx_charge_voltage(chip, WPC_CHARGE_VOLTAGE_FASTCHG);
			work_delay = WPC_ADJUST_CV_DELAY; 
			break;
		}

		if (g_oppo_chip->temperature < 410) {
			if(p922x_get_CEP_flag(chip) == 0) {
				cep_nonzero_cnt = 0;
				
				if (chip->p922x_chg_status.iout > WPC_CHARGE_1A_UPPER_LIMIT) {
					chg_err("<~WPC~> The Iout > 1050mA.\n");

					if (chip->p922x_chg_status.charge_voltage >= WPC_CHARGE_VOLTAGE_FASTCHG_MAX) {
						temp_value = p922x_decrease_vc_by_step(chip, 
																chip->p922x_chg_status.charge_current, 
																WPC_CHARGE_CURRENT_DEFAULT, 
																WPC_CHARGE_CURRENT_CHANGE_STEP_50MA);
						if (temp_value != 0) {
							p922x_set_rx_charge_current(chip, temp_value);
							p922x_reset_CEP_flag(chip);
							work_delay = WPC_ADJUST_CV_DELAY; 
							break;
						}
					} else {
						temp_value = p922x_increase_vc_by_step(chip, 
																chip->p922x_chg_status.charge_voltage, 
																WPC_CHARGE_VOLTAGE_FASTCHG_MAX, 
																WPC_CHARGE_VOLTAGE_CHANGE_STEP_20MV);
						if (temp_value != 0) {
							p922x_set_rx_charge_voltage(chip, temp_value);
							p922x_reset_CEP_flag(chip);
							work_delay = WPC_ADJUST_CV_DELAY;
							break;
						}
					}
				} else if (chip->p922x_chg_status.iout < WPC_CHARGE_1A_LOWER_LIMIT) {
					chg_err("<~WPC~> The Iout < 950mA.\n");

					if (chip->p922x_chg_status.charge_voltage > WPC_CHARGE_VOLTAGE_FASTCHG) {
						temp_value = p922x_decrease_vc_by_step(chip, 
																chip->p922x_chg_status.charge_voltage, 
																WPC_CHARGE_VOLTAGE_FASTCHG, 
																WPC_CHARGE_VOLTAGE_CHANGE_STEP_20MV);
						if (temp_value != 0) {
							p922x_set_rx_charge_voltage(chip, temp_value);
							p922x_reset_CEP_flag(chip);
							work_delay = WPC_ADJUST_CV_DELAY; 
							break;
						}
					} else {
						temp_value = p922x_increase_vc_by_step(chip, 
																chip->p922x_chg_status.charge_current, 
																chip->p922x_chg_status.cc_value_limit, 
																WPC_CHARGE_CURRENT_CHANGE_STEP_50MA);
						if (temp_value != 0) {
							p922x_set_rx_charge_current(chip, temp_value);
							p922x_reset_CEP_flag(chip);
							work_delay = WPC_ADJUST_CV_DELAY;
							break;
						}
					}
				} else {

				}
				
				if (chip->p922x_chg_status.wpc_skewing_proc == true) {
					cep_zero_cnt++;
					if (cep_zero_cnt > 10) {
						chg_err("<~WPC~> CEP zero > 10. Turn to chargepump fastcharge\n");
						chip->p922x_chg_status.wpc_skewing_proc = false;
						target_voltage = g_oppo_chip->batt_volt * 4 + 300;
						chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_INCREASE_VOLTAGE;
						break;
					}
				}
			}
			else if(p922x_get_CEP_flag(chip) == 1) {
				cep_zero_cnt = 0;
				cep_nonzero_cnt++;
				if(cep_nonzero_cnt >= 3) {
					chg_err("<~WPC~> CEP nonzero >= 3.\n");

					if (chip->p922x_chg_status.charge_current > WPC_CHARGE_CURRENT_FASTCHG_MID) {
						temp_value = p922x_decrease_vc_by_step(chip, 
																chip->p922x_chg_status.charge_current, 
																WPC_CHARGE_CURRENT_FASTCHG_MID, 
																WPC_CHARGE_CURRENT_CHANGE_STEP_50MA);
						if (temp_value != 0) {
							p922x_set_rx_charge_current(chip, temp_value);
							//work_delay = WPC_ADJUST_CV_DELAY; 
							break;
						}
					} else if (chip->p922x_chg_status.charge_voltage > WPC_CHARGE_VOLTAGE_FASTCHG) {
						temp_value = p922x_decrease_vc_by_step(chip, 
																chip->p922x_chg_status.charge_voltage, 
																WPC_CHARGE_VOLTAGE_FASTCHG, 
																WPC_CHARGE_VOLTAGE_CHANGE_STEP_20MV);
						if (temp_value != 0) {
							p922x_set_rx_charge_voltage(chip, temp_value);
							//work_delay = WPC_ADJUST_CV_DELAY; 
							break;
						}
					} else {
						p922x_set_rx_charge_current(chip, WPC_CHARGE_CURRENT_DEFAULT);
						//work_delay = WPC_ADJUST_CV_DELAY; 
						break;
					}

					cep_nonzero_cnt = 0;
				}
			}
		}

		if (g_oppo_chip->batt_volt >= 4500) {
			batt_vol_4450_cnt++;
			if (batt_vol_4450_cnt < 5) {
				break;
			}

			chg_err("<~WPC~> The max cell vol >= 4500mV.\n");
			
			batt_vol_4450_cnt = 0;

			temp_value = p922x_decrease_vc_by_step(chip, 
													chip->p922x_chg_status.charge_current, 
													WPC_CHARGE_CURRENT_FASTCHG_END, 
													WPC_CHARGE_CURRENT_CHANGE_STEP_200MA);
			if (temp_value != 0) {
				chip->p922x_chg_status.cc_value_limit = temp_value;
				p922x_set_rx_charge_current(chip, temp_value);
			
				work_delay = WPC_ADJUST_CV_DELAY;
				break;
			}
		} else {
			batt_vol_4450_cnt = 0;
		}
		
		break;

	case WPC_CHG_STATUS_READY_FOR_FTM:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_READY_FOR_FTM..........\n");
		if (p922x_get_CEP_flag(chip) == 0) {
			temp_value = p922x_increase_vc_by_step(chip, 
													chip->p922x_chg_status.charge_voltage, 
													WPC_CHARGE_VOLTAGE_FTM, 
													WPC_CHARGE_VOLTAGE_CHANGE_STEP_1V);
			if (temp_value != 0) {
				p922x_set_rx_charge_voltage(chip, temp_value);
				p922x_reset_CEP_flag(chip);
			} else {
				mp2650_disable_charging();
				chargepump_enable();
				chargepump_set_for_LDO();
				chargepump_enable_watchdog();
				
				chip->p922x_chg_status.charge_status = WPC_CHG_STATUS_FTM_WORKING;
			}
		}	
		break;
		
	case WPC_CHG_STATUS_FTM_WORKING:
		chg_err("<~WPC~> ..........WPC_CHG_STATUS_FTM_WORKING..........\n");
		chargepump_kick_watchdog();
		break;
		
	default:
		break;
	}
	
	return 0;
}

#ifdef WPC_USE_SP6001
static void p922x_commu_data_process(struct oppo_p922x_ic *chip)
{
	char temp[2] = {0, 0};
	char val_buf[5] = {0,0,0,0,0};

	chg_err("<linshangbo> RX message int@@@@@@@@@@@@@@@\n");

	p922x_read_reg(chip, READ_REG_00, &val_buf[0], 1);
	p922x_read_reg(chip, READ_REG_01, &val_buf[1], 1);
	p922x_read_reg(chip, READ_REG_02, &val_buf[2], 1);
	p922x_read_reg(chip, READ_REG_03, &val_buf[3], 1);
	p922x_read_reg(chip, READ_REG_04, &val_buf[4], 1);
	
	p922x_write_reg(chip, WRITE_REG_TX, 0xAA); 

	chg_err("<linshangbo> 1st RX Data: 0x%02X  0x%02X  0x%02X  0x%02X  0x%02X \n",
			val_buf[0], val_buf[1], val_buf[2], val_buf[3], val_buf[4]);
	if (val_buf[0] == 0x13) {
		chip->p922x_chg_status.tx_command = P9237_RESPONE_READY_FOR_EPP;
		chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
		chg_err("<~WPC~> Received TX command: READY FOR EPP!\n");
		chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
		return;
	} else if (val_buf[0] == 0x14) {
		chip->p922x_chg_status.tx_command = P9237_RESPONE_WORKING_IN_EPP;
		chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
		chg_err("<~WPC~> Received TX command: WORKING IN EPP!\n");
		chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
		return;
	}
	
	temp[0] = ~val_buf[2];
	temp[1] = ~val_buf[4];
	if ((val_buf[0] == 0x4F) && (val_buf[1] == temp[0]) && (val_buf[3] == temp[1])) {
		chip->p922x_chg_status.tx_command = val_buf[1];
		chip->p922x_chg_status.tx_data = val_buf[3];
		chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
		chg_err("<~WPC~> Received TX command: 0x%02X, data: 0x%02X\n", chip->p922x_chg_status.tx_command, chip->p922x_chg_status.tx_data);
		chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
	}
}

#else
static void p922x_idt_dischg_status(struct oppo_p922x_ic *chip)
{
    char regdata[2] = {0};
    int rc = 0;

    p922x_read_reg(chip, 0x78, regdata, 2);
    chg_err("<~WPC~>rtx func 0x78-->[0x%x],0x79-->[0x%x]!, wpc_dischg_status[%d]\n", 
            regdata[0], regdata[1], chip->p922x_chg_status.wpc_dischg_status);
    if (regdata[1] != 0) {
        if (P922X_REG_RTX_ERR_TX_RXAC & regdata[1]) {
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_RXAC;
        } else if (P922X_REG_RTX_ERR_TX_OCP & regdata[1]){
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_OCP;
        } else if (P922X_REG_RTX_ERR_TX_OVP & regdata[1]){
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_OVP;
        } else if (P922X_REG_RTX_ERR_TX_LVP & regdata[1]){
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_LVP;
        } else if (P922X_REG_RTX_ERR_TX_FOD & regdata[1]){
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_FOD;
        } else if (P922X_REG_RTX_ERR_TX_OTP & regdata[1]){
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_OTP;
        } else if (P922X_REG_RTX_ERR_TX_CEPTIMEOUT & regdata[1]){
		if (P922X_RTX_TRANSFER & regdata[0]) {
			chip->p922x_chg_status.tx_online = true;
		} else {
			chip->p922x_chg_status.tx_online = false;
		}
           ; //if (chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_IC_ERR_TX_CEPTIMEOUT) {
                //g_oppo_chip->otg_switch = 0;
                //oppo_set_wrx_otg_value(0);
                //mp2650_otg_disable();
                //mp2650_otg_wait_vbus_decline();
                //oppo_set_wrx_en_value(1);
            //} else {
                //chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_CEPTIMEOUT;
                //schedule_delayed_work(&chip->idt_dischg_work, WPC_DISCHG_WAIT_DEVICE_EVENT);
            //}
        } else if (P922X_REG_RTX_ERR_TX_RXEPT& regdata[1]){
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_ERR_TX_RXEPT;
        }
        if ((P922X_REG_RTX_ERR_TX_CEPTIMEOUT & regdata[1]) == 0) {
            //chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_STATUS_OFF;
            //g_oppo_chip->otg_switch = 0;
            mp2650_otg_disable();
            mp2650_otg_wait_vbus_decline();
            oppo_set_wrx_otg_value(0);
            oppo_set_wrx_en_value(0);
        }
    }else {
        if (P922X_RTX_READY & regdata[0]) {
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_READY;
            rc = p922x_config_interface(chip, 0x76, 0x01, 0xFF);
            schedule_delayed_work(&chip->idt_dischg_work, WPC_DISCHG_WAIT_READY_EVENT);
        } else if (P922X_RTX_DIGITALPING & regdata[0] || P922X_RTX_ANALOGPING & regdata[0]) {
            if (WPC_DISCHG_IC_PING_DEVICE == chip->p922x_chg_status.wpc_dischg_status) {
                chg_err("<~WPC~>rtx func no device to be charged, 90s timeout, disable RTX!\n");
                chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_STATUS_OFF;
				chip->p922x_chg_status.tx_online = false;
                //g_oppo_chip->otg_switch = 0;
                mp2650_otg_disable();
                mp2650_otg_wait_vbus_decline();
                oppo_set_wrx_otg_value(0);
                oppo_set_wrx_en_value(0);
            } else {
                chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_PING_DEVICE;
                schedule_delayed_work(&chip->idt_dischg_work, WPC_DISCHG_WAIT_DEVICE_EVENT);
            }
        } else if (P922X_RTX_TRANSFER & regdata[0]) {
			chip->p922x_chg_status.tx_online = true;
            chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_IC_TRANSFER;
            chg_err("<~WPC~>rtx func in discharging now!\n");
        }
    }

    return;
}

static void p922x_idt_dischg_work(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct oppo_p922x_ic *chip = container_of(dwork, struct oppo_p922x_ic, idt_dischg_work);

    chg_err("<~WPC~>rtx func wpc_dischg_status[%d]\n", chip->p922x_chg_status.wpc_dischg_status);

    if (chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_STATUS_OFF) {
		mp2650_otg_disable();
		mp2650_otg_wait_vbus_decline();
		oppo_set_wrx_otg_value(0);
	    oppo_set_wrx_en_value(0);
    } else {
        p922x_idt_dischg_status(chip);
    }

    return;
}

static void p922x_commu_data_process(struct oppo_p922x_ic *chip)
{
	int rc = -1;
	char temp[2] = {0, 0};
	char val_buf[5] = {0,0,0,0,0};

	rc = p922x_read_reg(chip, 0x0036, temp, 2);
	if (rc) {
		chg_err("Couldn't read 0x%04x rc = %x\n", 0x0036, rc);
		temp[0] = 0;
	} else {
		chg_err("read 0x0036 = 0x%02x 0x%02x\n", temp[0], temp[1]);
	}

	if (temp[0] & 0x10) {
		rc = p922x_read_reg(chip, 0x0058, val_buf, 5);
		if (rc) {
			 chg_err("Couldn't read 0x%04x rc = %x\n", 0x0058, rc);
		} else {
			temp[0] = ~val_buf[2];
			temp[1] = ~val_buf[4];
			if ((val_buf[0] == 0x4F) && (val_buf[1] == temp[0]) && (val_buf[3] == temp[1])) {
				chip->p922x_chg_status.tx_command = val_buf[1];
				chip->p922x_chg_status.tx_data = val_buf[3];
				chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
				chg_err("<~WPC~> Received TX command: 0x%02X, data: 0x%02X\n", chip->p922x_chg_status.tx_command, chip->p922x_chg_status.tx_data);
				chg_err("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
			}
		}
	}
    chg_err("rtx func chip->p922x_chg_status.wpc_dischg_status[%d]\n", chip->p922x_chg_status.wpc_dischg_status);
    if (chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_STATUS_ON
        || chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_IC_READY
        || chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_IC_PING_DEVICE
        || chip->p922x_chg_status.wpc_dischg_status == WPC_DISCHG_IC_TRANSFER) {
        cancel_delayed_work_sync(&chip->idt_dischg_work);
        p922x_idt_dischg_status(chip);
    }

	/*clear irq*/
	p922x_config_interface(chip, 0x0036, 0x00, 0xFF);
	p922x_config_interface(chip, 0x0056, 0x30, 0x30);
	p922x_config_interface(chip, 0x004E, 0x20, 0x20);
}

static void p922x_get_EPP_flag(struct oppo_p922x_ic *chip)
{
	int rc = -1;
	char temp;

	rc = p922x_read_reg(chip, 0x0088, &temp, 1);
	if (rc) {
		 chg_err("Couldn't read 0x0088 rc = %x\n",rc);
	} else {
		chg_err("<~WPC~>REG 0x0088 = %x\n", temp);
		
		if (temp == 0x31) {
			chg_err("<~WPC~> RX running in EPP!\n");
			chip->p922x_chg_status.adapter_type = ADAPTER_TYPE_EPP;
			chip->p922x_chg_status.rx_runing_mode = RX_RUNNING_MODE_EPP;
		} else if (temp == 0x04) {
			chg_err("<~WPC~> RX running in BPP!\n");
			chip->p922x_chg_status.rx_runing_mode = RX_RUNNING_MODE_BPP;
		} else{
			chg_err("<~WPC~> RX running in Others!\n");
			chip->p922x_chg_status.rx_runing_mode = RX_RUNNING_MODE_OTHERS;
		}
	}
}
#endif

int p922x_get_idt_con_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->idt_con_gpio <= 0) {
		chg_err("idt_con_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->idt_con_active)
		|| IS_ERR_OR_NULL(chip->idt_con_sleep)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->idt_con_gpio);
}

int p922x_get_idt_int_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->idt_int_gpio <= 0) {
		chg_err("idt_int_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->idt_int_active)
		|| IS_ERR_OR_NULL(chip->idt_int_sleep)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->idt_int_gpio);
}

static void p922x_idt_event_int_func(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct oppo_p922x_ic *chip = container_of(dwork, struct oppo_p922x_ic, idt_event_int_work);

	if (p922x_chip->p922x_chg_status.charge_online == true
			|| p922x_wpc_get_otg_charging() == true) {
		p922x_commu_data_process(chip);
	}
}

static void p922x_idt_connect_int_func(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct oppo_p922x_ic *chip = container_of(dwork, struct oppo_p922x_ic, idt_connect_int_work);

	if (p922x_firmware_is_updating() == true) {
		chg_err("<~WPC~> p922x_firmware_is_updating, return\n");
		return;
	}

	if (g_oppo_chip->charger_exist && (chip->wireless_mode == WIRELESS_MODE_NULL)) {
		chg_err("<~WPC~> wired charging, return\n");
		return;
	}
	
	chg_err("<~WPC~> action for 11111\n");

	if (p922x_get_idt_con_val() == 1) {
		chg_err("<~WPC~> wpc dock has connected!>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		if (p922x_chip->p922x_chg_status.charge_online == false) {
			p922x_chip->p922x_chg_status.charge_online = true;
			chip->wireless_mode = WIRELESS_MODE_RX;

			oppo_chg_cancel_update_work_sync();
			cancel_delayed_work_sync(&chip->p922x_task_work);
			cancel_delayed_work_sync(&chip->idt_event_int_work);

			chg_err("<~WPC~> ready for wireless charge\n");

			oppo_set_wrx_en_value(1);
			
			p922x_init_registers(chip);
			
			mp2650_hardware_init_for_wireless_charge();

#ifndef WPC_USE_SP6001
			p922x_get_EPP_flag(chip);
#endif

			send_msg_timer = 2;
			chip->p922x_chg_status.wpc_reach_stable_charge = false;
			chip->p922x_chg_status.wpc_reach_4370mv = false;
			chip->p922x_chg_status.wpc_reach_4500mv = false;
			chip->p922x_chg_status.wpc_ffc_charge = false;
#ifdef DEBUG_FASTCHG_BY_ADB
			chip->p922x_chg_status.vout_debug_mode = false;
			chip->p922x_chg_status.iout_debug_mode = false;
#endif
			
			schedule_delayed_work(&chip->p922x_task_work, P922X_TASK_INTERVAL);

			oppo_chg_restart_update_work();
			chg_err("<~WPC~> 001\n");
		}
	} else {
		chg_err("<~WPC~> wpc dock has disconnected!< < < < < < < < < < < < <\n");
		if (p922x_chip->p922x_chg_status.charge_online == true) {			
			p922x_chip->p922x_chg_status.charge_online = false;
			chip->wireless_mode = WIRELESS_MODE_NULL;

			chg_err("<~WPC~> cancel delayed work\n");
			oppo_chg_cancel_update_work_sync();
			cancel_delayed_work_sync(&chip->p922x_task_work);
			cancel_delayed_work_sync(&chip->idt_event_int_work);

			chg_err("<~WPC~> exit wireless charge\n");

			if (p922x_get_vbat_en_val() == 1) {
				chargepump_disable();
				p922x_set_cp_ldo_5v_val(0);
				msleep(100);
				if (oppo_get_wired_otg_online() == false)
					oppo_set_wrx_en_value(0);
				p922x_reset_variables(chip);
			} else {
				p922x_set_vbat_en_val(1);
				chargepump_disable();
				p922x_set_cp_ldo_5v_val(0);
				msleep(100);
				if (oppo_get_wired_otg_online() == false)
					oppo_set_wrx_en_value(0);
				p922x_reset_variables(chip);
				msleep(400);
				p922x_set_vbat_en_val(0);
			}

			oppo_chg_restart_update_work();
			chg_err("<~WPC~> 002\n");
		}
	}
}

static void p922x_idt_event_shedule_work(void)
{    
	if (!p922x_chip) {
		chg_err(" p922x_chip is NULL\n");
	} else {
		schedule_delayed_work(&p922x_chip->idt_event_int_work, 0);
	}
}

static void p922x_idt_connect_shedule_work(void)
{    
	if (!p922x_chip) {
		chg_err(" p922x_chip is NULL\n");
	} else {
		schedule_delayed_work(&p922x_chip->idt_connect_int_work, 0);
	}
}

static irqreturn_t irq_idt_event_int_handler(int irq, void *dev_id)
{
    chg_err("<~WPC~> 22222\n");
	p922x_idt_event_shedule_work();
	return IRQ_HANDLED;
}

static irqreturn_t irq_idt_connect_int_handler(int irq, void *dev_id)
{
	chg_err("<~WPC~> 11111-> %d\n", p922x_get_idt_con_val());
	p922x_idt_connect_shedule_work();
	return IRQ_HANDLED;
}

static void p922x_set_idt_int_active(struct oppo_p922x_ic *chip)
{
	gpio_direction_input(chip->idt_int_gpio);	// in
	pinctrl_select_state(chip->pinctrl,chip->idt_int_active);	// no_PULL
}

static void p922x_set_idt_con_active(struct oppo_p922x_ic *chip)
{
	gpio_direction_input(chip->idt_con_gpio);	// in
	pinctrl_select_state(chip->pinctrl,chip->idt_con_active);	// no_PULL
}

static void p922x_idt_int_irq_init(struct oppo_p922x_ic *chip)
{
	chip->idt_int_irq = gpio_to_irq(chip->idt_int_gpio);

    pr_err("tongfeng test %s chip->idt_int_irq[%d]\n",__func__, chip->idt_int_irq);
}

static void p922x_idt_con_irq_init(struct oppo_p922x_ic *chip)
{
	chip->idt_con_irq = gpio_to_irq(chip->idt_con_gpio);
    pr_err("tongfeng test %s chip->idt_con_irq[%d]\n",__func__, chip->idt_con_irq);
}

static void p922x_idt_int_eint_register(struct oppo_p922x_ic *chip)
{
	int retval = 0;
	
	p922x_set_idt_int_active(chip);
	retval = request_irq(chip->idt_int_irq, irq_idt_event_int_handler, IRQF_TRIGGER_FALLING, "p922x_idt_int", chip);	//0X01:rising edge,0x02:falling edge
	if (retval < 0) {
		pr_err("%s request idt_int irq failed.\n", __func__);
	}
}

static void p922x_idt_con_eint_register(struct oppo_p922x_ic *chip)
{
	int retval = 0;
	
    pr_err("%s tongfeng test start, irq happened\n", __func__);
	p922x_set_idt_con_active(chip);
	retval = request_irq(chip->idt_con_irq, irq_idt_connect_int_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "p922x_con_int", chip);	//0X01:rising edge,0x02:falling edge
	if (retval < 0) {
		pr_err("%s request idt_con irq failed.\n", __func__);
	}
}

static int p922x_idt_con_gpio_init(struct oppo_p922x_ic *chip)
{
	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	//idt_con
	chip->idt_con_active = 
			pinctrl_lookup_state(chip->pinctrl, "idt_connect_active");
	if (IS_ERR_OR_NULL(chip->idt_con_active)) {
		chg_err("get idt_con_active fail\n");
		return -EINVAL;
	}

	chip->idt_con_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "idt_connect_sleep");
	if (IS_ERR_OR_NULL(chip->idt_con_sleep)) {
		chg_err("get idt_con_sleep fail\n");
		return -EINVAL;
	}

	chip->idt_con_default = 
			pinctrl_lookup_state(chip->pinctrl, "idt_connect_default");
	if (IS_ERR_OR_NULL(chip->idt_con_default)) {
		chg_err("get idt_con_default fail\n");
		return -EINVAL;
	}
	

	if (chip->idt_con_gpio > 0) {
		gpio_direction_input(chip->idt_con_gpio);
	}

	pinctrl_select_state(chip->pinctrl, chip->idt_con_active);

	return 0;
}

static int p922x_idt_int_gpio_init(struct oppo_p922x_ic *chip)
{

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return -EINVAL;
	}

	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	//idt_int
	chip->idt_int_active = 
			pinctrl_lookup_state(chip->pinctrl, "idt_int_active");
	if (IS_ERR_OR_NULL(chip->idt_int_active)) {
		chg_err("get idt_int_active fail\n");
		return -EINVAL;
	}

	chip->idt_int_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "idt_int_sleep");
	if (IS_ERR_OR_NULL(chip->idt_int_sleep)) {
		chg_err("get idt_int_sleep fail\n");
		return -EINVAL;
	}

	chip->idt_int_default = 
			pinctrl_lookup_state(chip->pinctrl, "idt_int_default");
	if (IS_ERR_OR_NULL(chip->idt_int_default)) {
		chg_err("get idt_int_default fail\n");
		return -EINVAL;
	}

	if (chip->idt_int_gpio > 0) {
		gpio_direction_input(chip->idt_int_gpio);
	}

	pinctrl_select_state(chip->pinctrl, chip->idt_int_active);

	return 0;
}

static int p922x_ext1_wired_otg_gpio_init(struct oppo_p922x_ic *chip)
{

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return -EINVAL;
	}

	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	//vbat_en
	chip->ext1_wired_otg_en_active = 
			pinctrl_lookup_state(chip->pinctrl, "ext1_wired_otg_en_active");
	if (IS_ERR_OR_NULL(chip->ext1_wired_otg_en_active)) {
		chg_err("get ext1_wired_otg_en_active fail\n");
		return -EINVAL;
	}

	chip->ext1_wired_otg_en_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "ext1_wired_otg_en_sleep");
	if (IS_ERR_OR_NULL(chip->ext1_wired_otg_en_sleep)) {
		chg_err("get ext1_wired_otg_en_sleep fail\n");
		return -EINVAL;
	}

	chip->ext1_wired_otg_en_default = 
			pinctrl_lookup_state(chip->pinctrl, "ext1_wired_otg_en_default");
	if (IS_ERR_OR_NULL(chip->ext1_wired_otg_en_default)) {
		chg_err("get ext1_wired_otg_en_default fail\n");
		return -EINVAL;
	}

	if (get_PCB_Version() >= EVT2)
		gpio_direction_output(chip->ext1_wired_otg_en_gpio, 0);
	else
		gpio_direction_output(chip->ext1_wired_otg_en_gpio, 1);
	pinctrl_select_state(chip->pinctrl,
			chip->ext1_wired_otg_en_active);

	return 0;
}

void p922x_set_ext1_wired_otg_en_val(int value)
{
    struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return;
	}

	if (chip->ext1_wired_otg_en_gpio <= 0) {
		chg_err("ext1_wired_otg_en_gpio not exist, return\n");
		return;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->ext1_wired_otg_en_active)
		|| IS_ERR_OR_NULL(chip->ext1_wired_otg_en_sleep)
		|| IS_ERR_OR_NULL(chip->ext1_wired_otg_en_default)) {
		chg_err("pinctrl null, return\n");
		return;
	}

	if (get_PCB_Version() >= EVT2) {
		if (value)
			value = 0;
		else
			value = 1;
	}

	if (value) {
		gpio_direction_output(chip->ext1_wired_otg_en_gpio, 1);
		pinctrl_select_state(chip->pinctrl,
				chip->ext1_wired_otg_en_default);
	} else {
		gpio_direction_output(chip->ext1_wired_otg_en_gpio, 0);
		pinctrl_select_state(chip->pinctrl,
				chip->ext1_wired_otg_en_sleep);
	}

	chg_err("<~WPC~>set value:%d, gpio_val:%d\n", 
		value, gpio_get_value(chip->ext1_wired_otg_en_gpio));
}

int p922x_get_ext1_wired_otg_en_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->ext1_wired_otg_en_gpio <= 0) {
		chg_err("ext1_wired_otg_en_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->ext1_wired_otg_en_active)
		|| IS_ERR_OR_NULL(chip->ext1_wired_otg_en_sleep)
		|| IS_ERR_OR_NULL(chip->ext1_wired_otg_en_default)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->ext1_wired_otg_en_gpio);
}


static int p922x_ext2_wireless_otg_gpio_init(struct oppo_p922x_ic *chip)
{

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return -EINVAL;
	}

	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	chip->ext2_wireless_otg_en_active = 
			pinctrl_lookup_state(chip->pinctrl, "ext2_wireless_otg_en_active");
	if (IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_active)) {
		chg_err("get ext2_wireless_otg_en_active fail\n");
		return -EINVAL;
	}

	chip->ext2_wireless_otg_en_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "ext2_wireless_otg_en_sleep");
	if (IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_sleep)) {
		chg_err("get ext2_wireless_otg_en_sleep fail\n");
		return -EINVAL;
	}

	chip->ext2_wireless_otg_en_default = 
			pinctrl_lookup_state(chip->pinctrl, "ext2_wireless_otg_en_default");
	if (IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_default)) {
		chg_err("get ext2_wireless_otg_en_default fail\n");
		return -EINVAL;
	}

	if (get_PCB_Version() >= EVT2)
		gpio_direction_output(chip->ext2_wireless_otg_en_gpio, 0);
	else
		gpio_direction_output(chip->ext2_wireless_otg_en_gpio, 1);
	pinctrl_select_state(chip->pinctrl,
			chip->ext2_wireless_otg_en_active);

	return 0;
}

void p922x_set_ext2_wireless_otg_en_val(int value)
{
    struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return;
	}

	if (chip->ext2_wireless_otg_en_gpio <= 0) {
		chg_err("ext2_wireless_otg_en_gpio not exist, return\n");
		return;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_active)
		|| IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_sleep)
		|| IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_default)) {
		chg_err("pinctrl null, return\n");
		return;
	}

	if (get_PCB_Version() >= EVT2) {
		if (value)
			value = 0;
		else
			value = 1;
	}

	if (value) {
		gpio_direction_output(chip->ext2_wireless_otg_en_gpio, 1);
		pinctrl_select_state(chip->pinctrl,
				chip->ext2_wireless_otg_en_default);
	} else {
		gpio_direction_output(chip->ext2_wireless_otg_en_gpio, 0);
		pinctrl_select_state(chip->pinctrl,
				chip->ext2_wireless_otg_en_sleep);
	}

	chg_err("<~WPC~>set value:%d, gpio_val:%d\n", 
		value, gpio_get_value(chip->ext2_wireless_otg_en_gpio));
}

int p922x_get_ext2_wireless_otg_en_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->ext2_wireless_otg_en_gpio <= 0) {
		chg_err("ext2_wireless_otg_en_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_active)
		|| IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_sleep)
		|| IS_ERR_OR_NULL(chip->ext2_wireless_otg_en_default)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->ext2_wireless_otg_en_gpio);
}

static int p922x_vbat_en_gpio_init(struct oppo_p922x_ic *chip)
{

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return -EINVAL;
	}

	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	//vbat_en
	chip->vbat_en_active = 
			pinctrl_lookup_state(chip->pinctrl, "vbat_en_active");
	if (IS_ERR_OR_NULL(chip->vbat_en_active)) {
		chg_err("get vbat_en_active fail\n");
		return -EINVAL;
	}

	chip->vbat_en_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "vbat_en_sleep");
	if (IS_ERR_OR_NULL(chip->vbat_en_sleep)) {
		chg_err("get vbat_en_sleep fail\n");
		return -EINVAL;
	}

	chip->vbat_en_default = 
			pinctrl_lookup_state(chip->pinctrl, "vbat_en_default");
	if (IS_ERR_OR_NULL(chip->vbat_en_default)) {
		chg_err("get vbat_en_default fail\n");
		return -EINVAL;
	}

	gpio_direction_output(chip->vbat_en_gpio, 0);
	pinctrl_select_state(chip->pinctrl,
			chip->vbat_en_sleep);

	return 0;
}

void p922x_set_vbat_en_val(int value)
{
    struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return;
	}

	if (chip->vbat_en_gpio <= 0) {
		chg_err("vbat_en_gpio not exist, return\n");
		return;
	}
#ifndef WPC_USE_SP6001
//    printk(KERN_ERR "[OPPO_CHG][%s]: WPC IDT CHIP, do not set vbat-en-gpio[%d]!\n", 
//        __func__, gpio_get_value(chip->vbat_en_gpio));
//    return;
#endif

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->vbat_en_active)
		|| IS_ERR_OR_NULL(chip->vbat_en_sleep)
		|| IS_ERR_OR_NULL(chip->vbat_en_default)) {
		chg_err("pinctrl null, return\n");
		return;
	}

	if (value) {
		gpio_direction_output(chip->vbat_en_gpio, 1);
		pinctrl_select_state(chip->pinctrl,
				chip->vbat_en_default);
	} else {
		gpio_direction_output(chip->vbat_en_gpio, 0);
		pinctrl_select_state(chip->pinctrl,
				chip->vbat_en_sleep);
	}

	chg_err("<~WPC~>set value:%d, gpio_val:%d\n", 
		value, gpio_get_value(chip->vbat_en_gpio));
}

int p922x_get_vbat_en_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->vbat_en_gpio <= 0) {
		chg_err("vbat_en_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->vbat_en_active)
		|| IS_ERR_OR_NULL(chip->vbat_en_sleep)
		|| IS_ERR_OR_NULL(chip->vbat_en_default)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->vbat_en_gpio);
}

static int p922x_booster_en_gpio_init(struct oppo_p922x_ic *chip)
{

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return -EINVAL;
	}

	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	//booster_en
	chip->booster_en_active = 
			pinctrl_lookup_state(chip->pinctrl, "booster_en_active");
	if (IS_ERR_OR_NULL(chip->booster_en_active)) {
		chg_err("get booster_en_active fail\n");
		return -EINVAL;
	}

	chip->booster_en_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "booster_en_sleep");
	if (IS_ERR_OR_NULL(chip->booster_en_sleep)) {
		chg_err("get booster_en_sleep fail\n");
		return -EINVAL;
	}

	chip->booster_en_default = 
			pinctrl_lookup_state(chip->pinctrl, "booster_en_default");
	if (IS_ERR_OR_NULL(chip->booster_en_default)) {
		chg_err("get booster_en_default fail\n");
		return -EINVAL;
	}

	gpio_direction_output(chip->booster_en_gpio, 0);
	pinctrl_select_state(chip->pinctrl,
			chip->booster_en_sleep);

	chg_err("gpio_val:%d\n", gpio_get_value(chip->booster_en_gpio));

	return 0;
}

void p922x_set_booster_en_val(int value)
{    
    struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return;
	}

	if (chip->booster_en_gpio <= 0) {
		chg_err("booster_en_gpio not exist, return\n");
		return;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->booster_en_active)
		|| IS_ERR_OR_NULL(chip->booster_en_sleep)
		|| IS_ERR_OR_NULL(chip->booster_en_default)) {
		chg_err("pinctrl null, return\n");
		return;
	}

	if (value) {
		gpio_direction_output(chip->booster_en_gpio, 1);
		pinctrl_select_state(chip->pinctrl,
				chip->booster_en_active);
	} else {
		gpio_direction_output(chip->booster_en_gpio, 0);
		pinctrl_select_state(chip->pinctrl,
				chip->booster_en_sleep);
	}

	chg_err("set value:%d, gpio_val:%d\n", 
		value, gpio_get_value(chip->booster_en_gpio));
}

int p922x_get_booster_en_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->booster_en_gpio <= 0) {
		chg_err("booster_en_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->booster_en_active)
		|| IS_ERR_OR_NULL(chip->booster_en_sleep)
		|| IS_ERR_OR_NULL(chip->booster_en_default)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->booster_en_gpio);
}


void p922x_set_cp_ldo_5v_val(int value)
{    
    struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return;
	}

	if (chip->cp_ldo_5v_gpio <= 0) {
		chg_err("cp_ldo_5v_gpio not exist, return\n");
		return;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->cp_ldo_5v_active)
		|| IS_ERR_OR_NULL(chip->cp_ldo_5v_sleep)
		|| IS_ERR_OR_NULL(chip->cp_ldo_5v_default)) {
		chg_err("pinctrl null, return\n");
		return;
	}

	if (value) {
		gpio_direction_output(chip->cp_ldo_5v_gpio, 1);
		pinctrl_select_state(chip->pinctrl,
				chip->cp_ldo_5v_active);
	} else {
		gpio_direction_output(chip->cp_ldo_5v_gpio, 0);
		pinctrl_select_state(chip->pinctrl,
				chip->cp_ldo_5v_sleep);
	}

	chg_err("set value:%d, gpio_val:%d\n", 
		value, gpio_get_value(chip->cp_ldo_5v_gpio));
}

int p922x_get_cp_ldo_5v_val(void)
{
	struct oppo_p922x_ic *chip = p922x_chip;

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: p922x_chip not ready!\n", __func__);
		return -1;
	}

	if (chip->cp_ldo_5v_gpio <= 0) {
		chg_err("cp_ldo_5v_gpio not exist, return\n");
		return -1;
	}

	if (IS_ERR_OR_NULL(chip->pinctrl)
		|| IS_ERR_OR_NULL(chip->cp_ldo_5v_active)
		|| IS_ERR_OR_NULL(chip->cp_ldo_5v_sleep)
		|| IS_ERR_OR_NULL(chip->cp_ldo_5v_default)) {
		chg_err("pinctrl null, return\n");
		return -1;
	}

	return gpio_get_value(chip->cp_ldo_5v_gpio);
}

static int p922x_cp_ldo_5v_gpio_init(struct oppo_p922x_ic *chip)
{

	if (!chip) {
		printk(KERN_ERR "[OPPO_CHG][%s]: oppo_p922x_ic not ready!\n", __func__);
		return -EINVAL;
	}

	chip->pinctrl = devm_pinctrl_get(chip->dev);
	if (IS_ERR_OR_NULL(chip->pinctrl)) {
		chg_err("get pinctrl fail\n");
		return -EINVAL;
	}

	chip->cp_ldo_5v_active = 
			pinctrl_lookup_state(chip->pinctrl, "cp_ldo_5v_active");
	if (IS_ERR_OR_NULL(chip->cp_ldo_5v_active)) {
		chg_err("get cp_ldo_5v_active fail\n");
		return -EINVAL;
	}

	chip->cp_ldo_5v_sleep = 
			pinctrl_lookup_state(chip->pinctrl, "cp_ldo_5v_sleep");
	if (IS_ERR_OR_NULL(chip->cp_ldo_5v_sleep)) {
		chg_err("get cp_ldo_5v_sleep fail\n");
		return -EINVAL;
	}

	chip->cp_ldo_5v_default = 
			pinctrl_lookup_state(chip->pinctrl, "cp_ldo_5v_default");
	if (IS_ERR_OR_NULL(chip->cp_ldo_5v_default)) {
		chg_err("get cp_ldo_5v_default fail\n");
		return -EINVAL;
	}

	gpio_direction_output(chip->cp_ldo_5v_gpio, 0);
	pinctrl_select_state(chip->pinctrl,
			chip->cp_ldo_5v_sleep);

	chg_err("gpio_val:%d\n", gpio_get_value(chip->cp_ldo_5v_gpio));

	return 0;
}

static int p922x_idt_gpio_init(struct oppo_p922x_ic *chip)
{

    int rc=0;
	struct device_node *node = chip->dev->of_node;
    pr_err("tongfeng test %s start\n",__func__);

	// Parsing gpio idt_int
	chip->idt_int_gpio = of_get_named_gpio(node, "qcom,idt_int-gpio", 0);
	if (chip->idt_int_gpio < 0 ) {
		pr_err("chip->idt_int_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->idt_int_gpio)) {
			rc = gpio_request(chip->idt_int_gpio, "idt-idt-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->idt_int_gpio);
			} else {
				rc = p922x_idt_int_gpio_init(chip);
				if (rc)
					chg_err("unable to init idt_int_gpio:%d\n", chip->idt_int_gpio);
				else {
					p922x_idt_int_irq_init(chip);
					p922x_idt_int_eint_register(chip);
				}
			}
		}

		pr_err("chip->idt_int_gpio =%d\n",chip->idt_int_gpio);
	}

	// Parsing gpio idt_connect
	chip->idt_con_gpio = of_get_named_gpio(node, "qcom,idt_connect-gpio", 0);
	if (chip->idt_con_gpio < 0) {
		pr_err("chip->idt_con_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->idt_con_gpio)) {
			rc = gpio_request(chip->idt_con_gpio, "idt-connect-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->idt_con_gpio);
			} else {
				rc = p922x_idt_con_gpio_init(chip);
				if (rc)
					chg_err("unable to init idt_con_gpio:%d\n", chip->idt_con_gpio);
				else {
					p922x_idt_con_irq_init(chip);
					p922x_idt_con_eint_register(chip);
				}
			}
		}
		
		pr_err("chip->idt_con_gpio =%d\n",chip->idt_con_gpio);
	}

	// Parsing gpio vbat_en
	chip->vbat_en_gpio = of_get_named_gpio(node, "qcom,vbat_en-gpio", 0);
	if (chip->vbat_en_gpio < 0) {
		pr_err("chip->vbat_en_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->vbat_en_gpio)) {
			rc = gpio_request(chip->vbat_en_gpio, "vbat-en-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->vbat_en_gpio);
			} else {
				rc = p922x_vbat_en_gpio_init(chip);
				if (rc) {
					chg_err("unable to init vbat_en_gpio:%d\n", chip->vbat_en_gpio);
				}
			}
		}
		
		pr_err("chip->vbat_en_gpio =%d\n",chip->vbat_en_gpio);
	}

	// Parsing gpio ext1 otg
	chip->ext1_wired_otg_en_gpio = of_get_named_gpio(node, "qcom,ext1_wired_otg_en-gpio", 0);
	if (chip->ext1_wired_otg_en_gpio < 0) {
		pr_err("chip->ext1_wired_otg_en_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->ext1_wired_otg_en_gpio)) {
			rc = gpio_request(chip->ext1_wired_otg_en_gpio, "ext1_wired_otg_en-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->ext1_wired_otg_en_gpio);
			} else {
				rc = p922x_ext1_wired_otg_gpio_init(chip);
				if (rc) {
					chg_err("unable to init ext1_wired_otg_en_gpio:%d\n", chip->ext1_wired_otg_en_gpio);
				}
			}
		}
		
		pr_err("chip->ext1_wired_otg_en_gpio =%d\n",chip->ext1_wired_otg_en_gpio);
	}

	// Parsing gpio ext2 wireless otg
	chip->ext2_wireless_otg_en_gpio = of_get_named_gpio(node, "qcom,ext2_wireless_otg_en-gpio", 0);
	if (chip->ext2_wireless_otg_en_gpio < 0) {
		pr_err("chip->ext2_wireless_otg_en_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->ext2_wireless_otg_en_gpio)) {
			rc = gpio_request(chip->ext2_wireless_otg_en_gpio, "ext2_wireless_otg_en-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->ext2_wireless_otg_en_gpio);
			} else {
				rc = p922x_ext2_wireless_otg_gpio_init(chip);
				if (rc) {
					chg_err("unable to init ext2_wireless_otg_en_gpio:%d\n", chip->ext2_wireless_otg_en_gpio);
				}
			}
		}

		pr_err("chip->ext2_wireless_otg_en_gpio =%d\n",chip->ext2_wireless_otg_en_gpio);
	}

	// Parsing gpio booster_en
	chip->booster_en_gpio = of_get_named_gpio(node, "qcom,booster_en-gpio", 0);
	if (chip->booster_en_gpio < 0) {
		pr_err("chip->booster_en_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->booster_en_gpio)) {
			rc = gpio_request(chip->booster_en_gpio, "booster-en-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->booster_en_gpio);
			} else {
				rc = p922x_booster_en_gpio_init(chip);
				if (rc) {
					chg_err("unable to init booster_en_gpio:%d\n", chip->booster_en_gpio);
				}
			}
		}
		
		pr_err("chip->booster_en_gpio =%d\n",chip->booster_en_gpio);
	}

	// Parsing gpio 5v_en
	chip->cp_ldo_5v_gpio = of_get_named_gpio(node, "qcom,cp_ldo_5v-gpio", 0);
	if (chip->cp_ldo_5v_gpio < 0) {
		pr_err("chip->cp_ldo_5v_gpio not specified\n");	
	} else {
		if (gpio_is_valid(chip->cp_ldo_5v_gpio)) {
			rc = gpio_request(chip->cp_ldo_5v_gpio, "cp-ldo-5v-gpio");
			if (rc) {
				pr_err("unable to request gpio [%d]\n", chip->cp_ldo_5v_gpio);
			} else {
				rc = p922x_cp_ldo_5v_gpio_init(chip);
				if (rc) {
					chg_err("unable to init cp_ldo_5v_gpio:%d\n", chip->cp_ldo_5v_gpio);
				}
			}
		}

		pr_err("chip->cp_ldo_5v_gpio =%d\n",chip->cp_ldo_5v_gpio);
	}

	return rc;
}

#ifdef DEBUG_BY_FILE_OPS
int p922x_add;
static ssize_t p922x_add_log_write(struct file *filp, const char __user *buff, size_t len, loff_t *data)
{
	char write_data[32] = {0};
	char val_buf;
	int rc;
	
	if (copy_from_user(&write_data, buff, len)) {
		pr_err("bat_log_write error.\n");
		return -EFAULT;
	}
	write_data[len] = '\0';
	if (write_data[len - 1] == '\n') {
		write_data[len - 1] = '\0';
	}
	
	p922x_add = (int)simple_strtoul(write_data, NULL, 10);

	pr_err("%s:received data=%s, p922x_register address: 0x%02x\n", __func__, write_data, p922x_add);

	rc = p922x_read_reg(p922x_chip, p922x_add, &val_buf, 1);
	if (rc) {
		 chg_err("Couldn't read 0x%02x rc = %d\n", p922x_add, rc);
	} else {
		 chg_err("p922x_read 0x%02x = 0x%02x\n", p922x_add, val_buf);
	}

	return len;
}

static const struct file_operations p922x_add_log_proc_fops = {
	.write = p922x_add_log_write,
};
static void init_p922x_add_log(void)
{
	struct proc_dir_entry *p = NULL;

	p = proc_create("p922x_add_log", 0664, NULL, &p922x_add_log_proc_fops);
	if (!p) {
		pr_err("proc_create init_p922x_add_log_proc_fops fail!\n");
	}
}

static ssize_t p922x_data_log_write(struct file *filp, const char __user *buff, size_t len, loff_t *data)
{
	char write_data[32] = {0};
	int critical_log = 0;
	int rc;
	
	if (copy_from_user(&write_data, buff, len)) {
		pr_err("bat_log_write error.\n");
		return -EFAULT;
	}

	write_data[len] = '\0';
	if (write_data[len - 1] == '\n') {
		write_data[len - 1] = '\0';
	}
	
	critical_log = (int)simple_strtoul(write_data, NULL, 10);
	if (critical_log > 0xFF) {
		critical_log = 0xFF;
	}
	
	pr_err("%s:received data=%s, p922x_data=%x\n", __func__, write_data, critical_log);

	rc = p922x_config_interface(p922x_chip, p922x_add, critical_log, 0xFF);
	if (rc) {
		 chg_err("Couldn't write 0x%02x rc = %d\n", p922x_add, rc);
	} 

	return len;
}

static const struct file_operations p922x_data_log_proc_fops = {
	.write = p922x_data_log_write,
};
static void init_p922x_data_log(void)
{
	struct proc_dir_entry *p = NULL;

	p = proc_create("p922x_data_log", 0664, NULL, &p922x_data_log_proc_fops);
	if (!p) 
		pr_err("proc_create init_p922x_data_log_proc_fops fail!\n");

}
#endif /*DEBUG_BY_FILE_OPS*/

static void p922x_task_work_process(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct oppo_p922x_ic *chip = container_of(dwork, struct oppo_p922x_ic, p922x_task_work);
	static int idt_disconnect_cnt = 0;
	
	chg_err("<~WPC~> in\n");

	if (p922x_get_idt_con_val() == 0) {
		chg_err("<~WPC~> idt_connect == 0\n");
		
		idt_disconnect_cnt++;
		if (idt_disconnect_cnt >= 2) {
			if (p922x_chip->p922x_chg_status.charge_online) {
				chg_err("<~WPC~> idt_connect has dispeared. exit wpc\n");
				schedule_delayed_work(&p922x_chip->idt_connect_int_work, 0);
			}
		}

		return;
	} else {
		idt_disconnect_cnt = 0;
	}
	
	if (p922x_chip->p922x_chg_status.charge_online) {
		p922x_get_vrect_iout(chip);

		p922x_RXTX_message_process(chip);
		p922x_temperature_and_terminate_process(chip);
		p922x_charge_status_process(chip);

		/* run again after interval */
		schedule_delayed_work(&chip->p922x_task_work, P922X_TASK_INTERVAL);
	}

	chg_err("<~WPC~> out\n");
}

static void p922x_CEP_work_process(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct oppo_p922x_ic *chip = container_of(dwork, struct oppo_p922x_ic, p922x_CEP_work);
	
	if (p922x_chip->p922x_chg_status.charge_online) {
		p922x_detect_CEP(chip);
		schedule_delayed_work(&chip->p922x_CEP_work, P922X_CEP_INTERVAL);
	}
}

static void p922x_update_work_process(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct oppo_p922x_ic *chip = container_of(dwork, struct oppo_p922x_ic, p922x_update_work);
	int rc;
    int boot_mode = get_boot_mode();

    if (boot_mode == MSM_BOOT_MODE__FACTORY) {
        chg_err("<IDT UPDATE> MSM_BOOT_MODE__FACTORY do not update\n");
        return;
    }
	chg_err("<IDT UPDATE> p922x_update_work_process\n");

	if (!chip->p922x_chg_status.check_fw_update) {
		rc = p922x_check_idt_fw_update(chip);
		if (rc) {
			/* run again after interval */
			;//schedule_delayed_work(&chip->p922x_update_work, P922X_UPDATE_RETRY_INTERVAL);
		}
	}
}

static void wlchg_reset_variables(struct oppo_p922x_ic *chip)
{
	struct wpc_data *chg_status = &chip->p922x_chg_status;
	//struct charge_param *chg_param = &chip->chg_param;

	//chip->pmic_high_vol = false;

	chg_status->charge_status = WPC_CHG_STATUS_DEFAULT;
	chg_status->fastchg_startup_step = FASTCHG_EN_CHGPUMP1_STEP;
	chg_status->charge_online = false;
	chg_status->tx_online = false;
	chg_status->tx_present = false;
	chg_status->charge_voltage = 0;
	chg_status->charge_current = 0;
	chg_status->temp_region = WLCHG_TEMP_REGION_MAX;
	chg_status->wpc_dischg_status = WPC_DISCHG_STATUS_OFF;
	chg_status->fastchg_ing = false;
	chg_status->max_current = FASTCHG_CURR_MAX_UA / 1000;
	chg_status->target_curr = WPC_CHARGE_CURRENT_DEFAULT;
	chg_status->target_vol = WPC_CHARGE_VOLTAGE_DEFAULT;
	chg_status->vol_set = WPC_CHARGE_VOLTAGE_DEFAULT;
	chg_status->curr_limit_mode = false;
	chg_status->vol_set_ok = true;
	chg_status->curr_set_ok = true;
	chg_status->fastchg_mode = false;
	chg_status->startup_fast_chg = false;
	chg_status->cep_err_flag = false;
	chg_status->cep_exit_flag = false;
	chg_status->ffc_check = false;
	chg_status->curr_need_dec = false;
	chg_status->vol_not_step = false; //By default, voltage drop requires step
	chg_status->is_power_changed = false;
	chg_status->vbat_too_high = false;
	chg_status->freq_threshold = 130;
	chg_status->freq_check_count = 0;
	chg_status->freq_thr_inc = false;
	chg_status->is_deviation = false;
	chg_status->deviation_check_done = false;

	//chg_param->mBattTempBoundT0 = chg_param->BATT_TEMP_T0;
	//chg_param->mBattTempBoundT1 = chg_param->BATT_TEMP_T1;
	//chg_param->mBattTempBoundT2 = chg_param->BATT_TEMP_T2;
	//chg_param->mBattTempBoundT3 = chg_param->BATT_TEMP_T3;
	//chg_param->mBattTempBoundT4 = chg_param->BATT_TEMP_T4;
	//chg_param->mBattTempBoundT5 = chg_param->BATT_TEMP_T5;
	//chg_param->mBattTempBoundT6 = chg_param->BATT_TEMP_T6;

	if (p922x_chip != NULL)
		p922x_reset_variables(p922x_chip);

}

static ssize_t proc_wireless_voltage_rect_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    uint8_t ret = 0;
    char page[10];
    int vrect = 0;
    struct oppo_p922x_ic *chip = p922x_chip;

    if(chip == NULL) {
        chg_err("%s: p922x driver is not ready\n", __func__);
        return 0;
    }
    if (atomic_read(&chip->suspended) == 1) {
        return 0;
    }

    vrect = chip->p922x_chg_status.vrect;

    chg_err("%s: vrect = %d.\n", __func__, vrect);
    sprintf(page, "%d", vrect);
    ret = simple_read_from_buffer(buf, count, ppos, page, strlen(page));

    return ret;
}

static ssize_t proc_wireless_voltage_rect_write(struct file *file, const char __user *buf, size_t count, loff_t *lo)
{
    return count;
}

static const struct file_operations proc_wireless_voltage_rect_ops =
{
    .read = proc_wireless_voltage_rect_read,
    .write  = proc_wireless_voltage_rect_write,
    .open  = simple_open,
    .owner = THIS_MODULE,
};

static ssize_t proc_wireless_current_out_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    uint8_t ret = 0;
    char page[10];
    int iout = 0;
    struct oppo_p922x_ic *chip = p922x_chip;

    if(chip == NULL) {
        chg_err("%s: p922x driver is not ready\n", __func__);
        return 0;
    }
    if (atomic_read(&chip->suspended) == 1) {
        return 0;
    }

    iout = chip->p922x_chg_status.iout;

    chg_err("%s: iout = %d.\n", __func__, iout);
    sprintf(page, "%d", iout);
    ret = simple_read_from_buffer(buf, count, ppos, page, strlen(page));

    return ret;
}

static ssize_t proc_wireless_current_out_write(struct file *file, const char __user *buf, size_t count, loff_t *lo)
{
#ifdef DEBUG_FASTCHG_BY_ADB
	char cur_string[8];
	int cur = 0;
	int len = count < 8 ? count : 8;

	if (p922x_chip == NULL) {
		chg_err("%s: p922x_chip is not ready\n", __func__);
		return -ENODEV;
	}

	copy_from_user(cur_string, buf, len);
	kstrtoint(cur_string, 0, &cur);
	chg_err("set current: cur_string = %s, vol = %d.", cur_string, cur);
	p922x_chip->p922x_chg_status.fastchg_current = cur;

	p922x_chip->p922x_chg_status.vout_debug_mode = false;
	p922x_chip->p922x_chg_status.iout_debug_mode = true;
#endif

    return count;
}

static const struct file_operations proc_wireless_current_out_ops =
{
    .read = proc_wireless_current_out_read,
    .write  = proc_wireless_current_out_write,
    .open  = simple_open,
    .owner = THIS_MODULE,
};


static ssize_t proc_wireless_ftm_mode_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    return count;
}

static ssize_t proc_wireless_ftm_mode_write(struct file *file, const char __user *buf, size_t len, loff_t *lo)
{
    char buffer[2] = {0};

    chg_err("%s: len[%d] start.\n", __func__, len);
    if (len > 2) {
        return -EFAULT;
    }

    if (copy_from_user(buffer, buf, 2)) {
        chg_err("%s:  error.\n", __func__);
        return -EFAULT;
    }

    chg_err("%s: buffer[%s] .\n", __func__, buffer);
    if (buffer[0] == '0') {
        chg_err("%s:ftm_mode write 0.\n", __func__);
        p922x_enable_ftm(false);
    } else {
        chg_err("%s:ftm_mode write 1.\n", __func__);
        p922x_enable_ftm(true);
    }
    chg_err("%s: end.\n", __func__);

    return len;
}

static const struct file_operations proc_wireless_ftm_mode_ops =
{
    .read = proc_wireless_ftm_mode_read,
    .write  = proc_wireless_ftm_mode_write,
    .open  = simple_open,
    .owner = THIS_MODULE,
};

static ssize_t proc_wireless_rx_voltage_read(struct file *file,
					     char __user *buf, size_t count,
					     loff_t *ppos)
{
	char vol_string[8];
	int len = 0;
	len = snprintf(vol_string, 8, "%d",
		       p922x_chip->p922x_chg_status.charge_voltage);

	copy_to_user(buf, vol_string, len);

	return 0;
}
static ssize_t proc_wireless_rx_voltage_write(struct file *file,
					      const char __user *buf,
					      size_t count, loff_t *lo)
{
	char vol_string[8];
	int vol = 0;
	int len = count < 8 ? count : 8;

	if (p922x_chip == NULL) {
		chg_err("%s: p922x_chip is not ready\n", __func__);
		return -ENODEV;
	}

	copy_from_user(vol_string, buf, len);
	kstrtoint(vol_string, 0, &vol);
	chg_err("set voltage: vol_string = %s, vol = %d.", vol_string, vol);
	p922x_set_rx_charge_voltage(p922x_chip, vol);

#ifdef DEBUG_FASTCHG_BY_ADB
	p922x_chip->p922x_chg_status.vout_debug_mode = true;
	p922x_chip->p922x_chg_status.iout_debug_mode = false;
#endif

	return count;
}

static const struct file_operations proc_wireless_rx_voltage = {
	.read = proc_wireless_rx_voltage_read,
	.write = proc_wireless_rx_voltage_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static ssize_t proc_wireless_tx_read(struct file *file, char __user *buf,
				     size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
	char page[10];
	struct oppo_p922x_ic *chip = p922x_chip;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return -ENODEV;
	}

	if (chip->wireless_mode == WIRELESS_MODE_TX) {
		if (chip->p922x_chg_status.tx_online)
			snprintf(page, 10, "%s\n", "charging");
		else
			snprintf(page, 10, "%s\n", "enable");
	} else
		snprintf(page, 10, "%s\n", "disable");

	ret = simple_read_from_buffer(buf, count, ppos, page, strlen(page));
	return ret;
}

static ssize_t proc_wireless_tx_write(struct file *file, const char __user *buf,
				      size_t count, loff_t *lo)
{
	char buffer[5] = { 0 };
	struct oppo_p922x_ic *chip = p922x_chip;
	int val;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return -ENODEV;
	}

	if (count > 5) {
		return -EFAULT;
	}

	if (copy_from_user(buffer, buf, count)) {
		chg_err("%s: error.\n", __func__);
		return -EFAULT;
	}

	chg_err("buffer=%s", buffer);
	kstrtoint(buffer, 0, &val);
	chg_err("val = %d", val);

	if (val == 1) {
		//if (chip->wireless_mode == WIRELESS_MODE_NULL) {
			//if (p922x_get_usbin_val() == 1) {
			//	chg_err("USB cable is in, don't allow enter otg wireless charge.");
			//	return -EFAULT;
			//}
			wlchg_reset_variables(chip);
			p922x_set_rtx_function(true);
		//} else {
		//	return -EFAULT;
		//}
	} else {
		//if (chip->wireless_mode == WIRELESS_MODE_TX) {
			p922x_set_rtx_function(false);
		//} else {
		//	return -EFAULT;
		//}
	}
	return count;
}

static const struct file_operations proc_wireless_tx_ops = {
	.read = proc_wireless_tx_read,
	.write = proc_wireless_tx_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static ssize_t proc_wireless_epp_read(struct file *file, char __user *buf,
				      size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
#ifdef oppo_wireless
	char page[6];
	struct oppo_p922x_ic *chip = p922x_chip;
	size_t len = 6;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

	memset(page, 0, 6);
	if (force_epp) {
		len = snprintf(page, len, "epp\n");
	} else if (force_bpp) {
		len = snprintf(page, len, "bpp\n");
	} else if (!auto_mode) {
		len = snprintf(page, len, "manu\n");
	} else {
		len = snprintf(page, len, "auto\n");
	}
	ret = simple_read_from_buffer(buf, count, ppos, page, len);
#endif
	return ret;
}

static ssize_t proc_wireless_epp_write(struct file *file,
				       const char __user *buf, size_t count,
				       loff_t *lo)
{
#ifdef oppo_wireless
	char buffer[5] = { 0 };
	int val = 0;

	chg_err("%s: len[%d] start.\n", __func__, count);
	if (count > 5) {
		return -EFAULT;
	}

	if (copy_from_user(buffer, buf, count)) {
		chg_err("%s: error.\n", __func__);
		return -EFAULT;
	}
	chg_err("buffer=%s", buffer);
	kstrtoint(buffer, 0, &val);
	chg_err("val=%d", val);
	if (val == 1) {
		force_bpp = true;
		force_epp = false;
		auto_mode = true;
	} else if (val == 2) {
		force_bpp = false;
		force_epp = true;
		auto_mode = true;
	} else if (val == 3) {
		force_bpp = false;
		force_epp = false;
		auto_mode = false;
	} else {
		force_bpp = false;
		force_epp = false;
		auto_mode = true;
	}
#endif
	return count;
}

static const struct file_operations proc_wireless_epp_ops = {
	.read = proc_wireless_epp_read,
	.write = proc_wireless_epp_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};
static int proc_charge_pump_status;
static ssize_t proc_wireless_charge_pump_read(struct file *file, char __user *buf,
					   size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
	char page[6];
	struct oppo_p922x_ic *chip = p922x_chip;
	size_t len = 6;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

	memset(page, 0, 6);
	len = snprintf(page, len, "%d\n", proc_charge_pump_status);
	ret = simple_read_from_buffer(buf, count, ppos, page, len);
	return ret;
}

static ssize_t proc_wireless_charge_pump_write(struct file *file,
					       const char __user *buf,
					       size_t count, loff_t *lo)
{
	char buffer[2] = { 0 };
	int val = 0;

	chg_err("%s: len[%d] start.\n", __func__, count);
	if (count > 2) {
		return -EFAULT;
	}

	if (copy_from_user(buffer, buf, count)) {
		chg_err("%s: error.\n", __func__);
		return -EFAULT;
	}
	chg_err("buffer=%s", buffer);
	val = buffer[0] - '0';
	chg_err("val=%d", val);
	if (val < 0 || val > 6) {
		return -EINVAL;
	}
	switch (val) {
	case 0:
		chg_err("wkcs: disable all charge pump\n");
		chargepump_disable();
		//bq2597x_enable_charge_pump(false);
		break;
	case 1:
		chg_err("wkcs: disable charge pump 1\n");
		chargepump_disable();
		break;
	case 2:
		chg_err("wkcs: enable charge pump 1\n");
		chargepump_set_for_EPP(); //enable chargepump
		chargepump_enable();
		chargepump_set_for_LDO();
		break;
	case 3:
		chg_err("wkcs: disable charge pump 2\n");
		//bq2597x_enable_charge_pump(false);
		break;
	case 4:
		chg_err("wkcs: enable charge pump 2\n");
		//bq2597x_enable_charge_pump(true);
		break;
	case 5:
		p922x_set_rx_charge_current(p922x_chip, 0);
		//pmic_high_vol_en(g_op_chip, false);
		break;
	case 6:
		//pmic_high_vol_en(g_op_chip, true);
		p922x_set_rx_charge_current(p922x_chip, 300);
		break;
	default:
		chg_err("wkcs: invalid value.");
		break;
	}
	proc_charge_pump_status = val;
	return count;
}

static const struct file_operations proc_wireless_charge_pump_ops = {
	.read = proc_wireless_charge_pump_read,
	.write = proc_wireless_charge_pump_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static ssize_t proc_wireless_bat_mult_read(struct file *file, char __user *buf,
					   size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
	char page[6];
	struct oppo_p922x_ic *chip = p922x_chip;
	size_t len = 6;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

	memset(page, 0, 6);
	//snprintf(page, len, "%d\n", test_bat_val);
	ret = simple_read_from_buffer(buf, count, ppos, page, len);
	return ret;
}

static ssize_t proc_wireless_bat_mult_write(struct file *file,
					    const char __user *buf,
					    size_t count, loff_t *lo)
{
#ifdef oppo_wireless
	char buffer[5] = { 0 };
	int val = 0;

	chg_err("%s: len[%d] start.\n", __func__, count);
	if (count > 5) {
		return -EFAULT;
	}

	if (copy_from_user(buffer, buf, count)) {
		chg_err("%s: error.\n", __func__);
		return -EFAULT;
	}
	chg_err("buffer=%s", buffer);
	kstrtoint(buffer, 0, &val);
	chg_err("val=%d", val);
	test_bat_val = val;
#endif
	return count;
}

static const struct file_operations proc_wireless_bat_mult_ops = {
	.read = proc_wireless_bat_mult_read,
	.write = proc_wireless_bat_mult_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static ssize_t proc_wireless_deviated_read(struct file *file, char __user *buf,
					   size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
	char page[7];
	struct oppo_p922x_ic *chip = p922x_chip;
	size_t len = 7;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

	memset(page, 0, 7);
	if (chip->p922x_chg_status.is_deviation) {
		len = snprintf(page, len, "%s\n", "true");
	} else {
		len = snprintf(page, len, "%s\n", "false");
	}
	ret = simple_read_from_buffer(buf, count, ppos, page, len);
	return ret;
}

static const struct file_operations proc_wireless_deviated_ops = {
	.read = proc_wireless_deviated_read,
	.write = NULL,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static ssize_t proc_wireless_rx_read(struct file *file, char __user *buf,
					    size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
	char page[3];
	struct oppo_p922x_ic *chip = p922x_chip;

	if (chip == NULL) {
		chg_err("<~WPC~> p922x_chip is NULL!\n");
		return -ENODEV;
	}


	memset(page, 0, 3);
	snprintf(page, 3, "%c\n", !chip->disable_charge ? '1' : '0');
	ret = simple_read_from_buffer(buf, count, ppos, page, 3);
	return ret;
}

static ssize_t proc_wireless_rx_write(struct file *file, const char __user *buf,
				      size_t count, loff_t *lo)
{
	char buffer[5] = { 0 };
	struct oppo_p922x_ic *chip = p922x_chip;
	int val;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return -ENODEV;
	}

	if (count > 5) {
		return -EFAULT;
	}

	if (copy_from_user(buffer, buf, count)) {
		chg_err("%s: error.\n", __func__);
		return -EFAULT;
	}

	chg_err("buffer=%s", buffer);
	kstrtoint(buffer, 0, &val);
	chg_err("val = %d", val);

	if (val == 0) {
		chip->disable_charge = true;
		p922x_set_vbat_en_val(1);
	} else {
		chip->disable_charge = false;
		p922x_set_vbat_en_val(0);
	}
	return count;
}

static const struct file_operations proc_wireless_rx_ops = {
	.read = proc_wireless_rx_read,
	.write = proc_wireless_rx_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

#define UPGRADE_START 0
#define UPGRADE_FW    1
#define UPGRADE_END   2
struct idt_fw_head {
	u8 magic[4];
	int size;
};
static ssize_t proc_wireless_upgrade_firmware_write(struct file *file,
					      const char __user *buf,
					      size_t count, loff_t *lo)
{
#ifdef oppo_wireless
	u8 temp_buf[sizeof(struct idt_fw_head)];
	int rc = 0;
	static u8 *fw_buf;
	static int upgrade_step = UPGRADE_START;
	static int fw_index;
	static int fw_size;
	struct idt_fw_head *fw_head;

	if (p922x_chip == NULL) {
		chg_err("<IDT UPDATE>p922x_chip not't ready\n");
		return -ENODEV;
	}

start:
	switch (upgrade_step) {
	case UPGRADE_START:
		if (count < sizeof(struct idt_fw_head)) {
			chg_err("<IDT UPDATE>image format error\n");
			return -EINVAL;
		}
		memset(temp_buf, 0, sizeof(struct idt_fw_head));
		copy_from_user(temp_buf, buf, sizeof(struct idt_fw_head));
		fw_head = (struct idt_fw_head *)temp_buf;
		if (fw_head->magic[0] == 0x02 && fw_head->magic[1] == 0x00 &&
		    fw_head->magic[2] == 0x03 && fw_head->magic[3] == 0x00) {
			fw_size = fw_head->size;
			fw_buf = kzalloc(fw_size, GFP_KERNEL);
			if (fw_buf == NULL) {
				chg_err("<IDT UPDATE>alloc fw_buf err\n");
				return -ENOMEM;
			}
			chg_err("<IDT UPDATE>image header verification succeeded, fw_size=%d\n", fw_size);
			copy_from_user(fw_buf, buf + sizeof(struct idt_fw_head), count - sizeof(struct idt_fw_head));
			fw_index = count - sizeof(struct idt_fw_head);
			chg_info("<IDT UPDATE>Receiving image, fw_size=%d, fw_index=%d\n", fw_size, fw_index);
			if (fw_index >= fw_size) {
				upgrade_step = UPGRADE_END;
				goto start;
			} else {
				upgrade_step = UPGRADE_FW;
			}
		} else {
			chg_err("<IDT UPDATE>image format error\n");
			return -EINVAL;
		}
		break;
	case UPGRADE_FW:
		copy_from_user(fw_buf + fw_index, buf, count);
		fw_index += count;
		chg_info("<IDT UPDATE>Receiving image, fw_size=%d, fw_index=%d\n", fw_size, fw_index);
		if (fw_index >= fw_size) {
			upgrade_step = UPGRADE_END;
			goto start;
		}
		break;
	case UPGRADE_END:
		rc = p922x_upgrade_firmware(p922x_chip, fw_buf, fw_size);
		kfree(fw_buf);
		fw_buf = NULL;
		upgrade_step = UPGRADE_START;
		if (rc < 0)
			return rc;
		break;
	default:
		upgrade_step = UPGRADE_START;
		chg_err("<IDT UPDATE>status error\n");
		if (fw_buf != NULL) {
			kfree(fw_buf);
			fw_buf = NULL;
		}
		break;
	}
#endif
	return count;
}

static const struct file_operations proc_upgrade_firmware_ops = {
	.read = NULL,
	.write = proc_wireless_upgrade_firmware_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static ssize_t proc_wireless_rx_freq_read(struct file *file,
					  char __user *buf, size_t count,
					  loff_t *ppos)
{
	int rc;
	char string[8];
	int len = 0;
	struct oppo_p922x_ic *chip = p922x_chip;

	if (chip == NULL) {
		chg_err("p922x driver is not ready\n");
		return -ENODEV;
	}

	memset(string, 0, 8);

	len = snprintf(string, 8, "%d\n", chip->p922x_chg_status.freq_threshold);
	rc = simple_read_from_buffer(buf, count, ppos, string, len);

	return rc;
}
static ssize_t proc_wireless_rx_freq_write(struct file *file,
					   const char __user *buf,
					   size_t count, loff_t *lo)
{
	char string[16];
	int freq = 0;
	struct oppo_p922x_ic *chip = p922x_chip;

	if (chip == NULL) {
		chg_err("p922x driver is not ready\n");
		return -ENODEV;
	}

	memset(string, 0, 16);
	copy_from_user(string, buf, count);
	chg_err("buf = %s, len = %d\n", string, count);
	kstrtoint(string, 0, &freq);
	chg_err("set freq threshold to %d\n", freq);
	chip->p922x_chg_status.freq_threshold = freq;

	return count;
}

static const struct file_operations proc_wireless_rx_freq_ops = {
	.read = proc_wireless_rx_freq_read,
	.write = proc_wireless_rx_freq_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

#ifdef HW_TEST_EDITION
static ssize_t proc_wireless_w30w_time_read(struct file *file, char __user *buf,
					    size_t count, loff_t *ppos)
{
	uint8_t ret = 0;
#ifdef oppo_wireless
	char page[32];
	struct op_chg_chip *chip = g_op_chip;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

	snprintf(page, 32, "w30w_time:%d minutes\n", chip->w30w_time);
	ret = simple_read_from_buffer(buf, count, ppos, page, strlen(page));
#endif
	return ret;
}

static ssize_t proc_wireless_w30w_time_write(struct file *file,
					     const char __user *buf,
					     size_t count, loff_t *lo)
{
#ifdef oppo_wireless
	char buffer[4] = { 0 };
	int timeminutes = 0;
	struct op_chg_chip *chip = g_op_chip;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

	chg_err("%s: len[%d] start.\n", __func__, count);
	if (count > 3) {
		return -EFAULT;
	}

	if (copy_from_user(buffer, buf, count)) {
		chg_err("%s: error.\n", __func__);
		return -EFAULT;
	}
	chg_err("buffer=%s", buffer);
	kstrtoint(buffer, 0, &timeminutes);
	chg_err("set w30w_time = %dm", timeminutes);
	if (timeminutes >= 0 && timeminutes <= 60)
		chip->w30w_time = timeminutes;
	chip->w30w_work_started = false;
	chip->w30w_timeout = false;
#endif
	return count;
}

static const struct file_operations proc_wireless_w30w_time_ops = {
	.read = proc_wireless_w30w_time_read,
	.write = proc_wireless_w30w_time_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

#endif

static ssize_t proc_wireless_user_sleep_mode_read(struct file *file, char __user *buf,
					    size_t count, loff_t *ppos)
{
    uint8_t ret = 0;
    char page[10];
	struct oppo_p922x_ic *chip = p922x_chip;

    if(chip == NULL) {
        chg_err("%s: p922x_chip driver is not ready\n", __func__);
        return 0;
    }

    chg_err("chip->quiet_mode_need = %d.\n", chip->quiet_mode_need);
    sprintf(page, "%d", chip->quiet_mode_need);
    ret = simple_read_from_buffer(buf, count, ppos, page, strlen(page));

    return ret;
}

static ssize_t proc_wireless_user_sleep_mode_write(struct file *file, const char __user *buf,
				      size_t len, loff_t *lo)
{
    char buffer[2] = {0};
	struct oppo_p922x_ic *chip = p922x_chip;

	if (chip == NULL) {
		chg_err("%s: p922x driver is not ready\n", __func__);
		return 0;
	}

    chg_err("%s: len[%d] start.\n", __func__, len);
    if (len > 2) {
        return -EFAULT;
    }

    if (copy_from_user(buffer, buf, 2)) {
        chg_err("%s:  error.\n", __func__);
        return -EFAULT;
    }

    chg_err("%s: buffer[%s] .\n", __func__, buffer);
    if (buffer[0] == '0') {
        chg_err("%s:ftm_mode write 0.\n", __func__);
		chip->quiet_mode_need = 0;
    } else {
        chg_err("%s:ftm_mode write 1.\n", __func__);
		chip->quiet_mode_need = 1;
    }

    return len;
}

static const struct file_operations proc_wireless_user_sleep_mode_ops = {
	.read = proc_wireless_user_sleep_mode_read,
	.write = proc_wireless_user_sleep_mode_write,
	.open = simple_open,
	.owner = THIS_MODULE,
};

static int init_wireless_charge_proc(struct oppo_p922x_ic *chip)
{
	int ret = 0;
	struct proc_dir_entry *prEntry_da = NULL;
	struct proc_dir_entry *prEntry_tmp = NULL;

	prEntry_da = proc_mkdir("wireless", NULL);
	if (prEntry_da == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create wireless proc entry\n",
			  __func__);
	}

	prEntry_tmp = proc_create_data("voltage_rect", 0664, prEntry_da,
				       &proc_wireless_voltage_rect_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("rx_voltage", 0664, prEntry_da,
				       &proc_wireless_rx_voltage, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("current_out", 0664, prEntry_da,
				       &proc_wireless_current_out_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("ftm_mode", 0664, prEntry_da,
				       &proc_wireless_ftm_mode_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("enable_tx", 0664, prEntry_da,
				       &proc_wireless_tx_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("epp_or_bpp", 0664, prEntry_da,
				       &proc_wireless_epp_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("charge_pump_en", 0664, prEntry_da,
				       &proc_wireless_charge_pump_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("bat_mult", 0664, prEntry_da,
				       &proc_wireless_bat_mult_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("deviated", 0664, prEntry_da,
				       &proc_wireless_deviated_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("enable_rx", 0664, prEntry_da,
				       &proc_wireless_rx_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("upgrade_firmware", 0664, prEntry_da,
				       &proc_upgrade_firmware_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("rx_freq", 0664, prEntry_da,
				       &proc_wireless_rx_freq_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

	prEntry_tmp = proc_create_data("user_sleep_mode", 0664, prEntry_da,
				       &proc_wireless_user_sleep_mode_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}

#ifdef HW_TEST_EDITION
	prEntry_tmp = proc_create_data("w30w_time", 0664, prEntry_da,
				       &proc_wireless_w30w_time_ops, chip);
	if (prEntry_tmp == NULL) {
		ret = -ENOMEM;
		chg_debug("%s: Couldn't create proc entry, %d\n", __func__,
			  __LINE__);
	}
#endif
	return 0;
}


static enum power_supply_property p922x_wireless_props[] = {
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_REAL_TYPE,
	POWER_SUPPLY_PROP_TX_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_TX_CURRENT_NOW,
	POWER_SUPPLY_PROP_CP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CP_CURRENT_NOW,
	POWER_SUPPLY_PROP_WIRELESS_MODE,
	POWER_SUPPLY_PROP_WIRELESS_TYPE,
};

static int p922x_wireless_get_prop(struct power_supply *psy,
				   enum power_supply_property psp,
				   union power_supply_propval *val)
{
	struct oppo_p922x_ic *chip = p922x_chip;
//	int tmp;
	int rc = 0;
	///chg_err("%s: p922x_wireless_get_prop, %d, psp[%d]\n", __func__,  __LINE__, psp);

	switch (psp) {
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = chip->p922x_chg_status.charge_online;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		chg_err("%s: p922x_wireless_get_prop, %d\n", __func__, chip->p922x_chg_status.charge_online);
		val->intval = chip->p922x_chg_status.charge_online;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_RX)
			val->intval = p922x_wireless_get_vout();
		else
		#endif
		val->intval = p922x_wireless_get_vout();
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_RX)
			val->intval = chip->p922x_chg_status.target_vol;
		else
			#endif
			val->intval = 0;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:	
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_RX)
			val->intval = p922x_chip->p922x_chg_status.iout;
		else
			#endif
		val->intval = p922x_chip->p922x_chg_status.iout;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_CURRENT_MAX:
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_RX)
			val->intval = chip->p922x_chg_status.max_current;
		else
			#endif
			val->intval = 0;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_TX_VOLTAGE_NOW:
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_TX)
			val->intval = p922x_chip->p922x_chg_status.tx_vol;
		else
			#endif
			val->intval = 0;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_TX_CURRENT_NOW:
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_TX)
			val->intval = p922x_chip->p922x_chg_status.tx_curr;
		else
			#endif
			val->intval = 0;
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_CP_VOLTAGE_NOW:
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_RX) {
			if (exchgpump_bq == NULL)
				return -ENODEV;
			bq2597x_get_adc_data(exchgpump_bq, ADC_VBUS, &tmp);
			val->intval = tmp * 1000;
		} else {
			val->intval = 0;
		}
		#else
			val->intval = 0;
		#endif
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_CP_CURRENT_NOW:
#ifdef oppo_wireless
		if (chip->wireless_mode == WIRELESS_MODE_RX) {
			if (exchgpump_bq == NULL)
				return -ENODEV;
			bq2597x_get_adc_data(exchgpump_bq, ADC_IBUS, &tmp);
			val->intval = tmp * 1000;
		} else {
			val->intval = 0;
		}
		#else
			val->intval = 0;
		#endif
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_REAL_TYPE:
		switch (p922x_chip->p922x_chg_status.adapter_type) {
		case ADAPTER_TYPE_FASTCHAGE_VOOC:
		case ADAPTER_TYPE_FASTCHAGE_SVOOC:
			//if (chip->p922x_chg_status.deviation_check_done)
			val->intval = POWER_SUPPLY_TYPE_USB_DCP;
			//else
			//	val->intval = POWER_SUPPLY_TYPE_UNKNOWN;
			break;
		case ADAPTER_TYPE_USB:
			val->intval = POWER_SUPPLY_TYPE_USB;
			break;
		case ADAPTER_TYPE_NORMAL_CHARGE:
			val->intval = POWER_SUPPLY_TYPE_USB_DCP;
			break;
		case ADAPTER_TYPE_EPP:
			val->intval = POWER_SUPPLY_TYPE_USB_PD;//PD/QC
			break;
		default:
			val->intval = POWER_SUPPLY_TYPE_UNKNOWN;
			break;
		}
		chg_err("%s: p922x_wireless_get_prop[%d], adapter_type[%d]\n", __func__, val->intval, p922x_chip->p922x_chg_status.adapter_type);
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_WIRELESS_MODE:
#ifdef oppo_wireless
		val->strval = p922x_wireless_get_mode(chip);
#endif
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_WIRELESS_TYPE:
#ifdef oppo_wireless
		val->intval = chip->wireless_type;
#endif
		rc = 0;
		break;
	default:
		return -EINVAL;
	}
	if (rc < 0) {
		pr_debug("Couldn't get prop %d rc = %d\n", psp, rc);
		return -ENODATA;
	}
	return 0;
}

static int p922x_wireless_set_prop(struct power_supply *psy,
				   enum power_supply_property psp,
				   const union power_supply_propval *val)
{
#ifdef oppo_wireless
	struct oppo_chg_chip *chip = power_supply_get_drvdata(psy);
#endif
	int rc = 0;

	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
#ifdef oppo_wireless
		if (!chip->p922x_chg_status.fastchg_mode) {
			p922x_set_rx_target_voltage(chip, val->intval / 1000);
			rc = 0;
		} else {
			chg_err("is fastchg mode, can't set rx voltage\n");
			rc = -EINVAL;
		}
		#endif
		break;
	case POWER_SUPPLY_PROP_CURRENT_MAX:
#ifdef oppo_wireless
		chip->p922x_chg_status.max_current = val->intval / 1000;
		vote(chip->wlcs_fcc_votable, MAX_VOTER, true, val->intval);
		#endif
		rc = 0;
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:
#ifdef oppo_wireless
		vote(chip->wlcs_fcc_votable, USER_VOTER, true, val->intval);
#endif
		rc = 0;
		break;
	default:
		chg_err("set prop %d is not supported\n", psp);
		rc = -EINVAL;
		break;
	}

	return rc;
}

static int p922x_wireless_prop_is_writeable(struct power_supply *psy,
					    enum power_supply_property psp)
{
	int rc;

	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
	case POWER_SUPPLY_PROP_CURRENT_MAX:
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		rc = 1;
		break;
	default:
		rc = 0;
		break;
	}

	return rc;
}


static const struct power_supply_desc wireless_psy_desc = {
	.name = "wireless",
	.type = POWER_SUPPLY_TYPE_WIRELESS,
	.properties = p922x_wireless_props,
	.num_properties = ARRAY_SIZE(p922x_wireless_props),
	.get_property = p922x_wireless_get_prop,
	.set_property = p922x_wireless_set_prop,
	.property_is_writeable = p922x_wireless_prop_is_writeable,
};

static int p922x_init_wireless_psy(struct oppo_p922x_ic *chip)
{
	struct power_supply_config wireless_cfg = {};

	wireless_cfg.drv_data = chip;
	wireless_cfg.of_node = chip->dev->of_node;
	chip->wireless_psy = devm_power_supply_register(
		chip->dev, &wireless_psy_desc, &wireless_cfg);
	if (IS_ERR(chip->wireless_psy)) {
		chg_err("Couldn't register wireless power supply\n");
		return PTR_ERR(chip->wireless_psy);
	}

	return 0;
}


static int p922x_driver_probe(struct i2c_client *client, const struct i2c_device_id *id) 
{
	struct oppo_p922x_ic	*chip;
	int rc = 0;

	chg_debug( " call \n");

	if (oppo_chg_check_chip_is_null() == true) {
		chg_debug( " g_oppo_chg chip is null, probe again \n");
		return -EPROBE_DEFER;
	}

	chip = devm_kzalloc(&client->dev,
		sizeof(struct oppo_p922x_ic), GFP_KERNEL);
	if (!chip) {
		chg_err(" kzalloc() failed\n");
		return -ENOMEM;
	}
	
	chip->client = client;
	chip->dev = &client->dev;

	p922x_idt_gpio_init(chip);

#ifdef DEBUG_BY_FILE_OPS
	init_p922x_add_log();
	init_p922x_data_log();
#endif
	
	p922x_chip = chip;

	chip->p922x_chg_status.check_fw_update = false;
	p922x_chip->p922x_chg_status.wpc_dischg_status = WPC_DISCHG_STATUS_OFF;

	p922x_reset_variables(chip);

	chip->p922x_chg_status.ftm_mode = false;

	INIT_DELAYED_WORK(&chip->idt_event_int_work, p922x_idt_event_int_func);
	INIT_DELAYED_WORK(&chip->idt_connect_int_work, p922x_idt_connect_int_func);
	INIT_DELAYED_WORK(&chip->idt_dischg_work, p922x_idt_dischg_work);


	INIT_DELAYED_WORK(&chip->p922x_task_work, p922x_task_work_process);
	INIT_DELAYED_WORK(&chip->p922x_CEP_work, p922x_CEP_work_process);
	INIT_DELAYED_WORK(&chip->p922x_update_work, p922x_update_work_process);

	schedule_delayed_work(&chip->p922x_update_work, P922X_UPDATE_INTERVAL);

    p922x_idt_connect_shedule_work();
	rc = init_wireless_charge_proc(chip);
	if (rc < 0) {
		chg_err("Create wireless charge proc error.");
		//goto err_1;
	}
	p922x_init_wireless_psy(chip);

	chg_debug( " call end\n");

	return 0;                                                                                       

}


static struct i2c_driver p922x_i2c_driver;

static int p922x_driver_remove(struct i2c_client *client)
{    
	return 0;
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
static int p922x_pm_resume(struct device *dev)
{
	return 0;
}

static int p922x_pm_suspend(struct device *dev)
{
	return 0;
}

static const struct dev_pm_ops p922x_pm_ops = {
	.resume		= p922x_pm_resume,
	.suspend		= p922x_pm_suspend,
};
#else
static int p922x_resume(struct i2c_client *client)
{	
	return 0;
}

static int p922x_suspend(struct i2c_client *client, pm_message_t mesg)
{
	return 0;
}
#endif

static void p922x_reset(struct i2c_client *client)
{
    int wpc_con_level = 0;
    int wait_wpc_disconn_cnt = 0;

	p922x_set_vbat_en_val(1);
	
    wpc_con_level = p922x_get_idt_con_val();
    if(wpc_con_level == 1) {
        //p922x_set_vbat_en_val(1);
        msleep(100);

        while(wait_wpc_disconn_cnt < 10) {
            wpc_con_level = p922x_get_idt_con_val();
            if (wpc_con_level == 0) {
                break;
            }
            msleep(150);
            wait_wpc_disconn_cnt++;
        }
        chargepump_disable();
    }
	return;
}

/**********************************************************
  *
  *   [platform_driver API] 
  *
  *********************************************************/

static const struct of_device_id p922x_match[] = {
	{ .compatible = "oppo,p922x-charger"},
	{ },
};

static const struct i2c_device_id p922x_id[] = {
	{"p922x-charger", 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, p922x_id);


static struct i2c_driver p922x_i2c_driver = {
	.driver		= {
		.name = "p922x-charger",
		.owner	= THIS_MODULE,
		.of_match_table = p922x_match,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
					.pm 	= &p922x_pm_ops,
#endif

	},
	.probe		= p922x_driver_probe,
	.remove		= p922x_driver_remove,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 4, 0))
	.resume		= p922x_resume,
	.suspend	= p922x_suspend,
#endif
	.shutdown	= p922x_reset,
	.id_table	= p922x_id,
};


module_i2c_driver(p922x_i2c_driver);
MODULE_DESCRIPTION("Driver for p922x charger chip");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("i2c:p922x-charger");
