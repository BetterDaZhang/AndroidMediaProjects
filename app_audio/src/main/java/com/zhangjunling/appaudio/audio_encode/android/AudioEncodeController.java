package com.zhangjunling.appaudio.audio_encode.android;

import android.os.Environment;

import com.zhangjunling.appaudio.pcm_recorder.AudioConfigurationException;
import com.zhangjunling.appaudio.pcm_recorder.StartRecordingException;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class AudioEncodeController implements OutputPCMDelegate,OutputAACDelegate{

    private static final String TAG = "AudioEncodeController";

    //负责采集音频数据
    private AudioRecordRecorder audioRecordRecorder;
    //负责对音频数据进行编码
    private AudioEncodeEncoder audioEncodeEncoder;
    //负责输出AAC音频数据
    private FileOutputStream outputStream = null;
    private String aacFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "local_mediacodec.aac";

    public void prepare() throws AudioConfigurationException, IOException {
        audioRecordRecorder = new AudioRecordRecorder();
        audioRecordRecorder.initMetaData(this,AudioRecordRecorder.BUFFER_TYPE_BYTE);

        audioEncodeEncoder = new AudioEncodeEncoder(this,
                audioRecordRecorder.getSampleRate(),
                audioRecordRecorder.getChannelConfiguration(),
                128 * 1024);
        outputStream = new FileOutputStream(aacFilePath);
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

        if(audioEncodeEncoder != null){
            audioEncodeEncoder.stop();
            audioEncodeEncoder = null;
        }

        if(outputStream != null){
            try {
                outputStream.close();
                outputStream = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    //async 执行
    @Override
    public void outputPCMPacketInByte(byte[] data) {
        audioEncodeEncoder.fireAudio(data, data.length);
//        Log.e(TAG, "PCM Data:" + Arrays.toString(data));
    }

    @Override
    public void outputPCMPacketInShort(short[] data) {

    }

    @Override
    public void outputAACPacket(byte[] data) {
        // 返回　AAC　数据
//        Log.e(TAG, "AAC Data:" + Arrays.toString(data));

        try {
            outputStream.write(data);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
