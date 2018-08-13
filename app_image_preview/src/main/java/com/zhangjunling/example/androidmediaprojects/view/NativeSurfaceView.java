package com.zhangjunling.example.androidmediaprojects.view;

import android.content.Context;
import android.os.Environment;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class NativeSurfaceView extends SurfaceView implements SurfaceHolder.Callback{

    public NativeSurfaceView(Context context) {
        this(context,null);
    }

    public NativeSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public NativeSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        String imagePath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "a.png";
        start(imagePath);
        setWindow(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        stop();
    }

    public native void start(String imageFilePath);
    public native void setWindow(Surface surface);
    public native void stop();
}
