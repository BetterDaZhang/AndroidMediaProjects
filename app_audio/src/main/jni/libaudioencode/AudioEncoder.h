//
// Created by zhangjunling on 18-8-20.
//

#ifndef ANDROIDMEDIAPROJECTS_AUDIOENCODER_H
#define ANDROIDMEDIAPROJECTS_AUDIOENCODER_H
extern "C" {
    #include "./3rdparty/ffmpeg/include/libavcodec/avcodec.h"
    #include "./3rdparty/ffmpeg/include/libavformat/avformat.h"
    #include "./3rdparty/ffmpeg/include/libswresample/swresample.h"
};

#include "./libcommon/CommonTools.h"

#ifndef AUDIO_BITE_RATE
#define AUDIO_BITE_RATE 64000
#endif


class AudioEncoder {
public:
    AudioEncoder();
    virtual ~AudioEncoder();

    int init(int bitRate, int channels, int sampleRate, int bitsPerSample, const char* aacFilePath, char* codecName);

    void encode(byte *buffer, int bufferSize);

    void destroy();

private:
    AVFormatContext *avFormatContext;
    AVCodecContext *avCodecContext;
    AVStream *audioStream;
    AVCodec *avCodec;

    bool isWriteHeaderSuccess;
    double duration;

    AVFrame *inputFrame;
    int sampleSize;
    uint8_t *samples;

    int samplesCursor;
    int audioBufferSize;
    int audioSamplesCursor;

    uint8_t **convertData;

    SwrContext *swrContext;
    AVFrame *swrFrame;
    uint8_t *swrBuffer;
    int swrBufferSize;

    int audioBitRate;
    int audioChannels;
    int audioSampleRate;
    int audioBitsPerSample;

    int totalSWRTimeMills;
    int totalEncodeTimeMills;

    //初始化的时候,要进行的工作
    int alloc_audio_stream(const char *codec_name);
    int alloc_avframe();
    void cpyToAudioSamples(byte* sourceBuffer, int cpyLength);

    //当够了一个frame之后就要编码一个packet
    void encodePacket();
};


#endif //ANDROIDMEDIAPROJECTS_AUDIOENCODER_H
