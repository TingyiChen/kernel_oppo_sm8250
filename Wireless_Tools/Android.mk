LOCAL_PATH:= $(call my-dir)

# Version of the Wireless Tools
WT_VERSION := $(shell sed -ne "/WT_VERSION/{s:\([^0-9]*\)::;p;q;}" < $(LOCAL_PATH)/iwlib.h )

# Version of Wireless Extensions.
WE_VERSION := $(shell sed -ne "/WE_VERSION/{s:\([^0-9]*\)::;p;q;}" < $(LOCAL_PATH)/iwlib.h )

# Always use local header for wireless extensions
WEXT_HEADER := wireless.$(WE_VERSION).h

CFLAGS:=-Os -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow \
	-Wpointer-arith -Wcast-qual -Winline

STRIPFLAGS:= -Wl,-s
WELIB_FLAG:= -DWE_NOLIBM=y
DEPFLAGS:=-MMD
PICFLAG:=-fPIC
#undefine WE_ESSENTIAL to have the tool to print more messages.
#WEDEF_FLAG:= -DWE_ESSENTIAL=y
WEDEF_FLAG :=
CFLAGS += -DHAVE_ANDROID=y
XCFLAGS:=$(CFLAGS) $(DEPFLAGS) $(WELIB_FLAG) $(WEDEF_FLAG)

# ------ wireless.h -------
include $(CLEAR_VARS)
LOCAL_MODULE := wireless_tool
LOCAL_MODULE_CLASS := include
WEXTH_TARGET_DIR := $(call local-intermediates-dir, FORCE_COMMON)
WEXTH_BUILT_MODULE := $(WEXTH_TARGET_DIR)/wireless.h
$(WEXTH_BUILT_MODULE): $(LOCAL_PATH)/$(WEXT_HEADER) 
	$(copy-file-to-new-target-with-cp)

all_copied_headers: $(WEXTH_BUILT_MODULE)

# --- iwlib ---------
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= iwlib.c
LOCAL_MODULE := libiw
LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
LOCAL_CFLAGS := $(CFLAGS)
include $(BUILD_STATIC_LIBRARY)

# --- iwconfig -----------
#include $(CLEAR_VARS)
#LOCAL_MODULE = iwconfig
#LOCAL_SRC_FILES := iwconfig.c
#LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
#LOCAL_STATIC_LIBRARIES := libiw 
#LOCAL_LDFLAGS := $(XCFLAGS)
#LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES) 
#include $(BUILD_EXECUTABLE)

# --- iwlist -----------
include $(CLEAR_VARS)
LOCAL_MODULE = iwlist
LOCAL_SRC_FILES := iwlist.c
LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
LOCAL_STATIC_LIBRARIES := libiw 
LOCAL_LDFLAGS := $(XCFLAGS)
LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES) 
include $(BUILD_EXECUTABLE)

# --- iwpriv -----------
include $(CLEAR_VARS)
LOCAL_MODULE = iwpriv
LOCAL_SRC_FILES := iwpriv.c
LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
LOCAL_STATIC_LIBRARIES := libiw 
LOCAL_LDFLAGS := $(XCFLAGS)
LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
include $(BUILD_EXECUTABLE)

# --- iwspy -----------
#include $(CLEAR_VARS)
#LOCAL_MODULE = iwspy
#LOCAL_SRC_FILES := iwspy.c
#LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
#LOCAL_STATIC_LIBRARIES := libiw 
#LOCAL_LDFLAGS := $(XCFLAGS)
#LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
#include $(BUILD_EXECUTABLE)

# --- iwgetid -----------
#include $(CLEAR_VARS)
#LOCAL_MODULE = iwgetid
#LOCAL_SRC_FILES := iwgetid.c
#LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
#LOCAL_STATIC_LIBRARIES := libiw 
#LOCAL_LDFLAGS := $(XCFLAGS)
#LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
#include $(BUILD_EXECUTABLE)

# --- iwevent -----------
#include $(CLEAR_VARS)
#LOCAL_MODULE = iwevent
#LOCAL_SRC_FILES := iwevent.c
#LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
#LOCAL_STATIC_LIBRARIES := libiw 
#LOCAL_LDFLAGS := $(XCFLAGS)
#LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
#include $(BUILD_EXECUTABLE)

# --- ifrename -----------
#include $(CLEAR_VARS)
#LOCAL_MODULE = ifrename
#LOCAL_SRC_FILES := ifrename.c
#LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
#LOCAL_STATIC_LIBRARIES := libiw 
#LOCAL_LDFLAGS := $(XCFLAGS)
#LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
#include $(BUILD_EXECUTABLE)
#ifrename requires getline() which is not supported by bionic. so we don't enable it currently.

# --- macaddr -----------
#include $(CLEAR_VARS)
#LOCAL_MODULE = macadd
#LOCAL_SRC_FILES := macaddr.c
#LOCAL_C_INCLUDES := $(WEXTH_TARGET_DIR)
#LOCAL_STATIC_LIBRARIES := libiw 
#LOCAL_LDFLAGS := $(XCFLAGS)
#LOCAL_CFLAGS := $(XCFLAGS)
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
#include $(BUILD_EXECUTABLE)

