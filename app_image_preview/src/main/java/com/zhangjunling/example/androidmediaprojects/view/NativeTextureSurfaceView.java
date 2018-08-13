package com.zhangjunling.example.androidmediaprojects.view;

import android.content.Context;
import android.os.Environment;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class NativeTextureSurfaceView extends SurfaceView implements SurfaceHolder.Callback{
    public NativeTextureSurfaceView(Context context) {
        this(context,null);
    }

    public NativeTextureSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public NativeTextureSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        String imagePath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "a.png";
        start(imagePath);
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        requestSurfaceChanged(holder.getSurface(), width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        requestExit();
    }

    private native void start(String filename);
    public native void stop();


    private native void requestSurfaceChanged(Surface surface,int width, int height);
    private native void requestExit();
}
