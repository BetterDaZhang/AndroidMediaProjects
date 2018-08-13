package com.zhangjunling.example.androidmediaprojects;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.zhangjunling.example.androidmediaprojects.view.GraphicalGLSurfaceView;

public class NativeGraphicalActivity extends AppCompatActivity {

    private GraphicalGLSurfaceView graphicalGLSurfaceView;

    static {
        System.loadLibrary("PngDecodePreview");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        graphicalGLSurfaceView = new GraphicalGLSurfaceView(this);
        setContentView(graphicalGLSurfaceView);
    }
}
