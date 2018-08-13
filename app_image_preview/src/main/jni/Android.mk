LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/3rdparty/libpng \
    ./librender/GraphicalRender.h

LOCAL_SRC_FILES = \
    ./librender/PicPreviewControl.cpp \
    ./librender/GraphicalRender.cpp \
    ./librender/egl_core.cpp \
    ./librender/ImagePreviewControl.cpp \
    ./librender/pic_preview_texture.cpp \
    ./librender/TexturePreviewRender.cpp \
    ./librender/TexturePreviewControl.cpp \
    ./libcommon/GLHelper.cpp \
    ./NativeGraphicalContral.cpp \
    ./NativeEglGraphicalContral.cpp \
    ./NativeControl.cpp \
    ./NativeTextureControl.cpp

LOCAL_STATIC_LIBRARIES := libpngdecoder libglm

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lz -landroid -lGLESv2 -lEGL

LOCAL_MODULE := libPngDecodePreview
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))