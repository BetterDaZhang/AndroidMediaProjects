//
// Created by zhangjunling on 18-8-8.
//

#ifndef ANDROIDMEDIAPROJECTS_PICPREVIEWCONTROL_H
#define ANDROIDMEDIAPROJECTS_PICPREVIEWCONTROL_H

#include <pthread.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "./3rdparty/libpng/png_decoder.h"
#include "libcommon/CommonTools.h"

class PicPreviewControl{
public:
    PicPreviewControl();
    virtual ~PicPreviewControl();

    bool start(char* imageFilePath);
    void setWindow(ANativeWindow* window);
    void stop();

private:
    PngPicDecoder* pngPicDecoder;

    int screenWidth;
    int screenHeight;

    pthread_t _threadId;
    pthread_mutex_t _mLock;
    pthread_cond_t _mCondition;

    ANativeWindow* _window;

    void previewImage();
    static void* threadStartDecoder(void* myself);
    static void copyFrame(const uint8_t *src, uint8_t *dest, const int width, int height, const int stride_src, const int stride_dest);
};



#endif //ANDROIDMEDIAPROJECTS_PICPREVIEWCONTROL_H
