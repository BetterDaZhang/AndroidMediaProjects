//
// Created by zhangjunling on 18-8-9.
//

#ifndef ANDROIDMEDIAPROJECTS_GLHELPER_H
#define ANDROIDMEDIAPROJECTS_GLHELPER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "CommonTools.h"

class GLHelper {
public:
    static int compileShader(int type, const char* shaderCode);
    static int createProgram(const char * vertexShaderCode, const char * fragmentShaderCode);
};


#endif //ANDROIDMEDIAPROJECTS_GLHELPER_H
