package com.zhangjunling.appaudio.audio_encode.natived;

import android.os.Environment;
import android.util.Log;

import com.zhangjunling.appaudio.audio_encode.android.AudioRecordRecorder;
import com.zhangjunling.appaudio.audio_encode.android.OutputPCMDelegate;
import com.zhangjunling.appaudio.pcm_recorder.AudioConfigurationException;
import com.zhangjunling.appaudio.pcm_recorder.StartRecordingException;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;

public class AudioFFmpegEncodeController implements OutputPCMDelegate {

    private static final String TAG = "AudioFFmpegEncode";
    private static final int AUDIO_CHANNELS = 1;
    private static final int BIT_RATE = 128 * 1024;
    private static final int SAMPLE_RATE = 44100;
    private static final int BITS_PER_SAMPLE = 16;

    //负责采集音频数据
    private AudioRecordRecorder audioRecordRecorder;
    private String aacFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "local_ffmpeg.aac";

    //负责编码aac数据,输出aac文件
    private AudioFFmpegEncoder audioFFmpegEncoder;

    public void prepare() throws AudioConfigurationException, IOException {
        audioRecordRecorder = new AudioRecordRecorder();
        audioRecordRecorder.initMetaData(this,AudioRecordRecorder.BUFFER_TYPE_SHORT);

        audioFFmpegEncoder = new AudioFFmpegEncoder();
        audioFFmpegEncoder.prepare(BIT_RATE, AUDIO_CHANNELS, SAMPLE_RATE, BITS_PER_SAMPLE, aacFilePath);
    }

    public void start() throws StartRecordingException {
        if(audioRecordRecorder != null){
            audioRecordRecorder.start();
        }
    }


    public void stop(){
        if(audioRecordRecorder != null){
            audioRecordRecorder.stop();
            audioRecordRecorder = null;
        }

        if(audioFFmpegEncoder != null){
            audioFFmpegEncoder.stop();
            audioFFmpegEncoder = null;
        }
    }

    @Override
    public void outputPCMPacketInByte(byte[] data) {

    }

    @Override
    public void outputPCMPacketInShort(short[] data) {
        if(audioFFmpegEncoder != null){
//            Log.e(TAG, "audioData:" + Arrays.toString(data));
//            Log.e(TAG, "data.length:" + data.length);
            audioFFmpegEncoder.fireAudio(data, data.length);
        }
    }
}
