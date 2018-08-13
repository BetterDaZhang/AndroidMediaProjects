//
// Created by zhangjunling on 18-8-13.
//

#ifndef ANDROIDMEDIAPROJECTS_TEXTUREPREVIEWCONTROL_H
#define ANDROIDMEDIAPROJECTS_TEXTUREPREVIEWCONTROL_H

#include "pthread.h"
#include "android/native_window_jni.h"
#include "libcommon/CommonTools.h"
#include "egl_core.h"
#include "../3rdparty/libpng/png_decoder.h"
#include "pic_preview_texture.h"
#include "TexturePreviewRender.h"

class TexturePreviewControl {
private:
    enum RenderEvent {
        RE_NONE,
        RE_EGL_INIT,
        RE_SURFACE_CREATED,
        RE_SURFACE_CHANGED,
        RE_DRAW_FRAME,
        RE_EXIT
    };

    enum RenderEvent mEnumRenderEvent;
    pthread_t mThreadId;
    pthread_mutex_t mLock;
    pthread_cond_t mCondition;

    EGLCore *eglCore;
    EGLSurface eglSurface;

    PngPicDecoder* pngPicDecoder;
    PicPreviewTexture* picPreviewTexture;
    TexturePreviewRender* texturePreviewRender;

    bool isStartRendering;
    ANativeWindow *window;
    int viewWidth;
    int viewHeight;

    static void* startRenderThread(void*);
    void renderLoop();

public:
    TexturePreviewControl();
    virtual ~TexturePreviewControl();

    void start(const char*);
    void stop();

    void requestSurfaceChanged(ANativeWindow*,int , int );
    void requestExit();
};


#endif //ANDROIDMEDIAPROJECTS_TEXTUREPREVIEWCONTROL_H
