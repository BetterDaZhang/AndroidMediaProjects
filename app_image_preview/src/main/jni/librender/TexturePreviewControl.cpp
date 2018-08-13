//
// Created by zhangjunling on 18-8-13.
//

#include "TexturePreviewControl.h"

TexturePreviewControl::TexturePreviewControl(){
    pthread_mutex_init(&mLock, NULL);
    pthread_cond_init(&mCondition, NULL);
    mEnumRenderEvent = RE_NONE;
    isStartRendering = false;
    window = NULL;

}

TexturePreviewControl::~TexturePreviewControl(){
    pthread_mutex_destroy(&mLock);
    pthread_cond_destroy(&mCondition);
    window = NULL;
}

void TexturePreviewControl::start(const char* imageFilePath){
    pngPicDecoder = new PngPicDecoder();
    pngPicDecoder->openFile((char*)imageFilePath);

    picPreviewTexture = new PicPreviewTexture();
    texturePreviewRender = new TexturePreviewRender();

    pthread_create(&mThreadId,NULL,startRenderThread,this);
}

void TexturePreviewControl::stop(){
    isStartRendering = false;
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_NONE;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);

    pthread_join(mThreadId, NULL);

}

void TexturePreviewControl::requestSurfaceChanged(ANativeWindow *pWindow,int width, int height){
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



void TexturePreviewControl::requestExit(){
    pthread_mutex_lock(&mLock);
    mEnumRenderEvent = RE_EXIT;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mCondition);
}

void* TexturePreviewControl::startRenderThread(void* myself){
//    LOGI("startRenderThread()    <----------------");
    TexturePreviewControl *imagePreviewControl = (TexturePreviewControl *) myself;
    imagePreviewControl->renderLoop();
//    LOGI("startRenderThread()    ---------------->");
    pthread_exit(0);
    return 0;
}

void TexturePreviewControl::renderLoop(){
    LOGI("renderLoop()    <----------------");
    isStartRendering = true;
    while (isStartRendering){
        pthread_mutex_lock(&mLock);
        pthread_cond_wait(&mCondition, &mLock);

        switch(mEnumRenderEvent){

            case RE_SURFACE_CHANGED:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("surface changed...");

                //初始化EGL环境
                eglCore = new EGLCore();
                eglCore->init();
                eglSurface = eglCore->createWindowSurface(window);
                eglCore->makeCurrent(eglSurface);

                picPreviewTexture->createTexture();

                if(NULL != pngPicDecoder){
                    const RawImageData raw_image_data = pngPicDecoder->getRawImageData();
                    picPreviewTexture->updateTexImage((byte*) raw_image_data.data, raw_image_data.width, raw_image_data.height);
                    pngPicDecoder->releaseRawImageData(&raw_image_data);
                }

                texturePreviewRender->init(viewWidth, viewHeight, picPreviewTexture);
                texturePreviewRender->onDrawFrame();

                eglCore->swapBuffers(eglSurface);

                break;
            case RE_EXIT:
                mEnumRenderEvent = RE_NONE;
                pthread_mutex_unlock(&mLock);
                LOGI("exit...");
                eglCore->releaseSurface(eglSurface);
                eglCore->release();

                if(NULL != pngPicDecoder){
                    pngPicDecoder->closeFile();
                    delete pngPicDecoder;
                    pngPicDecoder = NULL;
                }

                if(NULL != texturePreviewRender){
                    texturePreviewRender->dealloc();
                    delete texturePreviewRender;
                    texturePreviewRender = NULL;
                }

                if(NULL != picPreviewTexture){
                    picPreviewTexture->dealloc();
                    delete picPreviewTexture;
                    picPreviewTexture = NULL;
                }

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