//
// Created by zhangjunling on 18-8-15.
//

#ifndef ANDROIDMEDIAPROJECTS_OPENSLRECORDER_H
#define ANDROIDMEDIAPROJECTS_OPENSLRECORDER_H

#include "./libcommon/CommonTools.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

typedef void (*getAudioDataCallback)(short buffer[], int bufferSize, void* ctx);

class OpenSLRecorder {
public:
    OpenSLRecorder();
    virtual ~OpenSLRecorder();

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

    // recorder interface
    SLObjectItf recorderObject;
    SLRecordItf recorderRecord;
    SLAndroidSimpleBufferQueueItf recorderBufferQueue;

    static void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

    void produceAudioData();

    SLresult openSLCreateEngine();
    SLresult openSLRecordOpen();
    SLresult openSLRecordStop();

    void openSLDestroyEngine();

};


#endif //ANDROIDMEDIAPROJECTS_OPENSLRECORDER_H
