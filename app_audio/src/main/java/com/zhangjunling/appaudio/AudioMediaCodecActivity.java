package com.zhangjunling.appaudio;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.zhangjunling.appaudio.audio_encode.AudioEncodeController;
import com.zhangjunling.appaudio.pcm_recorder.AudioConfigurationException;
import com.zhangjunling.appaudio.pcm_recorder.StartRecordingException;

import java.io.IOException;

public class AudioMediaCodecActivity extends AppCompatActivity implements View.OnClickListener{

    private AudioEncodeController audioEncodeController;

    private Button mStartEncodeView;
    private Button mStopEncodeView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_media_codec);
        mStartEncodeView = this.findViewById(R.id.bt_start_audio_mediccodec);
        mStopEncodeView = this.findViewById(R.id.bt_stop_audio_mediccodec);

        audioEncodeController = new AudioEncodeController();

        mStartEncodeView.setOnClickListener(this);
        mStopEncodeView.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_start_audio_mediccodec:
                try {
                    audioEncodeController.prepare();
                    audioEncodeController.start();
                } catch (AudioConfigurationException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (StartRecordingException e) {
                    e.printStackTrace();
                }

                break;
            case R.id.bt_stop_audio_mediccodec:
                audioEncodeController.stop();
                break;
        }
    }
}
