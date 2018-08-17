//
// Created by zhangjunling on 18-8-17.
//

#ifndef ANDROIDMEDIAPROJECTS_OPENSLPLAYER_H
#define ANDROIDMEDIAPROJECTS_OPENSLPLAYER_H

#include "./libcommon/CommonTools.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

typedef void (*getAudioDataCallback)(short buffer[], int bufferSize, void* ctx);

class OpenSLPlayer {
public:
    OpenSLPlayer();
    virtual ~OpenSLPlayer();

    void android_OpenAudioDevice(int sampleRete, int channels, int sampleFormat, int bufferFrames, getAudioDataCallback audioDataCallback);

    void android_CloseAudioDevice();

private:
    double currentCaptureTime;
    int channels;
    int sampleRate;
    int sampleFormat;
    int bufferFrames;

    int numsOfBuffer;
    short* tempBuffer;

    getAudioDataCallback produceDataCallback;

    // engine interface
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // buffer queue player interface
    SLObjectItf bqPlayObject;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;


    static void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
    void consumeAudioData();

    SLresult openSLCreateEngine();
    SLresult openSLPlayOpen();
    SLresult openSLPlayClose();

    void openSLDestroyEngine();
};


#endif //ANDROIDMEDIAPROJECTS_OPENSLPLAYER_H
