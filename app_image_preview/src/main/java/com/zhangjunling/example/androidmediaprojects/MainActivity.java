package com.zhangjunling.example.androidmediaprojects;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    private Button imagePreviewView;
    private Button nativePreviewView;
    private Button glImagePreviewView;
    private Button nativeGraphicalView;
    private Button nativeEGLGraphicalView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imagePreviewView   = this.findViewById(R.id.bt_image_preview);
        nativePreviewView  = this.findViewById(R.id.bt_native_image_preview);
        glImagePreviewView = this.findViewById(R.id.bt_gl_image_preview);
        nativeGraphicalView = this.findViewById(R.id.bt_native_graphical);
        nativeEGLGraphicalView = this.findViewById(R.id.bt_native_egl_graphical);

        imagePreviewView.setOnClickListener(this);
        nativePreviewView.setOnClickListener(this);
        glImagePreviewView.setOnClickListener(this);
        nativeGraphicalView.setOnClickListener(this);
        nativeEGLGraphicalView.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_image_preview:
                Intent imageIntent = new Intent(MainActivity.this,ImagePreviewActivity.class);
                startActivity(imageIntent);
                break;

            case R.id.bt_native_image_preview:
                Intent nativeImageIntent = new Intent(MainActivity.this,NativeImagePreviewActivity.class);
                startActivity(nativeImageIntent);
                break;
            case R.id.bt_gl_image_preview:
                Intent glImageIntent = new Intent(MainActivity.this,GLImagePreviewActivity.class);
                startActivity(glImageIntent);
                break;
            case R.id.bt_native_graphical:
                Intent nativeGraphicalIntent = new Intent(MainActivity.this,NativeGraphicalActivity.class);
                startActivity(nativeGraphicalIntent);
                break;
            case R.id.bt_native_egl_graphical:
                Intent nativeEGLGraphicalIntent = new Intent(MainActivity.this,NativeEglImageActivity.class);
                startActivity(nativeEGLGraphicalIntent);
                break;
        }
    }
}
