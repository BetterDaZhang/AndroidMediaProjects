//
// Created by zhangjunling on 18-8-13.
//

#ifndef ANDROIDMEDIAPROJECTS_TEXTUREPREVIEWRENDER_H
#define ANDROIDMEDIAPROJECTS_TEXTUREPREVIEWRENDER_H

#include "../libcommon/GLHelper.h"
#include "pic_preview_texture.h"

class TexturePreviewRender {
public:
    TexturePreviewRender();
    virtual ~TexturePreviewRender();

    GLint mProgram;
    GLint aPositionLoc;
    GLint aTexcoordLoc;
    GLint uYuvTexSamplerLoc;

    int viewWidth;
    int viewHeight;

    PicPreviewTexture* picPreviewTexture;

    GLfloat* mVertexArray;
    GLfloat* mColorArray;
    void onSurfaceCreated();
    void onSurfaceChange(int width,int height);
    void onDrawFrame();

    void dealloc();
    void init(int ,int ,PicPreviewTexture*);
};


#endif //ANDROIDMEDIAPROJECTS_TEXTUREPREVIEWRENDER_H
