package com.zhangjunling.appaudio.audio_encode.natived;

public class AudioFFmpegEncoder {

    public void prepare(int bitRate, int channels, int sampleRate, int bitsPerSample, String aacFilePath){
        init(bitRate,channels,sampleRate,bitsPerSample,aacFilePath);
    }

    public void fireAudio(short[] data, int len) {
        encode(data, len);
    }

    public void stop(){
        destroy();
    }

    private native int init(int bitRate, int channels, int sampleRate, int bitsPerSample, String aacFilePath);
    private native void encode(short[] buffer, int bufferSize);
    private native void destroy();
}
