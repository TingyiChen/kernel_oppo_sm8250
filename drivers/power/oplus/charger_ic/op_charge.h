/**********************************************************************************
* Copyright (c)  2008-2019  Guangdong OPPO Mobile Comm Corp., Ltd
* OPLUS_FEATURE_CHG_BASIC
* Description: OPPO Charge Module Device Tree
* 
* Version: 1.0
* Date: 2019-06-10
* ------------------------------ Revision History: --------------------------------
* <version>           <date>                <author>                            <desc>
*                   2019-07-03            Yichun.Chen                     add common function
***********************************************************************************/
#ifndef __OP_CHARGE_H__
#define __OP_CHARGE_H__

struct ntc_table {
	int resistance;		/* ohm */
	int temperature;	/* 0.1*celsius */
};

struct usb_temp {
	int temperature;	/* 0.1*celsius */
	int interval;		/* ms */
};

#define ADC_SAMPLE_COUNT		5
#define ADC_SAMPLE_INTERVAL		10

#define PULL_UP_VOLTAGE_1800MV		1800	/* 1800mV */
#define PULL_UP_RESISTANCE_51KOHM	51000	/* 51kohm */

#define DETECT_INTERVAL_50MS		50	/* detect interval 50ms */
#define DETECT_INTERVAL_300MS		300	/* detect interval 300ms */
#define VALID_LOW_TEMPERATURE		200	/* 20C - 100C */
#define VALID_HIGH_TEMPERATURE		1000	/* 20C - 100C */
#define CRITICAL_TEMPERATURE		570	/* 57C */
#define CRITICAL_USBTEMP_ABOVE_BATTEMP	100	/* usb_temp - bat_temp >= 10C */
#define CRITICAL_RISE_TEMPERATURE	30	/* rise 3Celsius in 1500ms */
#define CRITICAL_RISE_INTERVAL		1500	/* rise 3Celsius in 1500ms */
#define TEMPERATURE_LIST_LENGTH		30	/* 1.5s / 50ms */

/*func:				check if dischg_enable
*temperature1 temperature2:	usb_temp (0.1*Celsius)
*critical_rise1 critical_rise2:	if rise 3Cesius in 1500ms
*bat_temp:			bat_temp (0.1*Celsius)
*dischg_enable:			dischg_enable		*/
#define check_dischg_enable(temperature1, temperature2, critical_rise1, critical_rise2, bat_temp, dischg_enable)	\
	do {														\
		dischg_enable = false;											\
		if (temperature1 >= VALID_LOW_TEMPERATURE && temperature1 <= VALID_HIGH_TEMPERATURE) {			\
			if (temperature1 >= CRITICAL_TEMPERATURE)							\
				dischg_enable = true;									\
			if (temperature1 - bat_temp >= CRITICAL_USBTEMP_ABOVE_BATTEMP && critical_rise1 == true)	\
				dischg_enable = true;									\
		}													\
		if (temperature2 >= VALID_LOW_TEMPERATURE && temperature2 <= VALID_HIGH_TEMPERATURE) {			\
			if (temperature2 >= CRITICAL_TEMPERATURE)							\
				dischg_enable = true;									\
			if (temperature2 - bat_temp >= CRITICAL_USBTEMP_ABOVE_BATTEMP && critical_rise2 == true)	\
				dischg_enable = true;									\
		}													\
	} while (0)

/*temp1 temp2:	usb_temp (0.1*Celsius)
*time:		thread interval (ms)
*i:		index of ntc_table	*/
#define add_temperature_list(temp1, temp2, time, i)							\
	do {												\
		for (i = TEMPERATURE_LIST_LENGTH - 1; i > 0; i --) {					\
			temperature_list1[i].temperature = temperature_list1[i - 1].temperature;	\
			temperature_list1[i].interval = temperature_list1[i - 1].interval;		\
			temperature_list2[i].temperature = temperature_list2[i - 1].temperature;	\
			temperature_list2[i].interval = temperature_list2[i - 1].interval;		\
		}											\
		temperature_list1[0].temperature = temp1;						\
		temperature_list1[0].interval = time;							\
		temperature_list2[0].temperature = temp2;						\
		temperature_list2[0].interval = time;							\
	} while (0)

/* check if rise 3Cesius within 1500ms */
#define check_temperature_list(list, total_time, critical_rise, i)					\
	do {												\
		critical_rise = false;									\
		total_time = 0;										\
		for (i = 0; i < TEMPERATURE_LIST_LENGTH; i ++) {					\
			total_time += list[i].interval;							\
			if (list[i].temperature == 0 && list[i].interval == 0)				\
				break;									\
			if (total_time - list[i].interval >= CRITICAL_RISE_INTERVAL)			\
				break;									\
			if (list[0].temperature - list[i].temperature >= CRITICAL_RISE_TEMPERATURE) {	\
				critical_rise = true;							\
				break;									\
			}										\
		}											\
	} while (0)

#define clear_temperature_list(i)					\
	do {								\
		for (i = 0; i < TEMPERATURE_LIST_LENGTH; i ++) {	\
			temperature_list1[i].temperature = 0;		\
			temperature_list1[i].interval = 0;		\
			temperature_list2[i].temperature = 0;		\
			temperature_list2[i].interval = 0;		\
		}							\
	} while (0)

/*R:	NTC resistance (ohm)
*T:	NTC temperature (0.1*Celsius)
*L:	ARRAY_SIZE(ntc_table)
*i:	index of ntc_table
*table:	ntc_table		*/
#define resistance_convert_temperature(R, T, i, table)			\
	do {								\
		for (i = 0; i < ARRAY_SIZE(table); i ++)		\
			if (table[i].resistance <= R)			\
				break;					\
		if (i == 0)						\
			T = table[0].temperature;			\
		else if (i == ARRAY_SIZE(table))			\
			T = table[ARRAY_SIZE(table) - 1].temperature;	\
		else						\
			T = table[i].temperature * (R - table[i - 1].resistance) / (table[i].resistance - table[i - 1].resistance) +	\
				table[i - 1].temperature * (table[i].resistance - R) / (table[i].resistance - table[i - 1].resistance);	\
	} while (0)

/*vadc:			usb_temp_adc voltage (mV)
*pull_up_voltage:	pull_up_voltage (mV)
*pull_up_resistance:	pull_up_resistance (ohm)	*/
#define voltage_convert_resistance(vadc, pull_up_voltage, pull_up_resistance)	\
	(pull_up_resistance / (pull_up_voltage - vadc) * vadc)

extern struct usb_temp temperature_list1[TEMPERATURE_LIST_LENGTH];	/* length = 1.5s / 50ms */
extern struct usb_temp temperature_list2[TEMPERATURE_LIST_LENGTH];	/* length = 1.5s / 50ms */

/* ntc1 100Kohm when 25c */
extern struct ntc_table ntc_table_100K[166];
/* ntc2 10Kohm when 25c */
extern struct ntc_table ntc_table_10K[1];
extern int con_temp_855[109];
extern int con_volt_855[106];
extern int con_temp_7250[141];
extern int con_volt_7250[141];
extern int con_volt_7250_svooc[141];
extern int con_volt[166];
extern int con_temp[166];
#endif /* __OP_CHARGE_H__ */
