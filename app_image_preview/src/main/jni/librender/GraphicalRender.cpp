//
// Created by zhangjunling on 18-8-9.
//

#include "GraphicalRender.h"
#include "libglm/mat4x4.hpp"
#include "libglm/ext.hpp"

glm::mat4 projection;
glm::mat4 view;
glm::mat4 module;

GraphicalRender::GraphicalRender(){

}

GraphicalRender::~GraphicalRender(){

}

void GraphicalRender::onSurfaceCreated(const char* vertexShaderCode, const char* fragmentShaderCode){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    init(vertexShaderCode,fragmentShaderCode);
}

void GraphicalRender::onSurfaceChange(int width,int height){
    LOGI("GraphicalRender::onSurfaceChange()   <----------------");
    glViewport(0, 0, width, height);

//    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    projection = glm::perspective(glm::radians(50.0f), (float)width/height, 1.0f ,7.0f);

    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));

    module = glm::rotate(module, 90.0f, glm::vec3(1,0,0));

    LOGI("GraphicalRender::onSurfaceChange()   ---------------->");
}

void GraphicalRender::onDrawFrame(){
    LOGI("GraphicalRender::onDrawFrame()   <----------------");
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);

    glVertexAttribPointer(mAPositionLoc, 2, GL_FLOAT, false, 0, mVertexArray);
    glVertexAttribPointer(mAColorLoc, 3, GL_FLOAT, false, 0, mColorArray);
    glEnableVertexAttribArray(mAPositionLoc);
    glEnableVertexAttribArray(mAColorLoc);


    /**
     * 这部分代码，测试发现只能在onDrawFrame设置,onSurfaceCreated设置会失败!
     */
//    glm::mat4 mvpMatrix = projection * view * module;
    projectionMatrix = (float*) glm::value_ptr(projection);
    lookatMatrix = (float*) glm::value_ptr(view);
    rotationMatrix = (float *) glm::value_ptr(module);

    glUniformMatrix4fv(mUProjectionMatrixLoc, 1, false, projectionMatrix);
    glUniformMatrix4fv(mULookatMatrixLoc, 1, false, lookatMatrix);
    glUniformMatrix4fv(mURotationMatrixLoc, 1, false, rotationMatrix);

    // Draw the table.
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    // Draw the center dividing line.
    glDrawArrays(GL_LINES, 6, 2);

    // Draw the first mallet.
    glDrawArrays(GL_POINTS, 8, 1);

    // Draw the second mallet.
    glDrawArrays(GL_POINTS, 9, 1);

    glFinish();
    glUseProgram(0);

//    if(mVertexArray != NULL){
//        delete mVertexArray;
//        mVertexArray = NULL;
//    }
//
//    if(mColorArray != NULL){
//        delete mColorArray;
//        mColorArray = NULL;
//    }

//    if(projectionMatrix != NULL){
//        delete projectionMatrix;
//        projectionMatrix = NULL;
//    }
//
//    if(lookatMatrix != NULL){
//        delete lookatMatrix;
//        lookatMatrix = NULL;
//    }
//
//    if(rotationMatrix != NULL){
//        delete rotationMatrix;
//        rotationMatrix = NULL;
//    }
    LOGI("GraphicalRender::onDrawFrame()   ---------------->");
}

void GraphicalRender::init(const char* vertexShaderCode, const char* fragmentShaderCode){
    LOGI("GraphicalRender::init()   <----------------");
    mVertexArray = new GLfloat[20]{
            0.0f,    0.0f,
            -0.5f, -0.8f,
            0.5f, -0.8f,
            0.5f,  0.8f,
            -0.5f,  0.8f,
            -0.5f, -0.8f,

            // Line 1
            -0.5f, 0.0f,
            0.5f, 0.0f,

            // Mallets
            0.0f, -0.4f,
            0.0f,  0.4f,
    };

    mColorArray = new GLfloat[30]{
            1.0f, 1.0f, 1.0f,
            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f};

    mProgram = GLHelper::createProgram(vertexShaderCode, fragmentShaderCode);

    mUProjectionMatrixLoc = glGetUniformLocation(mProgram, "u_ProjectionMatrix");
    mULookatMatrixLoc = glGetUniformLocation(mProgram, "u_LookAtMatrix");
    mURotationMatrixLoc = glGetUniformLocation(mProgram, "u_RotationMatrix");
    mAPositionLoc = glGetAttribLocation(mProgram, "a_Position");
    mAColorLoc = glGetAttribLocation(mProgram, "a_Color");

    LOGI("GraphicalRender::init()   ---------------->");
}