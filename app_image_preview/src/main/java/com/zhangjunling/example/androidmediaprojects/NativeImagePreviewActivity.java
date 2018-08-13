package com.zhangjunling.example.androidmediaprojects;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class NativeImagePreviewActivity extends AppCompatActivity {


    static {
        System.loadLibrary("PngDecodePreview");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_image_native);
    }
}
