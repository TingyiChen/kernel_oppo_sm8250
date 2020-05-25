/*
 * Copyright (c) 2016 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

#if !defined(__TLV_CMDENCODER_H)
#define __TLV_CMDENCODER_H

typedef enum {
    TLV_ENCODER_OK              = 0,
    TLV_ENCODER_NOT_ON,
    TLV_ENCODER_CONSTRUCTION_IN_PROGRESS,
    TLV_ENCODER_INVALID_CMD,
    TLV_ENCODER_INVALID_PARM,
    TLV_ENCODER_BUFFER_OVERFLOW,
    TLV_ENCODER_SEGMENT_SIZE_TOO_BIG,
    TLV_ENCODER_NO_MEMORY_FOR_EXTENDED,
} TLV_ENCODER_ENUM;

#endif /*#if !defined(__TLV_CMDENCODER_H)*/

