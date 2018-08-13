//
// Created by zhangjunling on 18-8-13.
//

#include "TexturePreviewRender.h"

static const char* fragmentShaderCode =
        "varying highp vec2 v_texcoord;\n"
         "uniform sampler2D yuvTexSampler;\n"
         "void main() {\n"
         "  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
         "}\n";

static const char* vertexShaderCode =
        "attribute vec4 position;    \n"
        "attribute vec2 texcoord;   \n"
        "varying vec2 v_texcoord;     \n"
        "void main(void)               \n"
        "{                            \n"
        "   gl_Position = position;  \n"
        "   v_texcoord = texcoord;  \n"
        "}                            \n";


TexturePreviewRender::TexturePreviewRender(){

}

TexturePreviewRender::~TexturePreviewRender(){

}

void TexturePreviewRender::init(int width,int height,PicPreviewTexture* previewTexture){
    this->viewWidth = width;
    this->viewHeight = height;
    picPreviewTexture = previewTexture;

    mProgram = GLHelper::createProgram(vertexShaderCode, fragmentShaderCode);
    aPositionLoc = glGetAttribLocation(mProgram, "position");
    aTexcoordLoc = glGetAttribLocation(mProgram, "texcoord");
    uYuvTexSamplerLoc = glGetUniformLocation(mProgram, "yuvTexSampler");

}

void TexturePreviewRender::onSurfaceCreated(){

}

void TexturePreviewRender::onSurfaceChange(int width,int height){

}

void TexturePreviewRender::onDrawFrame(){
    glViewport(0, 0, viewWidth, viewHeight);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(mProgram);
    static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
    glVertexAttribPointer(aPositionLoc, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(aPositionLoc);
    static const GLfloat _texCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
    glVertexAttribPointer(aTexcoordLoc, 2, GL_FLOAT, 0, 0, _texCoords);
    glEnableVertexAttribArray(aTexcoordLoc);
    picPreviewTexture->bindTexture(uYuvTexSamplerLoc);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void TexturePreviewRender::dealloc(){
    if(picPreviewTexture){
        picPreviewTexture->dealloc();
    }

    if (mProgram) {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}

