//
// Created by zhangjunling on 18-8-10.
//

#ifndef ANDROIDMEDIAPROJECTS_IMAGEPREVIEWCONTROL_H
#define ANDROIDMEDIAPROJECTS_IMAGEPREVIEWCONTROL_H

#include "pthread.h"
#include "android/native_window_jni.h"
#include "libcommon/CommonTools.h"
#include "egl_core.h"
#include "GraphicalRender.h"

class ImagePreviewControl {
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
    GraphicalRender* graphicalRender;


    bool isStartRendering;
    ANativeWindow *window;
    int viewWidth;
    int viewHeight;

    static void* startRenderThread(void*);
    void renderLoop();

public:
    ImagePreviewControl();
    virtual ~ImagePreviewControl();

    void start();
    void stop();

    void requestInitEGL();
    void requestSurfaceCreated(ANativeWindow *pWindow);
    void requestSurfaceChanged(ANativeWindow *pWindow,int width, int height);
    void requestOnDrawFrame();
    void requestExit();

};


#endif //ANDROIDMEDIAPROJECTS_IMAGEPREVIEWCONTROL_H
