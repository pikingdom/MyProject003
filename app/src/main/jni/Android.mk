LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Encode
LOCAL_SRC_FILES := Encode.cpp ReadTools.cpp ReadTools.h Nsdes.cpp DES.h WriteTools.h WriteTools.cpp
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)
