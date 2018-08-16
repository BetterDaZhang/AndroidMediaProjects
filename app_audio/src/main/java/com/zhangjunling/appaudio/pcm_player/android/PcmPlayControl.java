package com.zhangjunling.appaudio.pcm_player.android;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.os.Environment;

import java.io.File;
import java.io.IOException;


/**
 * pcm播放器的控制系统:控制PcmFileReader与PcmPlayer之间的业务逻辑.
 *
 */
public class PcmPlayControl {
    private static final int SAMPLES_PER_FRAME = 1024;
    private volatile boolean mIsTestingExit = false;

    public static final int DEFAULT_STREAM_TYPE = AudioManager.STREAM_MUSIC;
    public static final int DEFAULT_SAMPLE_RATE = 44100;
    public static final int DEFAULT_CHANNEL_CONFIG = AudioFormat.CHANNEL_OUT_MONO;
    public static final int DEFAULT_AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;
    public static final String DEFAULT_FILE_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "audio.pcm";

    private PcmFileReader pcmFileReader;
    private PcmPlayer pcmPlayer;

    public PcmPlayControl() {

    }

    public void preparePlay(String filePath, int sampleRateInHz, int channelConfig, int audioFormat, int audioStreamType){
        pcmFileReader = new PcmFileReader();
        pcmPlayer = new PcmPlayer();

        mIsTestingExit = false;

        try {
            pcmFileReader.openFile(filePath);
            pcmPlayer.initAudioTrack(sampleRateInHz, channelConfig, audioFormat, audioStreamType);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void start(){
        pcmPlayer.startPlay();
        new Thread(audioPlayRunnable).start();
    }

    public void stop(){
        mIsTestingExit = true;
    }

    private Runnable audioPlayRunnable = new Runnable() {
        @Override
        public void run() {
            byte[] buffer = new byte[SAMPLES_PER_FRAME];
            while (!mIsTestingExit && pcmFileReader.readData(buffer, 0, buffer.length) > 0) {
                pcmPlayer.playAudioData(buffer, 0, buffer.length);
            }
            pcmPlayer.stopPlay();
            try {
                pcmFileReader.closeFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

}
