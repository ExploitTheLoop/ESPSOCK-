LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sock
LOCAL_SRC_FILES := main.cpp
LOCAL_CPPFLAGS := -std=c++11    
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -fPIE -pie  

include $(BUILD_EXECUTABLE)    
