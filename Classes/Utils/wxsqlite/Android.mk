LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := wxsqlite3_static

LOCAL_MODULE_FILENAME := wxsqlite

LOCAL_CFLAGS := \
  -DSQLITE_HAS_CODEC \

LOCAL_SRC_FILES := \
src/sqlite3secure.c \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..
                                 
include $(BUILD_STATIC_LIBRARY)