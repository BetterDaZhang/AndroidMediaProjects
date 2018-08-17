package com.zhangjunling.appaudio;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class OpenSLPlayerActivity extends AppCompatActivity implements View.OnClickListener{

    private Button mStartView;
    private Button mStopView;

    static {
        System.loadLibrary("native_audio");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_pcm_player);
        mStartView = this.findViewById(R.id.bt_start_player);
        mStopView = this.findViewById(R.id.bt_stop_player);

        mStartView.setOnClickListener(this);
        mStopView.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_start_player:
                opensl_start();
                break;
            case R.id.bt_stop_player:
                opensl_stop();
                break;
        }
    }

    private native void opensl_start();
    private native void opensl_stop();
}
