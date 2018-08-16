//
// Created by zhangjunling on 18-8-15.
//

#include "OpenSLRecorder.h"

OpenSLRecorder::OpenSLRecorder(){

}

OpenSLRecorder::~OpenSLRecorder(){

}

void OpenSLRecorder::android_OpenAudioDevice(int sampleRete, int channels, int sampleFormat, int bufferFrames, getAudioDataCallback audioDataCallback){
    LOGE("android_OpenAudioDevice()");
    this->sampleRate = sampleRete;
    this->channels = channels;
    this->sampleFormat = sampleFormat;
    this->produceDataCallback = audioDataCallback;
    this->bufferFrames = bufferFrames;
    this->numsOfBuffer = bufferFrames;
    this->currentCaptureTime = 0;

    if((tempBuffer = new short[numsOfBuffer]) == NULL){
        android_CloseAudioDevice();
        return;
    }

    if(openSLCreateEngine() != SL_RESULT_SUCCESS){
        android_CloseAudioDevice();
        return;
    }

    if(openSLRecordOpen() != SL_RESULT_SUCCESS){
        android_CloseAudioDevice();
        return;
    }

    //回调函数必须通过第一次调用Enqueue触发
    (*recorderBufferQueue)->Enqueue(recorderBufferQueue, tempBuffer, numsOfBuffer * sizeof(short));
}

void OpenSLRecorder::android_CloseAudioDevice(){
    LOGE("android_CloseAudioDevice");
    openSLRecordStop();
    openSLDestroyEngine();

    if(tempBuffer != NULL){
        delete[] tempBuffer;
        tempBuffer = NULL;
    }
}

void OpenSLRecorder::bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context){
    LOGE("bqRecorderCallback");
    OpenSLRecorder* openSLRecorder = (OpenSLRecorder*)context;
    openSLRecorder->produceAudioData();
}


void OpenSLRecorder::produceAudioData(){
    LOGE("produceAudioData()");
    (*recorderBufferQueue)->Enqueue(recorderBufferQueue, tempBuffer, numsOfBuffer * sizeof(short));
    if(produceDataCallback != NULL){
        produceDataCallback(tempBuffer, numsOfBuffer, this);
    }
}

SLresult OpenSLRecorder::openSLCreateEngine(){
    SLresult result;
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if(result != SL_RESULT_SUCCESS) {
        goto engine_eng;
    }

    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS) {
        goto engine_eng;
    }

    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if (result != SL_RESULT_SUCCESS) {
        goto engine_eng;
    }

engine_eng:
    return result;

}

SLresult OpenSLRecorder::openSLRecordOpen(){
    SLresult result;
    SLuint32 sampleRate = this->sampleRate;
    SLuint32 channels = this->channels;

    if(channels){
        switch(sampleRate) {
            case 8000:
                sampleRate = SL_SAMPLINGRATE_8;
                break;
            case 11025:
                sampleRate = SL_SAMPLINGRATE_11_025;
                break;
            case 16000:
                sampleRate = SL_SAMPLINGRATE_16;
                break;
            case 22050:
                sampleRate = SL_SAMPLINGRATE_22_05;
                break;
            case 24000:
                sampleRate = SL_SAMPLINGRATE_24;
                break;
            case 32000:
                sampleRate = SL_SAMPLINGRATE_32;
                break;
            case 44100:
                sampleRate = SL_SAMPLINGRATE_44_1;
                break;
            case 48000:
                sampleRate = SL_SAMPLINGRATE_48;
                break;
            case 64000:
                sampleRate = SL_SAMPLINGRATE_64;
                break;
            case 88200:
                sampleRate = SL_SAMPLINGRATE_88_2;
                break;
            case 96000:
                sampleRate = SL_SAMPLINGRATE_96;
                break;
            case 192000:
                sampleRate = SL_SAMPLINGRATE_192;
                break;
            default:
                break;

            return -1;
        }

        // configure audio source
//        SLuint32	locatorType;
//        SLuint32	deviceType;
//        SLuint32	deviceID;
//        SLObjectItf	device;
        SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE,
                                          SL_IODEVICE_AUDIOINPUT,
                                          SL_DEFAULTDEVICEID_AUDIOINPUT,
                                          NULL};
//        void *pLocator;
//        void *pFormat;
        SLDataSource audioSrc = {&loc_dev, NULL};

        // configure audio sink
        SLuint32 speakers;
        if (channels > 1) {
            speakers = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        } else {
            speakers = SL_SPEAKER_FRONT_CENTER;
        }

        SLDataLocator_AndroidSimpleBufferQueue loc_bq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
        SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, channels, sampleRate,
                                       SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                       speakers, SL_BYTEORDER_LITTLEENDIAN};
        SLDataSink audioSink = {&loc_bq, &format_pcm};

        // create audio recorder(requires the RECORD_AUDIO permission)
        const SLInterfaceID id[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
        const SLboolean req[1] = {SL_BOOLEAN_TRUE};
        result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject, &audioSrc,
                                                      &audioSink, 1, id, req);
        if (result != SL_RESULT_SUCCESS) {
            goto end_RecordOpen;
        }

        // realize the audio recorder
        result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);
        if(result != SL_RESULT_SUCCESS){
            goto end_RecordOpen;
        }

        // get the record interface
        result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recorderRecord);
        if (result != SL_RESULT_SUCCESS) {
            goto end_RecordOpen;
        }

        // get the buffer queue interface
        result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                                 &recorderBufferQueue);
        if (result != SL_RESULT_SUCCESS) {
            goto end_RecordOpen;
        }

        result = (*recorderBufferQueue)->RegisterCallback(recorderBufferQueue, bqRecorderCallback,
                                                          this);
        if (result != SL_RESULT_SUCCESS) {
            goto end_RecordOpen;
        }

        result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_RECORDING);

        LOGE("SetRecordState Recording.");

    end_RecordOpen:
        return result;
    }
}

SLresult OpenSLRecorder::openSLRecordStop(){
    SLresult  result;
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_STOPPED);
    return result;
}

void OpenSLRecorder::openSLDestroyEngine(){
    if(recorderObject != NULL){
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = NULL;
        recorderRecord = NULL;
        recorderBufferQueue = NULL;
    }

    if(engineObject != NULL){
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}