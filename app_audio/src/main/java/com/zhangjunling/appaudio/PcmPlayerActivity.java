package com.zhangjunling.appaudio;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.zhangjunling.appaudio.pcm_player.PcmPlayControl;

public class PcmPlayerActivity extends AppCompatActivity implements View.OnClickListener{

    private Button mStartPlayerView;
    private Button mStopPlayerView;

    private PcmPlayControl pcmPlayControl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pcm_player);

        mStartPlayerView = this.findViewById(R.id.bt_start_player);
        mStopPlayerView = this.findViewById(R.id.bt_stop_player);

        mStartPlayerView.setOnClickListener(this);
        mStopPlayerView.setOnClickListener(this);

        pcmPlayControl = new PcmPlayControl();

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_start_player:
                pcmPlayControl.preparePlay(PcmPlayControl.DEFAULT_FILE_PATH,
                        PcmPlayControl.DEFAULT_SAMPLE_RATE,
                        PcmPlayControl.DEFAULT_CHANNEL_CONFIG,
                        PcmPlayControl.DEFAULT_AUDIO_FORMAT,
                        PcmPlayControl.DEFAULT_STREAM_TYPE);
                pcmPlayControl.start();
                break;

            case R.id.bt_stop_player:
                pcmPlayControl.stop();
                break;
        }
    }
}
