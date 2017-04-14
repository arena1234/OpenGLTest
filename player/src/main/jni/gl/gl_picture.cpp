#include "gl/gl_picture.h"
#include "gl_renderer.h"

Picture::Picture(TransformBean *bean) : GLRenderer(bean) {
    LOGD("[Picture] +");
    bFirstFrame = GL_TRUE;
    pShape = new Shape();
}

Picture::~Picture() {
    LOGD("[Picture] -");
    delete pShape;
}

void Picture::loadShader() {
    GLRenderer::loadShader();
    pBeanProcess->mProgramHandle = createProgram(gPicVertexShader, gPicFragmentShader);
    // 获取投影、Camera、变换句柄
    pBeanProcess->mProjectionHandle = glGetUniformLocation(pBeanProcess->mProgramHandle,
                                                           "projection");
    pBeanProcess->mCameraHandle = glGetUniformLocation(pBeanProcess->mProgramHandle,
                                                       "camera");
    pBeanProcess->mTransformHandle = glGetUniformLocation(pBeanProcess->mProgramHandle,
                                                          "transform");
    pBeanProcess->mLightHandle = glGetUniformLocation(pBeanProcess->mProgramHandle,
                                                      "light");
    pBeanProcess->mAnimHandle = glGetUniformLocation(pBeanProcess->mProgramHandle,
                                                      "anim");
    LOGD("[Picture:loadShader]pBeanProcess->mProgramHandle=%d", pBeanProcess->mProgramHandle);

}

void Picture::prepareProcessBuffer() {
    LOGD("[GLRenderer:prepareProcessBuffer]");
    pShape->cylinder();
    pBeanProcess->pTextureBuffer->updateBuffer(pShape->textureBuffer,
                                               pShape->textureSize,
                                               pShape->textureSize,
                                               POINT_SIZE_TEXTURE);
    pBeanProcess->pVertexBuffer->updateBuffer(pShape->vertexBufferStart,
                                              pShape->vertexSize,
                                              pShape->vertexSize,
                                              POINT_SIZE_VERTEX);
    pBeanProcess->pVertexEndBuffer->updateBuffer(pShape->vertexBufferEnd,
                                              pShape->vertexSize,
                                              pShape->vertexSize,
                                              POINT_SIZE_VERTEX);
    pBeanProcess->bUpdateBuffer = GL_TRUE;
}

void Picture::prepareDraw(Bitmap *bmp) {
    if (bFirstFrame && bmp != NULL) {
        bFirstFrame = GL_FALSE;
        glBindTexture(pBeanProcess->eTextureTarget, pBeanProcess->mTextureId);
        glTexImage2D(pBeanProcess->eTextureTarget, 0, GL_RGBA,
                     bmp->bitmapInfo.width, bmp->bitmapInfo.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, bmp->pixels);
    }
}

