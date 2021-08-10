LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
# give module name
LOCAL_MODULE    := sock
# list your C files to compile
LOCAL_SRC_FILES := pubg/main32.cpp
# this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
# give module name
LOCAL_MODULE    := sock64
# list your C files to compile
LOCAL_SRC_FILES := pubg/main64.cpp
# this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)