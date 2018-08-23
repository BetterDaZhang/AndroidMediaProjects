//
// Created by zhangjunling on 18-8-23.
//

#include "com_zhangjunling_appaudio_audio_encode_natived_AudioFFmpegEncoder.h"
#include "./libaudioencode/AudioEncoder.h"

AudioEncoder* audioEncoder;

// init(int bitRate, int channels, int sampleRate, int bitsPerSample, String aacFilePath);
JNIEXPORT jint JNICALL Java_com_zhangjunling_appaudio_audio_1encode_natived_AudioFFmpegEncoder_init
        (JNIEnv *env, jobject obj, jint bitRate, jint channels, jint sampleRate, jint bitsPerSample, jstring aacFilePath){

    const char* _aacFilePath = env->GetStringUTFChars(aacFilePath, 0);
    char* codec_name = "libfdk_aac";

    audioEncoder = new AudioEncoder();
    audioEncoder->init(bitRate, channels, sampleRate, bitsPerSample, _aacFilePath, codec_name);

    env->ReleaseStringUTFChars(aacFilePath, _aacFilePath);
}


JNIEXPORT void JNICALL Java_com_zhangjunling_appaudio_audio_1encode_natived_AudioFFmpegEncoder_encode
(JNIEnv *env, jobject obj, jshortArray audioArray, jint audioArrayLen){
    if(NULL != audioEncoder){
        short* pAudioArray = env->GetShortArrayElements(audioArray, 0);
        byte* audioBuffer = new byte[audioArrayLen * sizeof(short)];
        memcpy(audioBuffer, pAudioArray, audioArrayLen * sizeof(short));
        env->ReleaseShortArrayElements(audioArray, pAudioArray, 0);

        audioEncoder->encode(audioBuffer, audioArrayLen * sizeof(short));
        delete[] audioBuffer;
    }
}


JNIEXPORT void JNICALL Java_com_zhangjunling_appaudio_audio_1encode_natived_AudioFFmpegEncoder_destroy
(JNIEnv *env, jobject obj){
    if(NULL != audioEncoder){
        audioEncoder->destroy();
    }
}