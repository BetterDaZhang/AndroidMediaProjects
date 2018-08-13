//
// Created by zhangjunling on 18-8-13.
//


#include <librender/TexturePreviewControl.h>
#include "com_zhangjunling_example_androidmediaprojects_view_NativeTextureSurfaceView.h"

TexturePreviewControl* texturePreviewControl = NULL;

JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeTextureSurfaceView_start
        (JNIEnv *env, jobject obj, jstring imageFilePath){
    const char* _imageFilePath = env->GetStringUTFChars(imageFilePath, 0);
    texturePreviewControl = new TexturePreviewControl();
    texturePreviewControl->start(_imageFilePath);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeTextureSurfaceView_stop
        (JNIEnv *env, jobject obj){
    if(NULL != texturePreviewControl){
        texturePreviewControl->stop();
        delete texturePreviewControl;
        texturePreviewControl = NULL;
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeTextureSurfaceView_requestSurfaceChanged
        (JNIEnv *env, jobject obj, jobject jSurface, jint width, jint height){
    ANativeWindow *window = ANativeWindow_fromSurface(env, jSurface);
    if(NULL != texturePreviewControl){
        texturePreviewControl->requestSurfaceChanged(window, width, height);
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeTextureSurfaceView_requestExit
        (JNIEnv *env, jobject obj){
    if(NULL != texturePreviewControl){
        texturePreviewControl->requestExit();
    }
}