package com.zhangjunling.example.androidmediaprojects.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class NativeEGLSurfaceView extends SurfaceView implements SurfaceHolder.Callback{
    public NativeEGLSurfaceView(Context context) {
        this(context, null);
    }

    public NativeEGLSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public NativeEGLSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        start();
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
//        requestInitEGL();
//        requestSurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        requestSurfaceChanged(holder.getSurface(),width, height);
//        requestOnDrawFrame();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        requestExit();
    }

    private native void start();
    public native void stop();

    private native void requestInitEGL();
    private native void requestSurfaceCreated(Surface surface);
    private native void requestSurfaceChanged(Surface surface,int width, int height);
    private native void requestOnDrawFrame();
    private native void requestExit();

    /**
     * 总结：
     *  如果我们先后执行的顺序为：
     *     １.start();
     *     2.requestInitEGL();
     *     3.requestSurfaceCreated(holder.getSurface());
     *     4.requestSurfaceChanged(width, height);
     *     5.requestOnDrawFrame();
     *     6.requestExit();
     *
     *     会出现只执行５的现象;所以不能这样调用;
     *     造成这种现象的原因是：发送signal时子线程不能及时的抢占资源;
     *     所以一直执行到onDrawFrame子线程才开始执行;造成之前的额方法不能执行,产生bug;
     *     以后通过设计类似android的Handler+Message可以解决这个问题;
     *
     */

}
