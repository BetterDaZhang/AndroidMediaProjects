package com.zhangjunling.example.androidmediaprojects.render;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.zhangjunling.example.androidmediaprojects.R;
import com.zhangjunling.example.androidmediaprojects.util.TextResourceReader;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GraphicalRender implements GLSurfaceView.Renderer {

    private Context context;
    public GraphicalRender(Context context){
        this.context = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
//        GLES20.glClearColor(1.0f,1.0f,1.0f,1.0f);
        String vertexShaderCode = TextResourceReader.readTextFileFromResource(context, R.raw.simple_native_vertex_graphical_shader);
        String fragmentShaderCode = TextResourceReader.readTextFileFromResource(context, R.raw.simple_native_fragment_graphical_shader);
        onSurfaceCrerated(vertexShaderCode, fragmentShaderCode);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
//        GLES20.glViewport(0, 0, width, height);
        onSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
//        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        onDrawFrame();
    }

    private native void onSurfaceCrerated(String vertexShaderCode, String fragmentShaderCode);
    private native void onSurfaceChanged(int width, int height);
    private native void onDrawFrame();
}
