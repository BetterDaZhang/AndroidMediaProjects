//
// Created by zhangjunling on 18-8-10.
//
#include <librender/GraphicalRender.h>
#include "com_zhangjunling_example_androidmediaprojects_render_GraphicalRender.h"

GraphicalRender *graphicalRender = NULL;

JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_render_GraphicalRender_onSurfaceCrerated
        (JNIEnv *env, jobject obj, jstring vertexShaderCode, jstring fragmentShaderCode){

    const char* _vertexShaderCode = env->GetStringUTFChars(vertexShaderCode, 0);
    const char *_fragmentShaderCode = env->GetStringUTFChars(fragmentShaderCode, 0);

    graphicalRender = new GraphicalRender();
    graphicalRender->onSurfaceCreated(_vertexShaderCode, _fragmentShaderCode);

    env->ReleaseStringUTFChars(vertexShaderCode, _vertexShaderCode);
    env->ReleaseStringUTFChars(fragmentShaderCode, _fragmentShaderCode);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_render_GraphicalRender_onSurfaceChanged
        (JNIEnv *env, jobject obj, jint width, jint height){
    graphicalRender->onSurfaceChange(width, height);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_example_androidmediaprojects_render_GraphicalRender_onDrawFrame
        (JNIEnv *env, jobject obj){
    graphicalRender->onDrawFrame();
}

//TODO ... 释放掉　delete graphicalRender