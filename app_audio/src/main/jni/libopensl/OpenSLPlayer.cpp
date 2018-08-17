//
// Created by zhangjunling on 18-8-17.
//

#include "OpenSLPlayerControl.h"

OpenSLPlayer::OpenSLPlayer(){

}

OpenSLPlayer::~OpenSLPlayer(){

}

void OpenSLPlayer::android_OpenAudioDevice(int sampleRete, int channels, int sampleFormat, int bufferFrames, getAudioDataCallback audioDataCallback){
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

    if (openSLPlayOpen() != SL_RESULT_SUCCESS) {
        android_CloseAudioDevice();
        return;
    }

    //回调函数必须通过第一次调用Enqueue触发
    (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, tempBuffer, numsOfBuffer * sizeof(short));

}

void OpenSLPlayer::android_CloseAudioDevice(){
    openSLPlayClose();
    openSLDestroyEngine();

    if(tempBuffer != NULL){
        delete[] tempBuffer;
        tempBuffer = NULL;
    }
}

void OpenSLPlayer::bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context){
    OpenSLPlayer *openSLPlayer = (OpenSLPlayer *) context;
    openSLPlayer->consumeAudioData();
}

void OpenSLPlayer::consumeAudioData(){

    if(produceDataCallback != NULL){
        produceDataCallback(tempBuffer, numsOfBuffer, this);
        (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, tempBuffer, numsOfBuffer * sizeof(short));
    }
}

SLresult OpenSLPlayer::openSLCreateEngine(){
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

SLresult OpenSLPlayer::openSLPlayOpen(){
    SLresult result;
    SLuint32 sampleRate = this->sampleRate;
    SLuint32 channels = this->channels;

    if(channels){
        //configure audio source
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,1};

        switch(sampleRate){
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
        }

        SLuint32 speaks;
        if (channels > 1) {
            speaks = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        } else {
            speaks = SL_SPEAKER_FRONT_CENTER;
        }


        SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, channels, sampleRate,
                                       SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                       speaks, SL_BYTEORDER_LITTLEENDIAN};
        SLDataSource audioSrc = {&loc_bufq, &format_pcm};

        // configure audio sink
        result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL);
        if(result != SL_RESULT_SUCCESS){
            goto end_playAudio;
        }

        // realize the output mix
        result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
        if (result != SL_RESULT_SUCCESS) {
            goto end_playAudio;
        }

        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
        SLDataSink audioSink = {&loc_outmix, NULL};

        // create audio player
        const SLInterfaceID ids[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
        const SLboolean reqs[1] = {SL_BOOLEAN_TRUE};
        result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayObject, &audioSrc,
                                                    &audioSink, 1, ids, reqs);
        if(result != SL_RESULT_SUCCESS){
            goto end_playAudio;
        }

        // realize the player
        result = (*bqPlayObject)->Realize(bqPlayObject, SL_BOOLEAN_FALSE);
        if(result != SL_RESULT_SUCCESS){
            goto end_playAudio;
        }

        // get the play interface
        result = (*bqPlayObject)->GetInterface(bqPlayObject, SL_IID_PLAY, &bqPlayerPlay);
        if(result != SL_RESULT_SUCCESS){
            goto end_playAudio;
        }

        // get the buffer queue interface
        result = (*bqPlayObject)->GetInterface(bqPlayObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                               &bqPlayerBufferQueue);
        if (result != SL_RESULT_SUCCESS) {
            goto end_playAudio;
        }

        // register callback on the buffer queue
        result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqRecorderCallback,
                                                          this);
        if (result != SL_RESULT_SUCCESS) {
            goto end_playAudio;
        }

        // set the player's state to playing
        result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    }
end_playAudio:
    return result;
}

SLresult OpenSLPlayer::openSLPlayClose(){
    return (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_STOPPED);
}

void OpenSLPlayer::openSLDestroyEngine(){
    if(bqPlayObject != NULL){
        (*bqPlayObject)->Destroy(bqPlayObject);
        bqPlayObject = NULL;
        bqPlayerPlay = NULL;
        bqPlayerBufferQueue = NULL;
    }

    if(engineObject != NULL){
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}