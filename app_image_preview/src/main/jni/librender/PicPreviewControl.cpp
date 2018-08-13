//
// Created by zhangjunling on 18-8-8.
//

#include "PicPreviewControl.h"
#define PREVIEW_PIXEL_BYTES 4

PicPreviewControl::PicPreviewControl(){
    pthread_mutex_init(&_mLock, NULL);
    pthread_cond_init(&_mCondition, NULL);
    pngPicDecoder = NULL;

}

PicPreviewControl::~PicPreviewControl(){
    pthread_mutex_destroy(&_mLock);
    pthread_cond_destroy(&_mCondition);
    pngPicDecoder = NULL;
}

bool PicPreviewControl::start(char* imageFilePath){
    LOGE("PicPreviewControl::start()");
    char* _imageFilePath = imageFilePath;
    pngPicDecoder = new PngPicDecoder();
    pngPicDecoder->openFile(_imageFilePath);
    return true;
}

void PicPreviewControl::setWindow(ANativeWindow* window){
    LOGE("PicPreviewControl::setWindow()");
    this->_window = window;
    pthread_create(&_threadId,0,threadStartDecoder,this);
}

void* PicPreviewControl::threadStartDecoder(void* myself){
    LOGE("PicPreviewControl::threadStartDecoder()");
    PicPreviewControl* picPreviewControl = (PicPreviewControl*)myself;
    picPreviewControl->previewImage();
    pthread_exit(0);
    return 0;
}

void PicPreviewControl::previewImage(){
    LOGE("PicPreviewControl::previewImage()");
    GLenum colorFormat = pngPicDecoder->getRawImageData().gl_color_format;
    switch(colorFormat){
        case GL_RGB:
            LOGE("colorFormat:GL_RGB");
            break;
        case GL_RGB565:
            LOGE("colorFormat:GL_RGB565");
            break;
        case GL_RGBA:
            LOGE("colorFormat:GL_RGBA");
            break;
    }
    RawImageData rawImageData = pngPicDecoder->getRawImageData();

    ANativeWindow_setBuffersGeometry(_window,
                                     rawImageData.width, rawImageData.height, WINDOW_FORMAT_RGBA_8888);

    //渲染
    if(_window != NULL){
        ANativeWindow_Buffer buffer;

        ANativeWindow_lock(_window, &buffer, NULL);

        const uint8_t *src = (uint8_t *)rawImageData.data;
        const int src_w = rawImageData.width * PREVIEW_PIXEL_BYTES;
        const int src_step = rawImageData.width * PREVIEW_PIXEL_BYTES;
        // destination = Surface(ANativeWindow)
        uint8_t *dest = (uint8_t *)buffer.bits;
        const int dest_w = buffer.width * PREVIEW_PIXEL_BYTES;
        const int dest_step = buffer.stride * PREVIEW_PIXEL_BYTES;
        // use lower transfer bytes
        const int w = src_w < dest_w ? src_w : dest_w;
        // use lower height
        const int h = rawImageData.height < buffer.height ? rawImageData.height : buffer.height;
        // transfer from frame data to the Surface
        copyFrame(src, dest, w, h, src_step, dest_step);


        ANativeWindow_unlockAndPost(_window);
    }
    pngPicDecoder->releaseRawImageData(&rawImageData);
}


void PicPreviewControl::copyFrame(const uint8_t *src, uint8_t *dest, const int width, int height, const int stride_src, const int stride_dest) {
    LOGE("PicPreviewControl::copyFrame()");
    const int h8 = height % 8;
    for (int i = 0; i < h8; i++) {
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
    }
    for (int i = 0; i < height; i += 8) {
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
        memcpy(dest, src, width);
        dest += stride_dest; src += stride_src;
    }
}

void PicPreviewControl::stop(){
    pthread_join(_threadId, 0);
    if(pngPicDecoder != NULL){
        pngPicDecoder->closeFile();
        pngPicDecoder = NULL;
    }
}
