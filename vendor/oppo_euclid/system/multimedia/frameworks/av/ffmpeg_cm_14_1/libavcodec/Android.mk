#
# Copyright (C) 2013 The Android-x86 Open Source Project
#
# Licensed under the GNU General Public License Version 2 or later.
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.gnu.org/licenses/gpl.html
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
FFMPEG_MULTILIB := 32
include $(LOCAL_PATH)/../android/build.mk

LOCAL_C_INCLUDES +=		\
	external/zlib

LOCAL_SHARED_LIBRARIES +=	\
	libz \
	libavutil \
	libswresample

ifneq ($(ARCH_ARM_HAVE_NEON),)
  LOCAL_SRC_FILES += neon/mpegvideo.c
endif

LOCAL_MULTILIB := $(FFMPEG_MULTILIB)
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
FFMPEG_MULTILIB := 64
include $(LOCAL_PATH)/../android/build.mk

LOCAL_C_INCLUDES +=		\
	external/zlib

LOCAL_SHARED_LIBRARIES +=	\
	libz \
	libavutil \
	libswresample

ifneq ($(ARCH_ARM_HAVE_NEON),)
  LOCAL_SRC_FILES += neon/mpegvideo.c
endif

# This file crashes SDCLANG at -O2 or -O3
$(intermediates)/vp9dsp_8bpp.o: PRIVATE_CFLAGS += $(if $(filter arm64,$(TARGET_ARCH)),-O1)

LOCAL_MULTILIB := $(FFMPEG_MULTILIB)
include $(BUILD_SHARED_LIBRARY)


#Add for ffmpeg omx
include $(CLEAR_VARS)
LOCAL_MODULE        :=  libavcodec_vendor
LOCAL_PROPRIETARY_MODULE := true
LOCAL_INSTALLED_MODULE_STEM := libavcodec.so
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_STRIP_MODULE  := false
LOCAL_MULTILIB      := 32
LOCAL_REQUIRED_MODULES := libavcodec
LOCAL_SRC_FILES     := ../../../../../$(TARGET_OUT)/lib/libavcodec.so
include $(BUILD_PREBUILT)


include $(CLEAR_VARS)
LOCAL_MODULE        :=  libavcodec_vendor
LOCAL_PROPRIETARY_MODULE := true
LOCAL_INSTALLED_MODULE_STEM := libavcodec.so
LOCAL_MODULE_CLASS  := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_STRIP_MODULE  := false
LOCAL_MULTILIB      := 64
LOCAL_REQUIRED_MODULES := libavcodec
LOCAL_SRC_FILES     := ../../../../../$(TARGET_OUT)/lib64/libavcodec.so
include $(BUILD_PREBUILT)