#include "gl/gl_pic_light.h"

PicLight::PicLight(TransformBean *transformBean)
        : GLBase(transformBean) {
    LOGD("[Picture] +");
    bFirstFrame = true;
}

PicLight::~PicLight() {
    LOGD("[Picture] -");
}

void PicLight::updateVertex() {
    LOGD("[Picture:updateVertex]");
    mTexCoordBuffer->updateBuffer((GLfloat *) cubeTexCoords, sizeof(cubeTexCoords),
                                  sizeof(cubeTexCoords[0]), 2);
}

void PicLight::updateTexCoord() {
    LOGD("[Picture:updateTexCoord]");
    mVertexBuffer->updateBuffer((GLfloat *) cubeVertex, sizeof(cubeVertex),
                                sizeof(cubeVertex[0]), 3);
}

GLuint PicLight::loadShader() {
    GLuint shaderHandle = createProgram(gCubeVertexShader, gCubeFragmentShader);

    // 获取投影、Camera、变换句柄
    mProjectionHandle = glGetUniformLocation(shaderHandle, "projection");
    mCameraHandle = glGetUniformLocation(shaderHandle, "camera");
    mTransformHandle = glGetUniformLocation(shaderHandle, "transform");
    return shaderHandle;
}

void PicLight::createTexture() {
    glGenTextures(1, mTextureId);
    checkGLError("glGenTextures");
    // 绑定纹理,之后任何的纹理指令都可以配置当前绑定的纹理
    glBindTexture(GL_TEXTURE_2D, mTextureId[0]);
    checkGLError("glBindTexture");
    // 纹理过滤
    // GL_NEAREST 当前像素值
    // GL_LINEAR 当前像素附近颜色的混合色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 纹理剩余位置显示处理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void PicLight::updateFrame(Bitmap *bmp) {
    if (bFirstFrame && bmp != NULL) {
        bFirstFrame = false;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     bmp->bitmapInfo.width, bmp->bitmapInfo.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, bmp->pixels);
        checkGLError("glTexImage2D");
    } else if (bmp != NULL) {
//        glBindTexture(GL_TEXTURE_2D, mTextureId[0]);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
//                        bmp->bitmapInfo.width, bmp->bitmapInfo.height,
//                        GL_RGBA, GL_UNSIGNED_BYTE, bmp->pixels);
    }
    for(GLuint cubeCount = 0; cubeCount < sizeof(positions) / sizeof(positions[0]); cubeCount++) {
        for (GLuint i = 0; i < mVertexBuffer->getSize(); i++) {
            // 绑定VAO，绑定之后开始绘制
            glBindVertexArray(mVAO[i]);
            checkGLError("glBindVertexArray +");

            // 投影、Camera、变换赋值
            mMatrix->perspective(mTransformBean->fov,
                                 (GLfloat) mWindowWidth / (GLfloat) mWindowHeight,
                                 0.1,
                                 100);
            mMatrix->setIdentity();
            mMatrix->translate(positions[cubeCount][0], positions[cubeCount][1], positions[cubeCount][2]);
            mMatrix->rotate(mTransformBean->degreeY, 1, 0, 0);
            mMatrix->rotate(mTransformBean->degreeX, 0, 1, 0);
            mMatrix->translate(positions[cubeCount][0], positions[cubeCount][1], positions[cubeCount][2]);
            glUniformMatrix4fv(mProjectionHandle, 1, GL_FALSE, mMatrix->getProjectionMatrix());
            glUniformMatrix4fv(mCameraHandle, 1, GL_FALSE, mMatrix->getCameraMatrix());
            glUniformMatrix4fv(mTransformHandle, 1, GL_FALSE, mMatrix->getTransformMatrix());
            glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer->getBuffer(i)->pointSize);
            // 解绑VAO
            glBindVertexArray(0);
            checkGLError("glBindVertexArray -");
        }
    }
}