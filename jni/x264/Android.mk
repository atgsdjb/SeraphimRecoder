#compile x264  20121130  dongjb
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
include $(LOCAL_PATH)/config.mk
LOCAL_MODULE    := td_x264
### Add all source file names to be included in lib separated by a whitespace

LOCAL_SRC_FILES := $(SRCS) $(SRCLI) $(ASMSRC)
LOCAL_CFLAGS := -std=c99  $(X264_CFLAGS)  
LOCAL_LDLIBS  := -lpthread
#include $(BUILD_EXECUTABLE)
include $(BUILD_STATIC_LIBRARY)