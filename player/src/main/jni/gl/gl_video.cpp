#include "gl/gl_video.h"

Video::Video(TransformBean *transformBean)
        : GLBase(transformBean) {
    LOGD("[Video] +");
}

Video::~Video() {
    LOGD("[Video] -");
}
void Video::updateVertex() {
    LOGD("[Video:updateVertex]");
    mTexCoordBuffer->updateBuffer((GLfloat *) videoTexCoords, sizeof(videoTexCoords),
                                  sizeof(videoTexCoords[0]), 2);
}

void Video::updateTexCoord() {
    LOGD("[Video:updateTexCoord]");
    mVertexBuffer->updateBuffer((GLfloat *) videoVertex, sizeof(videoVertex),
                                sizeof(videoVertex[0]), 3);
}

GLuint Video::loadShader() {
    GLuint shaderHandle = createProgram(gVideoVertexShader, gVideoFragmentShader);

    // 获取投影、Camera、变换句柄
    mProjectionHandle = glGetUniformLocation(shaderHandle, "projection");
    mCameraHandle = glGetUniformLocation(shaderHandle, "camera");
    mTransformHandle = glGetUniformLocation(shaderHandle, "transform");
    mLightHandle = glGetUniformLocation(shaderHandle, "light");
    return shaderHandle;
}

void Video::createTexture() {
    // 创建纹理ID
    glGenTextures(1, mTextureId);
    checkGLError("glGenTextures");
    // 绑定纹理,之后任何的纹理指令都可以配置当前绑定的纹理
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureId[0]);
    checkGLError("glBindTexture");
    // 纹理过滤
    // GL_NEAREST 当前像素值
    // GL_LINEAR 当前像素附近颜色的混合色
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 纹理剩余位置显示处理
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Video::updateFrame(Bitmap *bmp) {
    for (GLuint i = 0; i < mVertexBuffer->getSize(); i++) {
        // 绑定VAO，绑定之后开始绘制
        glBindVertexArray(mVAO[i]);
        checkGLError("glBindVertexArray +");

        // 投影、Camera、变换赋值
        mMatrix->perspective(mTransformBean->fov, (GLfloat) mWindowWidth / (GLfloat) mWindowHeight,
                             0.1,
                             100);
        mMatrix->setIdentity();
        mMatrix->rotate(mTransformBean->degreeY, 1, 0, 0);
        mMatrix->rotate(mTransformBean->degreeX, 0, 1, 0);
        //mMatrix->translate(0.5, 0.5, 0);
        glUniformMatrix4fv(mProjectionHandle, 1, GL_FALSE, mMatrix->getProjectionMatrix());
        glUniformMatrix4fv(mCameraHandle, 1, GL_FALSE, mMatrix->getCameraMatrix());
        glUniformMatrix4fv(mTransformHandle, 1, GL_FALSE, mMatrix->getTransformMatrix());
        glUniform3f(mLightHandle, mLight->x, mLight->y, mLight->z);
        // 绑定纹理,之后任何的纹理指令都可以配置当前绑定的纹理
        glActiveTexture(GL_TEXTURE0);
        checkGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureId[0]);
        checkGLError("glBindTexture");
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer->getBuffer(i)->pointSize);
        // 解绑VAO
        glBindVertexArray(0);
        checkGLError("glBindVertexArray -");
    }
}

