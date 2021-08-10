LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sock
LOCAL_SRC_FILES := Putri.cpp
LOCAL_CPPFLAGS := -std=gnu++0x -Wall -fPIE         
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -fPIE -pie  

include $(BUILD_EXECUTABLE)    