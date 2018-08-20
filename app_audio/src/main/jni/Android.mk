LOCAL_PATH := $(call my-dir)

#构建FFmpeg预编译静态库
include $(CLEAR_VARS)
LOCAL_MODULE := libfdk-aac
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libfdk-aac.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libvo-aacenc
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libvo-aacenc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libx264
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libx264.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavfilter
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavfilter.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavformat
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavformat.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavcodec
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavcodec.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libavutil
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libavutil.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libswscale
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libswscale.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libswresample
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libswresample.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpostproc
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libpostproc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libmp3lame
LOCAL_SRC_FILES := $(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/libmp3lame.a
include $(PREBUILT_STATIC_LIBRARY)

#构建native_audio动态库
include $(CLEAR_VARS)
LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/3rdparty/ffmpeg/include

LOCAL_MODULE := native_audio

LOCAL_SRC_FILES := ./libopensl/OpenSLRecorder.cpp \
        ./libopensl/OpenSLPlayer.cpp \
        OpenSLRecordControl.cpp \
        OpenSLPlayerControl.cpp

LOCAL_STATIC_LIBRARIES := libavfilter libavformat libavcodec libavutil libswscale libswresample libpostproc libfdk-aac libvo-aacenc libx264  libmp3lame

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib
LOCAL_LDLIBS += -L$(LOCAL_PATH)/3rdparty/prebuilt/armv7-a/
LOCAL_LDLIBS += -lz
LOCAL_LDLIBS += -llog
LOCAL_LDLIBS += -lOpenSLES

include $(BUILD_SHARED_LIBRARY)