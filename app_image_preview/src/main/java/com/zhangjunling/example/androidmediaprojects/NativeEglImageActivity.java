package com.zhangjunling.example.androidmediaprojects;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.zhangjunling.example.androidmediaprojects.view.NativeEGLSurfaceView;

public class NativeEglImageActivity extends AppCompatActivity {

    private NativeEGLSurfaceView nativeEGLSurfaceView;

    static {
        System.loadLibrary("PngDecodePreview");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        nativeEGLSurfaceView = new NativeEGLSurfaceView(this);
        setContentView(nativeEGLSurfaceView);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeEGLSurfaceView.stop();
    }
}
