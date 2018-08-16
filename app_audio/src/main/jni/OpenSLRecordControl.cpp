//
// Created by zhangjunling on 18-8-16.
//

#include "com_zhangjunling_appaudio_OpenSLRecordActivity.h"
#include "./libopensl/OpenSLRecorder.h"

#define SAMPLERATE 44100
#define CHANNELS 1
#define PERIOD_TIME 20 //ms
#define FRAME_SIZE SAMPLERATE*PERIOD_TIME/1000
#define BUFFER_SIZE FRAME_SIZE*CHANNELS
#define TEST_CAPTURE_FILE_PATH "/mnt/sdcard/audio.pcm"


OpenSLRecorder* openSLRecorder;
FILE* fp;

void audioDataCallback(short buffer[], int bufferSize, void* ctx){
    LOGE("bufferSize:%d", bufferSize);
    fwrite((unsigned char *)buffer, bufferSize*sizeof(short), 1, fp);
}

JNIEXPORT void JNICALL Java_com_zhangjunling_appaudio_OpenSLRecordActivity_opensl_1start
        (JNIEnv *env, jobject obj){
    LOGE("Java_com_zhangjunling_appaudio_OpenSLRecordActivity_opensl_1start");

    fp = fopen(TEST_CAPTURE_FILE_PATH, "wb");
    if( fp == NULL ) {
        LOGE("cannot open file (%s)\n", TEST_CAPTURE_FILE_PATH);
        return ;
    }
    openSLRecorder = new OpenSLRecorder();
    openSLRecorder->android_OpenAudioDevice(SAMPLERATE, CHANNELS, SL_PCMSAMPLEFORMAT_FIXED_16,
                                            BUFFER_SIZE, audioDataCallback);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_appaudio_OpenSLRecordActivity_opensl_1stop
        (JNIEnv *env, jobject obj){
    LOGE("Java_com_zhangjunling_appaudio_OpenSLRecordActivity_opensl_1stop");
    openSLRecorder->android_CloseAudioDevice();
    fclose(fp);
}

