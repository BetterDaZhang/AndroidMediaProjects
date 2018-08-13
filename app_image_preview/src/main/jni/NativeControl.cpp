//
// Created by zhangjunling on 18-8-8.
//
#include "com_zhangjunling_example_androidmediaprojects_view_NativeSurfaceView.h"
#include "librender/PicPreviewControl.h"

PicPreviewControl *picPreviewControl = NULL;
ANativeWindow *window = NULL;

JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeSurfaceView_start
(JNIEnv *env, jobject obj, jstring imageFilePath){
    if(picPreviewControl == NULL){
        picPreviewControl = new PicPreviewControl();
    }
    char* pngFilePath = (char*) env->GetStringUTFChars(imageFilePath, NULL);
    picPreviewControl->start(pngFilePath);
    env->ReleaseStringUTFChars(imageFilePath, pngFilePath);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeSurfaceView_setWindow
(JNIEnv *env, jobject obj,jobject jSurface){
    window = ANativeWindow_fromSurface(env, jSurface);
    picPreviewControl->setWindow(window);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_view_NativeSurfaceView_stop
(JNIEnv *env, jobject obj){
    if(picPreviewControl != NULL){
        picPreviewControl->stop();
        picPreviewControl = NULL;
    }

    if(window != NULL){
        ANativeWindow_release(window);
        window = NULL;
    }
}
