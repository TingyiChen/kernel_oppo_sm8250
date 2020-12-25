/************************************************************************************
** File:  oppo_chargepump.c
** VENDOR_EDIT
** Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
** 
** Description: 
**   
** 
** Version: 1.0
** Date created: 21:03:46,09/04/2019
** Author: Lin Shangbo
** 
** --------------------------- Revision History: ------------------------------------------------------------
* <version>       <date>        <author>              			<desc>
* Revision 1.0    2019-04-09    Lin Shangbo    		Created for new charger
************************************************************************************************************/


#ifndef __OPPO_CHARGEPUMP_H__
#define __OPPO_CHARGEPUMP_H__

#define OP20A

struct chip_chargepump {
	struct i2c_client		*client;
	struct device			*dev;
	struct pinctrl			*pinctrl;   
	int						chargepump_en_gpio;
	struct pinctrl_state	*chargepump_en_active;
	struct pinctrl_state	*chargepump_en_sleep;
	struct pinctrl_state	*chargepump_en_default;
	atomic_t				chargepump_suspended;
};

extern int chargepump_set_for_EPP(void);
extern int chargepump_enable(void);
extern int chargepump_set_for_otg(char enable);
extern int chargepump_set_for_LDO(void);
extern int chargepump_enable_voltage_diff_detect(void);
extern int chargepump_enable_watchdog(void);
extern int chargepump_kick_watchdog(void);
extern int chargepump_disable(void);
extern int chargepump_get_chargepump_en_val(void);
extern void chargepump_print_log(void);
#endif
