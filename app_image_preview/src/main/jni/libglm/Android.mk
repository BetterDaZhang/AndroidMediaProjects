LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
    
LOCAL_MODULE := libglm
LOCAL_SRC_FILES = ./detail/*.* \
				  ./gtc/*.* \
				  ./gtx/*.* \
				  ./simd/*.*  \
				  ./*.*

LOCAL_C_INCLUDES += $(LOCAL_PATH) \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/detail \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/gtc \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/gtx \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/simd \

LOCAL_EXPORT_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)