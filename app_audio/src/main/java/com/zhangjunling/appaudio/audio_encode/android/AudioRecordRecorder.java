package com.zhangjunling.appaudio.audio_encode.android;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

import com.zhangjunling.appaudio.pcm_recorder.AudioConfigurationException;
import com.zhangjunling.appaudio.pcm_recorder.StartRecordingException;

public class AudioRecordRecorder {
    private static final String TAG = "AudioRecordRecorder";
    public static final int BUFFER_TYPE_BYTE = 0x0001;
    public static final int BUFFER_TYPE_SHORT = 0x0002;


    protected AudioRecord audioRecord;
    private Thread recordThread;

    private int AUDIO_SOURCE = MediaRecorder.AudioSource.MIC;
    public static int SAMPLE_RATE_IN_HZ = 44100;
    private final static int CHANNEL_CONFIGURATION = AudioFormat.CHANNEL_IN_MONO;
    private final static int AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;

    private int bufferSizeInBytes = 0;
    private boolean isRecording = false;
    private OutputPCMDelegate outputPCMDelegate;
    private int bufferType = BUFFER_TYPE_BYTE;

    public void initMetaData(OutputPCMDelegate outputPCMDelegate,int bufferType) throws AudioConfigurationException {
        this.outputPCMDelegate = outputPCMDelegate;
        this.bufferType = bufferType;

        if (null != audioRecord) {
            audioRecord.release();
        }
        try {
            // 首先利用我们标准的44_1K的录音频率初是录音器
            bufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLE_RATE_IN_HZ, CHANNEL_CONFIGURATION, AUDIO_FORMAT);
            Log.e(TAG, "bufferSizeInBytes:" + bufferSizeInBytes);
            audioRecord = new AudioRecord(AUDIO_SOURCE, SAMPLE_RATE_IN_HZ, CHANNEL_CONFIGURATION, AUDIO_FORMAT,
                    bufferSizeInBytes);
        } catch (Exception e) {
            e.printStackTrace();
        }
        // 如果初始化不成功的话,则降低为16K的采样率来初始化录音器
        if (audioRecord == null || audioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
            try {
                SAMPLE_RATE_IN_HZ = 16000;
                bufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLE_RATE_IN_HZ, CHANNEL_CONFIGURATION,
                        AUDIO_FORMAT);
                audioRecord = new AudioRecord(AUDIO_SOURCE, SAMPLE_RATE_IN_HZ, CHANNEL_CONFIGURATION, AUDIO_FORMAT,
                        bufferSizeInBytes);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        if (audioRecord == null || audioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
            throw new AudioConfigurationException();
        }
    }

    public void start() throws StartRecordingException {
        if (audioRecord != null && audioRecord.getState() == AudioRecord.STATE_INITIALIZED) {
            try {
                audioRecord.startRecording();
            } catch (Exception e) {
                throw new StartRecordingException();
            }
        } else {
            throw new StartRecordingException();
        }
        isRecording = true;
        recordThread = new Thread(new RecordThread(), "RecordThread");
        try {
            recordThread.start();
        } catch (Exception e) {
            throw new StartRecordingException();
        }
    }

    public void stop() {
        try {
            if (audioRecord != null) {
                isRecording = false;
                try {
                    if (recordThread != null) {
                        recordThread.join();
                        recordThread = null;
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                releaseAudioRecord();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void releaseAudioRecord() {
        if (audioRecord.getState() == AudioRecord.STATE_INITIALIZED)
            audioRecord.stop();
        audioRecord.release();
        audioRecord = null;
    }


    private byte[] audioSamplesInByte;
    private short[] audioSamplesInShort;
    class RecordThread implements Runnable {

        @Override
        public void run() {
            if(bufferType == BUFFER_TYPE_BYTE){
                audioSamplesInByte = new byte[bufferSizeInBytes];
            }else if(bufferType == BUFFER_TYPE_SHORT){
                audioSamplesInShort = new short[bufferSizeInBytes / 2];
            }
            while (isRecording) {
                int audioSampleSize = 0;
                if(bufferType == BUFFER_TYPE_BYTE){
                    audioSampleSize = getAudioRecordBuffer(bufferSizeInBytes, audioSamplesInByte);
                }else if(bufferType == BUFFER_TYPE_SHORT){
                    audioSampleSize = getAudioRecordBuffer(bufferSizeInBytes / 2, audioSamplesInShort);
                }

                if (audioSampleSize > 0) {
                    //TODO ... 回调
                    if(null != outputPCMDelegate){
                        if(bufferType == BUFFER_TYPE_BYTE){
                            outputPCMDelegate.outputPCMPacketInByte(audioSamplesInByte);
                        }else if(bufferType == BUFFER_TYPE_SHORT){
                            outputPCMDelegate.outputPCMPacketInShort(audioSamplesInShort);
                        }
                    }

                }
            }
        }
    }

    protected int getAudioRecordBuffer(int bufferSize, byte[] audioSamples) {
        if (audioRecord != null) {
            int size = audioRecord.read(audioSamples, 0, bufferSize);
            return size;
        } else {
            return 0;
        }
    }

    protected int getAudioRecordBuffer(int bufferSize, short[] audioSamples) {
        if (audioRecord != null) {
            int size = audioRecord.read(audioSamples, 0, bufferSize);
            return size;
        } else {
            return 0;
        }
    }

    public int getSampleRate() {
        return SAMPLE_RATE_IN_HZ;
    }

    public int getChannelConfiguration(){
        int channels = 0;
        if(CHANNEL_CONFIGURATION == AudioFormat.CHANNEL_IN_MONO){
            channels = 1;
        }else if(CHANNEL_CONFIGURATION == AudioFormat.CHANNEL_IN_STEREO){
            channels = 2;
        }

        return channels;
    }

    public int getSampleFormat(){
        return AUDIO_FORMAT;
    }
}
