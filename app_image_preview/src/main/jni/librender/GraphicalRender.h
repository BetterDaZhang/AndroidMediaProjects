//
// Created by zhangjunling on 18-8-9.
//

#ifndef ANDROIDMEDIAPROJECTS_GRAPHICALRENDER_H
#define ANDROIDMEDIAPROJECTS_GRAPHICALRENDER_H

#include "../libcommon/GLHelper.h"

class GraphicalRender {
public:
    GraphicalRender();
    virtual ~GraphicalRender();

    GLint mProgram;
    GLint mUProjectionMatrixLoc;
    GLint mULookatMatrixLoc;
    GLint mURotationMatrixLoc;
    GLint mAPositionLoc;
    GLint mAColorLoc;

    float* projectionMatrix;
    float* lookatMatrix;
    float* rotationMatrix;

    GLfloat* mVertexArray;
    GLfloat* mColorArray;

    void onSurfaceCreated(const char* vertexShaderCode, const char* fragmentShaderCode);
    void onSurfaceChange(int width,int height);
    void onDrawFrame();

private:
    void init(const char* vertexShaderCode, const char* fragmentShaderCode);

};


#endif //ANDROIDMEDIAPROJECTS_GRAPHICALRENDER_H
