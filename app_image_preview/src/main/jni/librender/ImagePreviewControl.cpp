//
// Created by zhangjunling on 18-8-10.
//

#include "ImagePreviewControl.h"

const char* vertexShaderCode = "uniform mat4 u_ProjectionMatrix;\n"
        "uniform mat4 u_LookAtMatrix;\n"
        "uniform mat4 u_RotationMatrix;\n"
        "attribute vec4 a_Position;\n"
        "attribute vec4 a_Color;\n"
        "varying vec4 v_Color;\n"
        "void main()\n"
        "{\n"
        "    v_Color = a_Color;\n"
        "    gl_Position = u_ProjectionMatrix * u_LookAtMatrix * u_RotationMatrix * a_Position;\n"
        "    gl_PointSize = 10.0;\n"
        "}";

const char* fragmentShaderCode = "precision mediump float;\n"
        "varying vec4 v_Color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = v_Color;\n"
        "}";

ImagePreviewControl::ImagePreviewControl(){
    pthread_mutex_init(&mLock, NULL);
    pthread_cond_init(&mCondition, NULL);
    mEnumRenderEvent = RE_NONE;
    isStartRendering = false;
    window = NULL;

}

ImagePreviewControl::~ImagePreviewControl(){
    pthread_mutex_destroy(&mLock);
    pthread_cond_destroy(&mCondition);
    window = NULL;
}

void ImagePreviewControl::start(){
    graphicalRender = new GraphicalRender();
    pthread_create(&mThreadId,NULL,startRenderThread,this);
}

void ImagePreviewControl::stop(){
    isStartRendering = false;
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_NONE;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);

    pthread_join(mThreadId, NULL);

    if(NULL != graphicalRender){
        delete graphicalRender;
        graphicalRender = NULL;
    }


}

void ImagePreviewControl::requestInitEGL(){
    LOGI("requestInitEGL()    <----------------");
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_EGL_INIT;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);
    LOGI("requestInitEGL()    ---------------->");
}

void ImagePreviewControl::requestSurfaceCreated(ANativeWindow *pWindow){
    LOGI("requestSurfaceCreated()    <----------------");
    window = pWindow;
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_SURFACE_CREATED;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);
    LOGI("requestSurfaceCreated()    ---------------->");
}

void ImagePreviewControl::requestSurfaceChanged(ANativeWindow *pWindow,int width, int height){
    LOGI("requestSurfaceChanged()    <----------------");
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_SURFACE_CHANGED;
    window = pWindow;
    viewWidth = width;
    viewHeight = height;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);
    LOGI("requestSurfaceChanged()    ---------------->");
}

void ImagePreviewControl::requestOnDrawFrame(){
    LOGI("requestOnDrawFrame()    <----------------");
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_DRAW_FRAME;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);
    LOGI("requestOnDrawFrame()    ---------------->");
}

void ImagePreviewControl::requestExit(){
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_EXIT;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);
}

void* ImagePreviewControl::startRenderThread(void* myself){
//    LOGI("startRenderThread()    <----------------");
    ImagePreviewControl *imagePreviewControl = (ImagePreviewControl *) myself;
    imagePreviewControl->renderLoop();
//    LOGI("startRenderThread()    ---------------->");
    pthread_exit(0);
    return 0;
}

void ImagePreviewControl::renderLoop(){
    LOGI("renderLoop()    <----------------");
    isStartRendering = true;
    while (isStartRendering){
        pthread_mutex_lock(&mLock);
        pthread_cond_wait(&mCondition, &mLock);

        switch(mEnumRenderEvent){
            case RE_EGL_INIT:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("init EGL...");

                break;
            case RE_SURFACE_CREATED:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("surface created...");


                break;
            case RE_SURFACE_CHANGED:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("surface changed...");
                eglCore = new EGLCore();
                eglCore->init();

                eglSurface = eglCore->createWindowSurface(window);
                eglCore->makeCurrent(eglSurface);
                graphicalRender->onSurfaceCreated(vertexShaderCode, fragmentShaderCode);

                graphicalRender->onSurfaceChange(viewWidth, viewHeight);

                graphicalRender->onDrawFrame();

                eglCore->swapBuffers(eglSurface);

                break;
            case RE_DRAW_FRAME:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("on draw frame...");


                break;
            case RE_EXIT:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("exit...");
                eglCore->releaseSurface(eglSurface);
                eglCore->release();


                break;
            default:
                LOGI("default...");
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                break;
        }
    }
    LOGI("renderLoop()    ---------------->");
}