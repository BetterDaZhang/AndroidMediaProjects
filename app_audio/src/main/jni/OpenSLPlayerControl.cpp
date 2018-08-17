//
// Created by zhangjunling on 18-8-17.
//
#include "com_zhangjunling_appaudio_OpenSLPlayerActivity.h"
#include "./libopensl/OpenSLPlayerControl.h"

#define SAMPLERATE 44100
#define CHANNELS 1
#define PERIOD_TIME 20 //ms
#define FRAME_SIZE SAMPLERATE*PERIOD_TIME/1000
#define BUFFER_SIZE FRAME_SIZE*CHANNELS
#define TEST_CAPTURE_FILE_PATH "/mnt/sdcard/audio.pcm"

OpenSLPlayer* openSLPlayer;
FILE* pcmfp;


void consumeDataCallback(short buffer[], int bufferSize, void* ctx){
    LOGE("bufferSize:%d", bufferSize);
    if (fread((unsigned char *) buffer, bufferSize * 2, 1, pcmfp) != 1) {
        LOGE("failed to read data \n ");
    }else{
        //这里也可以直接停止播放
        memset(buffer, 0, sizeof(buffer));
    }
}

JNIEXPORT void JNICALL Java_com_zhangjunling_appaudio_OpenSLPlayerActivity_opensl_1start
        (JNIEnv *env, jobject obj){
    pcmfp = fopen(TEST_CAPTURE_FILE_PATH, "rb");
    if( pcmfp == NULL ) {
        LOGE("cannot open file (%s) !\n",TEST_CAPTURE_FILE_PATH);
        return ;
    }

    openSLPlayer = new OpenSLPlayer();
    openSLPlayer->android_OpenAudioDevice(SAMPLERATE,CHANNELS,SL_PCMSAMPLEFORMAT_FIXED_16,BUFFER_SIZE,consumeDataCallback);

}


JNIEXPORT void JNICALL Java_com_zhangjunling_appaudio_OpenSLPlayerActivity_opensl_1stop
        (JNIEnv *env, jobject obj){
    if(openSLPlayer != NULL){
        openSLPlayer->android_CloseAudioDevice();
        fclose(pcmfp);
        delete openSLPlayer;
        openSLPlayer = NULL;
    }
}

