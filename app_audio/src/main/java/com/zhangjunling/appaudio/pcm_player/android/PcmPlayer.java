package com.zhangjunling.appaudio.pcm_player.android;

import android.media.AudioTrack;
import android.util.Log;

/**
 * 设计思想:PCMPlayer就是一个消费pcm数据的消费者;
 */
public class PcmPlayer {
    public static final String TAG = "PcmPlayer";

    private AudioTrack mAudioTrack;
    private int mSampleRateInHz;
    private int mChannelConfig;
    private int mAudioFormat;
    private int mAudioStreamType;

    private boolean isPlaying = false;

    public void initAudioTrack(int sampleRateInHz, int channelConfig, int audioFormat, int audioStreamType) {
        this.mSampleRateInHz = sampleRateInHz;
        this.mChannelConfig = channelConfig;
        this.mAudioFormat = audioFormat;
        this.mAudioStreamType = audioStreamType;
        this.isPlaying = false;

        int bufferSize = AudioTrack.getMinBufferSize(mSampleRateInHz, mChannelConfig, mAudioFormat);
        mAudioTrack = new AudioTrack(mAudioStreamType, mSampleRateInHz, mChannelConfig, mAudioFormat, bufferSize,
                AudioTrack.MODE_STREAM);
    }

    public void startPlay(){
        try {
            if (null != mAudioTrack) {
                mAudioTrack.play();
                isPlaying = true;
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
        isPlaying = true;

    }

    public void stopPlay(){
        if (isPlaying && null != mAudioTrack) {
            if (null != mAudioTrack && mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
                try {
                    mAudioTrack.stop();
                } catch (Throwable t) {
                    t.printStackTrace();
                }
            }
            isPlaying = false;
            closeAudioTrack();
        }
    }

    private void closeAudioTrack() {
        try {
            if (null != mAudioTrack && mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
                mAudioTrack.release();
                mAudioTrack = null;
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    /**
     * 该方法在线程中执行.
     */
    public void playAudioData(byte[] audioData, int offsetInBytes, int sizeInBytes){
        if (isPlaying && null != mAudioTrack && mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
//            mAudioTrack.setStereoVolume(1, 1);
            if (mAudioTrack.write(audioData, offsetInBytes, sizeInBytes) != sizeInBytes) {
                Log.e(TAG, "Could not write all the samples to the audio device !");
            }
        }
    }

}
