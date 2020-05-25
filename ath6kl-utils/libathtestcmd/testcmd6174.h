/*
 * Copyright (c) 2014-2019 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
*/

/*
 * 2014-2016 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

/*
 * Copyright (c) 2006 Atheros Communications Inc.
 * All rights reserved.
 *
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
//
// <summary>
// Wifi driver for AR6002
// </summary>
//
 *
 */
#ifndef _TESTCMD6174_H
#define _TESTCMD6174_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ATH_MAC_LEN
#define ATH_MAC_LEN 6
#endif

#ifndef MAX_DATA_LEN
#define MAX_DATA_LEN 74
#endif

#ifndef MAX_RU_INDEX
#define MAX_RU_INDEX 8
#endif

#ifndef MAX_CLI_VAL
#define MAX_CLI_VAL 5000
#endif

typedef struct __attribute__((__packed__)) {
	uint8_t version;                        // Make it 684 bytes
	uint8_t bandwidth;                      // 1 - 74 (depends on RU allocation indices)
	uint8_t AllocIndx[MAX_RU_INDEX];        // 1 - 8 RU allocation indices (depends on bandwidth)
	uint8_t AllocRUs[MAX_RU_INDEX];         // 1 - 8 RU allocation indices (depends on bandwidth)
	uint8_t MCS[MAX_DATA_LEN];              // MCS 0-11
	uint8_t FEC[MAX_DATA_LEN];              // LDPC or BCC
	uint8_t NSS[MAX_DATA_LEN];              // Spatial streams 1-8 (NA for HK defaults to 1), default = 1
	uint8_t boost[MAX_DATA_LEN];            // Default = 0, TonePlanWizard float * 8
	uint8_t ID[MAX_DATA_LEN];               // 1..255 simulated AID
	// Newer fields
	uint16_t payloadLen[MAX_DATA_LEN];      // Default = 1500, V2 only
	uint16_t packetExt[MAX_DATA_LEN];
	uint16_t nHE_LTF_Symbol[MAX_DATA_LEN];
	uint8_t ltfGI[MAX_DATA_LEN];
	uint8_t Rsvd1[MAX_DATA_LEN];            // Reserved for future use, V2 only
	uint8_t Rsvd2[MAX_DATA_LEN];            // Reserved for future use, V2 only
} TLV2_OFDMATONEPLAN_PARMS;

typedef struct _cmdParmeter{
	int isTxStart;
	int isRxStart;

	int freq;
	int bandWidth;
	int rate;
	uint8_t bssid[ATH_MAC_LEN];

	double txPwr;
	int32_t tpcm;
	int32_t paConfig;
	int32_t gainIdx;
	int32_t dacGain;
	int32_t numPkt;
	int32_t agg;
	int32_t pktLen0;
	uint32_t broadcast;
	uint8_t txStation[ATH_MAC_LEN];
	uint8_t rxStation[ATH_MAC_LEN];
	int aifs;

	uint32_t rxPkt;
	int32_t  rxRssi;
	uint32_t rxCrcError;
	uint32_t rxSecError;
	uint32_t expectedPkts;
	uint8_t addr[ATH_MAC_LEN];

	int longpreamble;
	int shortguard;
	uint32_t antenna;
	uint32_t chain;
	uint32_t ani;

	int errCode;
	char errString[256];

	int pwr_backoff;
	int stbc;
	int ldpc;
	int wmode;
	int scpc_cal;

	/* NART Command */
	/* buffer to save NART response data */
	uint8_t nart_rsp_buf[2048];
	uint32_t phyid;
	int freq2;
	uint8_t bt_addr[ATH_MAC_LEN];
	uint32_t sar_index;
	uint32_t sar_chain;
	uint32_t sar_cck2glimit;
	uint32_t sar_ofdm2glimit;
	uint32_t sar_ofdm5glimit;
	uint32_t enable_dpd_flag;
	uint8_t rateBw;
	uint8_t nss;
	uint8_t gi;
	uint8_t ofdmadcm;
	uint8_t ofdmappdutype;
	uint8_t ofdmalinkdir;
	uint8_t fecpad;
	uint8_t ldpc_exsymbol;
	uint8_t duty_cycle;
	uint32_t regval;
	TLV2_OFDMATONEPLAN_PARMS toneplan;
}_CMD_PARM;


int qca6174ApiInit();
void qca6174init();
void qca6174ApiCleanup(void);
void qca6174ChannelSet(int channel);
void qca6174FreqSet(uint32_t freq);
void qca6174RateSet(int rate);
void qca6174TxPowerSet(char *);

/** @breif Enable long preamble */
void qca6174SetLongPreamble(int enable);

/** @breif Set the interval between frames in aifs number
 *  @param slot aifs slot 0->SIFS, 1->PIFS, 2->DIFS, ... 253 */
void qca6174SetAifsNum(int slot);
void qca6174SetAntenna(int antenna);
void qca6174SetChain(int chain);
void qca6174SetBssid(char *mac);
void qca6174SetTxStation(char *mac);
void qca6174SetRxStation(char *mac);
void qca6174SetAddr(char *mac);
void qca6174TxPcmSet(int txpwr);
void qca6174SetPaCfg(int val);
void qca6174SetDacGain(int val);
void qca6174SetGainIdx(int val);
void qca6174SetNumPkt(int val);
void qca6174SetAgg(int val);
void qca6174SetLdpc(int val);
void qca6174SetStbc(int val);
void qca6174SetWlanMode(int val);
void qca6174SetLPreamble();

void qca6174TxPacketSizeSet(int size);
void qca6174ShortGuardSet(int enable);
void qca6174SetBandWidth(int width);

int qca6174TxSineStart(void);
int qca6174Tx99Start(void);
int qca6174TxFrameStart(void);
int qca6174TxCWStart(void);
int qca6174TxStop(void);
int qca6174Tx100Start(void);
int qca6174TxCLPCPktStart(void);
#ifdef NOT_USE_CLITLVCMD_IF_MODULE
int qca6174RxPacketStart(void);
#else
int qca6174RxPacketStart(char *rx_type);
#endif
int qca6174RxPacketStop(void);
uint32_t qca6174RxGetErrorFrameNum(void);
uint32_t qca6174RxGetGoodFrameNum(void);
const char *qca6174GetErrorString(void);
void qca6174_compute_checksum(uint8_t *ptr_6320_eeprom);
int qca6174_clear_scpc_done();
void qca6174_enable_scpc_cal(int val);
int qca6174_get_scpc_cal();
int qca6174_eeprom_block_read(void* buf, uint32_t offset, uint32_t length);
int qca6174_eeprom_write_item(void* buf, uint32_t offset, uint8_t length);

void qca6174Freq2Set(uint32_t freq);
void qca6174SetPhyid(int);
void qca6174SetDBS(char *);
void qca6174SetBssid(char *);
void qca6174SetTxStation(char *);
void qca6174SetRxStation(char *);
void qca6174SetAddr(char *);
void qca6174SetBTAddr(char *);
void qca6174CmdSAR(uint32_t );
void qca6174CmdSARIndex(char *);
void qca6174CmdSARChain(uint32_t );
void qca6174CmdSARCCK2gLimit(char *);
void qca6174CmdSAROFDM2gLimit(char *);
void qca6174CmdSAROFDM5gLimit(char *);
void qca6174CmdFlagDPDEnable();
void qca6174CmdSETREGDMN(char *val);
int qca6174CmdDPDStatus();
void qca6174CmdRateBW(uint8_t val);
void qca6174CmdNSS(uint8_t val);
void qca6174CmdGI(uint8_t val);
void qca6174CmdADCM(uint8_t val);
void qca6174CmdPPDUTYPE(uint8_t val);
void qca6174CmdLINKDIR(uint8_t val);
int qca6174Cmd_TONEPLAN(char *val);
void qca6174Cmd_PREFECPAD(uint8_t val);
void qca6174Cmd_LDPCEXTRASYMBOL(uint8_t val);
void qca6174Cmd_DUTYCYCLE(uint8_t val);
int qca6174Cmd_OFDMAUL_TX();
void qca6174CmdRegWriteValue(uint32_t val);
int qca6174CmdRegWrite(uint32_t reg);
int qca6174CmdRegRead(uint32_t reg);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

