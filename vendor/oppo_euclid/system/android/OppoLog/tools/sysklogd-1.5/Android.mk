LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := klogd
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := XBIN
LOCAL_SRC_FILES    := klogd
LOCAL_MODULE_PATH  := $(TARGET_OUT)/xbin
include $(BUILD_PREBUILT)






