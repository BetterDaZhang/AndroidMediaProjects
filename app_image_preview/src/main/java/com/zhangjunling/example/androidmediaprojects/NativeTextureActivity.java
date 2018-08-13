package com.zhangjunling.example.androidmediaprojects;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.zhangjunling.example.androidmediaprojects.view.NativeTextureSurfaceView;

public class NativeTextureActivity extends AppCompatActivity {

    private NativeTextureSurfaceView nativeTextureSurfaceView;

    static {
        System.loadLibrary("PngDecodePreview");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        nativeTextureSurfaceView = new NativeTextureSurfaceView(this);
        setContentView(nativeTextureSurfaceView);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeTextureSurfaceView.stop();
    }
}
