package com.zhangjunling.appaudio;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.zhangjunling.appaudio.audio_encode.natived.AudioFFmpegEncodeController;
import com.zhangjunling.appaudio.pcm_recorder.AudioConfigurationException;
import com.zhangjunling.appaudio.pcm_recorder.StartRecordingException;

import java.io.IOException;

public class AudioFFmpegEncodeActivity extends AppCompatActivity implements View.OnClickListener{

    private Button mStartFFmpegDecodeView;
    private Button mStopFFmpegDecodeView;

    private AudioFFmpegEncodeController audioFFmpegEncodeController;

    static {
        System.loadLibrary("native_audio");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_ffmpeg_encode);
        mStartFFmpegDecodeView = this.findViewById(R.id.bt_start_audio_encoder_ffmpeg);
        mStopFFmpegDecodeView = this.findViewById(R.id.bt_stop_audio_encoder_ffmpeg);

        mStartFFmpegDecodeView.setOnClickListener(this);
        mStopFFmpegDecodeView.setOnClickListener(this);

        audioFFmpegEncodeController = new AudioFFmpegEncodeController();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_start_audio_encoder_ffmpeg:
                try {
                    audioFFmpegEncodeController.prepare();
                    audioFFmpegEncodeController.start();
                } catch (AudioConfigurationException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (StartRecordingException e) {
                    e.printStackTrace();
                }
                break;
            case R.id.bt_stop_audio_encoder_ffmpeg:
                audioFFmpegEncodeController.stop();
                break;
        }
    }
}
