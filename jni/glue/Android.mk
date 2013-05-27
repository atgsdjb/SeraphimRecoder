
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

MY_FILES := $(wildcard $(LOCAL_PATH)/*.c*)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES	+= $(MY_FILES)

LOCAL_STATIC_LIBRARIES :=faac mp4v2
LOCAL_LDLIBS := -lz -lc -lm -llog -L. -lx264
#LOCLA_
//LOCAL_CXXFLAGS += -std=c++11 
LOCAL_MODULE    := tdcodec.02

include $(BUILD_SHARED_LIBRARY)
