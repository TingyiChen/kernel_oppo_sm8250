/**********************************************************************************
* Copyright (c)  2008-2015  Guangdong OPPO Mobile Comm Corp., Ltd
* VENDOR_EDIT
* Description: Charger IC management module for charger system framework.
*             Manage all charger IC and define abstarct function flow.
* Version   : 1.0
* Date      : 2015-05-22
* Author    : fanhui@PhoneSW.BSP
*           : Fanhong.Kong@ProDrv.CHG
* ------------------------------ Revision History: --------------------------------
* <version>           <date>                <author>                               <desc>
* Revision 1.0        2015-05-22        fanhui@PhoneSW.BSP                 Created for new architecture
* Revision 1.0        2015-05-22        Fanhong.Kong@ProDrv.CHG            Created for new architecture
* Revision 2.0        2018-04-14        Fanhong.Kong@ProDrv.CHG            Upgrade for SVOOC
***********************************************************************************/

#ifndef _OPPO_VOOC_H_
#define _OPPO_VOOC_H_

#include <linux/workqueue.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))
#include <linux/wakelock.h>
#endif
#include <linux/timer.h>
#include <linux/slab.h>
#include <soc/oppo/device_info.h>
#include <soc/oppo/oppo_project.h>
#include <linux/firmware.h>

#define OPPO_VOOC_MCU_HWID_STM8S	0
#define OPPO_VOOC_MCU_HWID_N76E		1
#define OPPO_VOOC_ASIC_HWID_RK826	2
#define OPPO_VOOC_ASIC_HWID_OP10	3

enum {
	VOOC_CHARGER_MODE,
	HEADPHONE_MODE,
	NORMAL_CHARGER_MODE,
};

enum {
	FW_ERROR_DATA_MODE,
	FW_NO_CHECK_MODE,
	FW_CHECK_MODE,
};

enum {
	VOOC_MAX_CURRENT_NO_LIMIT,
	VOOC_MAX_CURRENT_LIMIT_2A,
	VOOC_MAX_CURRENT_LIMIT_OTHER,
};
enum {
	FASTCHG_CHARGER_TYPE_UNKOWN,
	PORTABLE_PIKAQIU_1 = 0x31,
	PORTABLE_PIKAQIU_2 = 0x32,
	PORTABLE_50W = 0x33,
	PORTABLE_20W_1 = 0X34,
	PORTABLE_20W_2 = 0x35,
	PORTABLE_20W_3 = 0x36,
};

enum {
	BAT_TEMP_NATURAL = 0,
	BAT_TEMP_HIGH0,
	BAT_TEMP_HIGH1,
	BAT_TEMP_HIGH2,
	BAT_TEMP_HIGH3,
	BAT_TEMP_HIGH4,
	BAT_TEMP_HIGH5,
	BAT_TEMP_LOW0,
	BAT_TEMP_LOW1,
	BAT_TEMP_LOW2,
	BAT_TEMP_LITTLE_COOL,
	BAT_TEMP_COOL,
	BAT_TEMP_EXIT,
	BAT_TEMP_OVER_HIGH_EXIT,
	BAT_TEMP_OVER_LOW_EXIT,
};


struct vooc_gpio_control {
	int switch1_gpio;
	int switch1_ctr1_gpio;
	int switch2_gpio;
	int switch3_gpio;
	int reset_gpio;
	int clock_gpio;
	int data_gpio;
	int vooc_mcu_id_gpio;
	int data_irq;
	struct pinctrl *pinctrl;

	struct pinctrl_state *gpio_switch1_act_switch2_act;
	struct pinctrl_state *gpio_switch1_sleep_switch2_sleep;
	struct pinctrl_state *gpio_switch1_act_switch2_sleep;
	struct pinctrl_state *gpio_switch1_sleep_switch2_act;
	struct pinctrl_state *gpio_switch1_ctr1_act;
	struct pinctrl_state *gpio_switch1_ctr1_sleep;

	struct pinctrl_state *gpio_clock_active;
	struct pinctrl_state *gpio_clock_sleep;
	struct pinctrl_state *gpio_data_active;
	struct pinctrl_state *gpio_data_sleep;
	struct pinctrl_state *gpio_reset_active;
	struct pinctrl_state *gpio_reset_sleep;
	struct pinctrl_state *gpio_vooc_mcu_id_default;
};

struct oppo_vooc_chip {
	struct i2c_client *client;
	struct device *dev;
	struct oppo_vooc_operations *vops;
	struct vooc_gpio_control vooc_gpio;
	struct delayed_work fw_update_work;
	struct delayed_work fastchg_work;
	struct delayed_work delay_reset_mcu_work;
	struct delayed_work check_charger_out_work;
	struct work_struct vooc_watchdog_work;
	struct timer_list watchdog;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 9, 0))
	struct wake_lock vooc_wake_lock;
#else
	struct wakeup_source *vooc_ws;
#endif

	struct power_supply *batt_psy;
	int pcb_version;
	bool allow_reading;
	bool fastchg_started;
	bool fastchg_ing;
	bool fastchg_allow;
	bool fastchg_to_normal;
	bool fastchg_to_warm;
	bool fastchg_low_temp_full;
	bool btb_temp_over;
	bool fastchg_dummy_started;
	bool need_to_up;
	bool have_updated;
	bool mcu_update_ing;
	bool mcu_boot_by_gpio;
	const unsigned char *firmware_data;
	unsigned int fw_data_count;
	int fw_mcu_version;
	int fw_data_version;
	int adapter_update_real;
	int adapter_update_report;
	int dpdm_switch_mode;
	bool support_vooc_by_normal_charger_path;
/* wenbin.liu@BSP.CHG.Vooc, 2016/10/20*/
/* Add for vooc batt 4.40*/
	bool batt_type_4400mv;
	bool vooc_fw_check;
	int vooc_fw_type;
//PengNan@BSP.CHG.Vooc, 2018/02/28, add for vooc fw update.
	int fw_update_flag;
	struct manufacture_info manufacture_info;
	bool vooc_fw_update_newmethod;
	char *fw_path;
	struct mutex pinctrl_mutex;
	int vooc_little_cool_temp;
	int vooc_cool_temp;
	int vooc_little_cool_to_normal_temp;
	int vooc_normal_to_little_cool_current;
	int vooc_low_temp;
	int vooc_high_temp;
	int vooc_low_soc;
	int vooc_high_soc;
	int fast_chg_type;
	bool disable_adapter_output;// 0--vooc adapter output normal,  1--disable vooc adapter output
	int set_vooc_current_limit;///0--no limit;  1--max current limit 2A
	bool vooc_multistep_adjust_current_support;
	int vooc_multistep_initial_batt_temp;
	int vooc_strategy_normal_current;
	int vooc_strategy1_batt_high_temp0;
	int vooc_strategy1_batt_high_temp1;
	int vooc_strategy1_batt_high_temp2;
	int vooc_strategy1_batt_low_temp2;
	int vooc_strategy1_batt_low_temp1;
	int vooc_strategy1_batt_low_temp0;
	int vooc_strategy1_high_current0;
	int vooc_strategy1_high_current1;
	int vooc_strategy1_high_current2;
	int vooc_strategy1_low_current2;
	int vooc_strategy1_low_current1;
	int vooc_strategy1_low_current0;
	int vooc_strategy2_batt_up_temp1;
	int vooc_strategy2_batt_up_down_temp2;
	int vooc_strategy2_batt_up_temp3;
	int vooc_strategy2_batt_up_down_temp4;
	int vooc_strategy2_batt_up_temp5;
	int vooc_strategy2_batt_up_temp6;
	int vooc_strategy2_high0_current;
	int vooc_strategy2_high1_current;
	int vooc_strategy2_high2_current;
	int vooc_strategy2_high3_current;
	int fastchg_batt_temp_status;
	int vooc_batt_over_high_temp;
	int vooc_batt_over_low_temp;
	int vooc_over_high_or_low_current;
	int vooc_strategy_change_count;
};

#define MAX_FW_NAME_LENGTH	60
#define MAX_DEVICE_VERSION_LENGTH 16
#define MAX_DEVICE_MANU_LENGTH    60
struct oppo_vooc_operations {
	int (*fw_update)(struct oppo_vooc_chip *chip);
	int (*fw_check_then_recover)(struct oppo_vooc_chip *chip);
	void (*eint_regist)(struct oppo_vooc_chip *chip);
	void (*eint_unregist)(struct oppo_vooc_chip *chip);
	void (*set_data_active)(struct oppo_vooc_chip *chip);
	void (*set_data_sleep)(struct oppo_vooc_chip *chip);
	void (*set_clock_active)(struct oppo_vooc_chip *chip);
	void (*set_clock_sleep)(struct oppo_vooc_chip *chip);
	void (*set_switch_mode)(struct oppo_vooc_chip *chip, int mode);
	int (*get_gpio_ap_data)(struct oppo_vooc_chip *chip);
	int (*read_ap_data)(struct oppo_vooc_chip *chip);
	void (*reply_mcu_data)(struct oppo_vooc_chip *chip, int ret_info, int device_type);
	void (*reply_mcu_data_4bits)(struct oppo_vooc_chip *chip,
		int ret_info, int device_type);
	void (*reset_fastchg_after_usbout)(struct oppo_vooc_chip *chip);
	void (*switch_fast_chg)(struct oppo_vooc_chip *chip);
	void (*reset_mcu)(struct oppo_vooc_chip *chip);
	void (*set_vooc_chargerid_switch_val)(struct oppo_vooc_chip *chip, int value);
	bool (*is_power_off_charging)(struct oppo_vooc_chip *chip);
	int (*get_reset_gpio_val)(struct oppo_vooc_chip *chip);
	int (*get_switch_gpio_val)(struct oppo_vooc_chip *chip);
	int (*get_ap_clk_gpio_val)(struct oppo_vooc_chip *chip);
	int (*get_fw_version)(struct oppo_vooc_chip *chip);
	int (*get_clk_gpio_num)(struct oppo_vooc_chip *chip);
	int (*get_data_gpio_num)(struct oppo_vooc_chip *chip);
};

void oppo_vooc_init(struct oppo_vooc_chip *chip);
void oppo_vooc_shedule_fastchg_work(void);
void oppo_vooc_read_fw_version_init(struct oppo_vooc_chip *chip);
void oppo_vooc_fw_update_work_init(struct oppo_vooc_chip *chip);
bool oppo_vooc_wake_fastchg_work(struct oppo_vooc_chip *chip);
void oppo_vooc_print_log(void);
void oppo_vooc_switch_mode(int mode);
bool oppo_vooc_get_allow_reading(void);
bool oppo_vooc_get_fastchg_started(void);
bool oppo_vooc_get_fastchg_ing(void);
bool oppo_vooc_get_fastchg_allow(void);
void oppo_vooc_set_fastchg_allow(int enable);
bool oppo_vooc_get_fastchg_to_normal(void);
void oppo_vooc_set_fastchg_to_normal_false(void);
bool oppo_vooc_get_fastchg_to_warm(void);
void oppo_vooc_set_fastchg_to_warm_false(void);
void oppo_vooc_set_fastchg_type_unknow(void);
bool oppo_vooc_get_fastchg_low_temp_full(void);
void oppo_vooc_set_fastchg_low_temp_full_false(void);
bool oppo_vooc_get_vooc_multistep_adjust_current_support(void);
bool oppo_vooc_get_fastchg_dummy_started(void);
void oppo_vooc_set_fastchg_dummy_started_false(void);
int oppo_vooc_get_adapter_update_status(void);
int oppo_vooc_get_adapter_update_real_status(void);
bool oppo_vooc_get_btb_temp_over(void);
void oppo_vooc_reset_fastchg_after_usbout(void);
void oppo_vooc_switch_fast_chg(void);
void oppo_vooc_reset_mcu(void);
void oppo_vooc_set_vooc_chargerid_switch_val(int value);
void oppo_vooc_set_ap_clk_high(void);
int oppo_vooc_get_vooc_switch_val(void);
bool oppo_vooc_check_chip_is_null(void);
void oppo_vooc_battery_update(void);

int oppo_vooc_get_uart_tx(void);
int oppo_vooc_get_uart_rx(void);
void oppo_vooc_uart_init(void);
void oppo_vooc_uart_reset(void);
void oppo_vooc_set_adapter_update_real_status(int real);
void oppo_vooc_set_adapter_update_report_status(int report);
int oppo_vooc_get_fast_chg_type(void);
void oppo_vooc_set_disable_adapter_output(bool disable);
void oppo_vooc_set_vooc_max_current_limit(int current_level);

extern int get_vooc_mcu_type(struct oppo_vooc_chip *chip);
#endif /* _OPPO_VOOC_H */
