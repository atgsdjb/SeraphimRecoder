LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
MY_FILES := $(wildcard $(LOCAL_PATH)/*.c*)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES	+= $(MY_FILES)
LOCAL_MODULE := faac
include $(BUILD_STATIC_LIBRARY)
