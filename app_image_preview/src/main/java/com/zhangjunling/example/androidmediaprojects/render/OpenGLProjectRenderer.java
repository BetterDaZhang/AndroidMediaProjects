package com.zhangjunling.example.androidmediaprojects.render;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.zhangjunling.example.androidmediaprojects.R;
import com.zhangjunling.example.androidmediaprojects.util.LoggerConfig;
import com.zhangjunling.example.androidmediaprojects.util.MatrixHelper;
import com.zhangjunling.example.androidmediaprojects.util.ShaderHelper;
import com.zhangjunling.example.androidmediaprojects.util.TextResourceReader;
import com.zhangjunling.example.androidmediaprojects.util.TextureHelper;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES20.GL_FLOAT;
import static android.opengl.GLES20.GL_LINES;
import static android.opengl.GLES20.GL_POINTS;
import static android.opengl.GLES20.GL_TEXTURE0;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_TRIANGLE_FAN;
import static android.opengl.GLES20.glActiveTexture;
import static android.opengl.GLES20.glBindTexture;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glUniform1i;
import static android.opengl.GLES20.glUniformMatrix4fv;
import static android.opengl.GLES20.glUseProgram;
import static android.opengl.Matrix.multiplyMM;
import static android.opengl.Matrix.rotateM;
import static android.opengl.Matrix.setIdentityM;
import static android.opengl.Matrix.translateM;

public class OpenGLProjectRenderer implements GLSurfaceView.Renderer {

    private static final String A_POSITION = "a_Position";
    private static final String A_COLOR = "a_Color";
    private static final String U_MATRIX = "u_Matrix";
    private static final String A_TEXTURE_COORDINATES = "a_TextureCoordinates";
    protected static final String U_TEXTURE_UNIT = "u_TextureUnit";


    private static final int POSITION_COMPONENT_COUNT = 2;
    private static final int COLOR_COMPONENT_COUNT = 3;
    private static final int BYTES_PER_FLOAT = 4;
    private static final int STRIDE = (POSITION_COMPONENT_COUNT + COLOR_COMPONENT_COUNT) * BYTES_PER_FLOAT;
    private final float[] projectionMatrix = new float[16];
    private final float[] modelMatrix = new float[16];
    private Context context;
    private int colorProgram;
    private int tableProgram;

    private final FloatBuffer colorVertexData;
    private int color_aPositionLocation;
    private int color_aColorLocation;
    private int color_uMatrixLocation;


    private final FloatBuffer tableVertexData;
    private int texture;
    private int table_aPositionLocation;
    private int tabler_aTextureCoordinatesLocation;
    private int table_uMatrixLocation;
    private int table_uTextureUnit;

    public OpenGLProjectRenderer(Context context){
        this.context = context;

        float[] colorVerticesWithTriangles = {
                // Order of coordinates: X, Y, R, G, B
                0f, -0.4f, 0f, 0f, 1f,
                0f,  0.4f, 1f, 0f, 0f
        };
        colorVertexData = ByteBuffer
                .allocateDirect(colorVerticesWithTriangles.length * BYTES_PER_FLOAT)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        colorVertexData.put(colorVerticesWithTriangles);



        float[] tableVerticesWithTriangles = {
                // Order of coordinates: X, Y, S, T
                0f,    0f, 0.5f, 0.5f,
                -0.5f, -0.8f,   0f, 0.9f,
                0.5f, -0.8f,   1f, 0.9f,
                0.5f,  0.8f,   1f, 0.1f,
                -0.5f,  0.8f,   0f, 0.1f,
                -0.5f, -0.8f,   0f, 0.9f
        };
        tableVertexData = ByteBuffer
                .allocateDirect(tableVerticesWithTriangles.length * BYTES_PER_FLOAT)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        tableVertexData.put(tableVerticesWithTriangles);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.0f,0.0f,0.0f,0.0f);

        String colorVertexShaderSource = TextResourceReader
                .readTextFileFromResource(context, R.raw.simple_vertex_shader);
        String colorFragmentShaderSource = TextResourceReader
                .readTextFileFromResource(context, R.raw.simple_fragment_shader);
        int vertexShader = ShaderHelper.compileVertexShader(colorVertexShaderSource);
        int fragmentShader = ShaderHelper.compileFragmentShader(colorFragmentShaderSource);
        colorProgram = ShaderHelper.linkProgram(vertexShader, fragmentShader);
        if (LoggerConfig.ON) {
            ShaderHelper.validateProgram(colorProgram);
        }
        glUseProgram(colorProgram);
        color_aPositionLocation = GLES20.glGetAttribLocation(colorProgram, A_POSITION);
        color_aColorLocation = GLES20.glGetAttribLocation(colorProgram, A_COLOR);
        color_uMatrixLocation = GLES20.glGetUniformLocation(colorProgram, U_MATRIX);


        texture = TextureHelper.loadTexture(context, R.drawable.air_hockey_surface);
        String tableVertexShaderSource = TextResourceReader
                .readTextFileFromResource(context, R.raw.texture_vertex_shader);
        String tableFragmentShaderSource = TextResourceReader
                .readTextFileFromResource(context, R.raw.texture_fragment_shader);
        int tableVertexShader = ShaderHelper.compileVertexShader(tableVertexShaderSource);
        int tableFragmentShader = ShaderHelper.compileFragmentShader(tableFragmentShaderSource);
        tableProgram = ShaderHelper.linkProgram(tableVertexShader, tableFragmentShader);
        if (LoggerConfig.ON) {
            ShaderHelper.validateProgram(colorProgram);
        }
        glUseProgram(tableProgram);

        table_aPositionLocation = GLES20.glGetAttribLocation(tableProgram, "a_Position");
        tabler_aTextureCoordinatesLocation = GLES20.glGetAttribLocation(tableProgram, "a_TextureCoordinates");
        table_uMatrixLocation = GLES20.glGetUniformLocation(tableProgram, "u_Matrix");
        table_uTextureUnit = GLES20.glGetUniformLocation(tableProgram, "u_TextureUnit");


    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0,0,width,height);

    /*
        //正交投影
        final float aspectRatio = width > height ? (float) width / (float) height : (float) height / (float) width;

        if (width > height) {
            // Landscape
            Matrix.orthoM(projectionMatrix, 0, -aspectRatio, aspectRatio, -1f, 1f, -1f, 1f);
        } else {
            // Portrait or square
            Matrix.orthoM(projectionMatrix, 0, -1f, 1f, -aspectRatio, aspectRatio, -1f, 1f);
        }
    */

        //透视投影
        MatrixHelper.perspectiveM(projectionMatrix, 45, (float) width / (float) height, 1f, 10f);

//        setIdentityM(modelMatrix, 0);
//        translateM(modelMatrix, 0, 0f, 0f, -2f);

        setIdentityM(modelMatrix, 0);
        translateM(modelMatrix, 0, 0f, 0f, -2.5f);
        rotateM(modelMatrix, 0, -60f, 1f, 0f, 0f);

        final float[] temp = new float[16];
        multiplyMM(temp, 0, projectionMatrix, 0, modelMatrix, 0);
        System.arraycopy(temp, 0, projectionMatrix, 0, temp.length);

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        /**
         * 提示：color/table　glVertexAttribPointer()最好放在onDrawFrame()中,否则可能会出现莫名其妙的渲染问题;
         */

        glUseProgram(tableProgram);
        tableVertexData.position(0);
        GLES20.glVertexAttribPointer(table_aPositionLocation, 2, GL_FLOAT, false, (2 + 2) * 4, tableVertexData);
        tableVertexData.position(2);
        GLES20.glVertexAttribPointer(tabler_aTextureCoordinatesLocation, 2, GL_FLOAT, false, (2 + 2) * 4, tableVertexData);
        GLES20.glEnableVertexAttribArray(table_aPositionLocation);
        GLES20.glEnableVertexAttribArray(tabler_aTextureCoordinatesLocation);

        glUniformMatrix4fv(table_uMatrixLocation, 1, false, projectionMatrix, 0);
        // Set the active texture unit to texture unit 0.
        glActiveTexture(GL_TEXTURE0);
        // Bind the texture to this unit.
        glBindTexture(GL_TEXTURE_2D, texture);
        // Tell the texture uniform sampler to use this texture in the shader by
        // telling it to read from texture unit 0.
        glUniform1i(table_uTextureUnit, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);


        glUseProgram(colorProgram);
        colorVertexData.position(0);
        GLES20.glVertexAttribPointer(color_aPositionLocation,POSITION_COMPONENT_COUNT,GL_FLOAT,false,STRIDE,colorVertexData);
        GLES20.glEnableVertexAttribArray(color_aPositionLocation);
        colorVertexData.position(POSITION_COMPONENT_COUNT);
        GLES20.glVertexAttribPointer(color_aColorLocation, COLOR_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, colorVertexData);
        GLES20.glEnableVertexAttribArray(color_aColorLocation);
        GLES20.glUniformMatrix4fv(color_uMatrixLocation,1,false,projectionMatrix,0);
        glDrawArrays(GL_POINTS, 0, 2);
    }
}
