//
// Created by zhangjunling on 18-8-13.
//

#include "com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView.h"
#include "./librender/ImagePreviewControl.h"

ImagePreviewControl *imagePreviewControl = NULL;

JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_start
        (JNIEnv *env, jobject obj){
    if(NULL == imagePreviewControl){
        imagePreviewControl = new ImagePreviewControl();
        imagePreviewControl->start();
    }
}

JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_stop
        (JNIEnv *env, jobject obj){
    if(NULL != imagePreviewControl){
        imagePreviewControl->stop();
        delete imagePreviewControl;
        imagePreviewControl = NULL;
    }

}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_requestInitEGL
        (JNIEnv *env, jobject obj){
    if(NULL != imagePreviewControl){
        imagePreviewControl->requestInitEGL();
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_requestSurfaceCreated
        (JNIEnv *env, jobject obj, jobject jSurface){
    ANativeWindow *window = ANativeWindow_fromSurface(env, jSurface);
    if(NULL != imagePreviewControl){
        imagePreviewControl->requestSurfaceCreated(window);
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_requestSurfaceChanged
        (JNIEnv *env, jobject obj, jobject jSurface, jint width, jint height){
    ANativeWindow *window = ANativeWindow_fromSurface(env, jSurface);
    if(NULL != imagePreviewControl){
        imagePreviewControl->requestSurfaceChanged(window, width, height);
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_requestOnDrawFrame
        (JNIEnv *env, jobject obj){
    if(NULL != imagePreviewControl){
        imagePreviewControl->requestOnDrawFrame();
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeEGLSurfaceView_requestExit
        (JNIEnv *env, jobject obj){
    if(NULL != imagePreviewControl){
        imagePreviewControl->requestExit();
    }
}