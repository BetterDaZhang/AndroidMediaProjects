package com.zhangjunling.appaudio;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    private Button mAudioRecordRecorderView;
    private Button mOpenSLRecorderView;
    private Button mAudioTrackPlayerView;
    private Button mOpenSLPlayerView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_audio);
        mAudioRecordRecorderView = this.findViewById(R.id.bt_audiorecord_recorder);
        mOpenSLRecorderView = this.findViewById(R.id.bt_opensl_recorder);
        mAudioTrackPlayerView = this.findViewById(R.id.bt_audiotrack_plalyer);
        mOpenSLPlayerView = this.findViewById(R.id.bt_opensl_player);

        mAudioRecordRecorderView.setOnClickListener(this);
        mOpenSLRecorderView.setOnClickListener(this);
        mAudioTrackPlayerView.setOnClickListener(this);
        mOpenSLPlayerView.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_audiorecord_recorder:
                Intent audioRecordRecorderIntent = new Intent(MainActivity.this, AudioRecorderActivity.class);
                startActivity(audioRecordRecorderIntent);
                break;
            case R.id.bt_opensl_recorder:
                Intent openslRecorderIntent = new Intent(MainActivity.this, OpenSLRecordActivity.class);
                startActivity(openslRecorderIntent);
                break;
            case R.id.bt_audiotrack_plalyer:
                Intent audioTrackPlayerIntent = new Intent(MainActivity.this, PcmPlayerActivity.class);
                startActivity(audioTrackPlayerIntent);
                break;
            case R.id.bt_opensl_player:
                Intent openslPlayerIntent = new Intent(MainActivity.this, OpenSLPlayerActivity.class);
                startActivity(openslPlayerIntent);
                break;
        }
    }
}
