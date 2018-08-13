package com.zhangjunling.example.androidmediaprojects.view;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.zhangjunling.example.androidmediaprojects.render.GraphicalRender;

public class GraphicalGLSurfaceView extends GLSurfaceView {
    public GraphicalGLSurfaceView(Context context) {
        this(context,null);
    }

    public GraphicalGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.setEGLContextClientVersion(2);
        this.setRenderer(new GraphicalRender(getContext()));
        this.setRenderMode(RENDERMODE_WHEN_DIRTY);
    }
}
