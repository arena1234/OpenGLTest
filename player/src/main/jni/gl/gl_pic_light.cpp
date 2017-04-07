#include "gl/gl_pic_light.h"

PicLight::PicLight(TransformBean *transformBean)
        : GLBase(transformBean) {
    LOGD("[Picture] +");
    bFirstFrame = true;
    mNormalBuffer = new FloatBuffer();
}

PicLight::~PicLight() {
    LOGD("[Picture] -");
    delete mNormalBuffer;
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

void PicLight::updateNormalVector() {
    LOGD("[Picture:updateNormalVector]");
    mNormalBuffer->updateBuffer((GLfloat *) normalVertex, sizeof(normalVertex),
                                sizeof(normalVertex[0]), 3);
}

GLuint PicLight::loadShader() {
    GLuint shaderHandle = createProgram(gCubeVertexShader, gCubeFragmentShader);

    // 获取投影、Camera、变换句柄
    mProjectionHandle = glGetUniformLocation(shaderHandle, "projection");
    mCameraHandle = glGetUniformLocation(shaderHandle, "camera");
    mTransformHandle = glGetUniformLocation(shaderHandle, "transform");

    mLightAmbient = glGetUniformLocation(shaderHandle, "light.ambient");
    mLightDiffuse = glGetUniformLocation(shaderHandle, "light.diffuse");
    mLightSpecular = glGetUniformLocation(shaderHandle, "light.specular");
    mLightPosition = glGetUniformLocation(shaderHandle, "light.position");
    mObjectPosition = glGetUniformLocation(shaderHandle, "objectPos");

    mMaterialAmbient = glGetUniformLocation(shaderHandle, "material.ambient");
    mMaterialDiffuse = glGetUniformLocation(shaderHandle, "material.diffuse");
    mMaterialSpecular = glGetUniformLocation(shaderHandle, "material.specular");
    mMaterialShininess = glGetUniformLocation(shaderHandle, "material.shininess");

    loadShaderForLight();
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
    for (GLuint cubeCount = 0; cubeCount < sizeof(positions) / sizeof(positions[0]); cubeCount++) {
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
//            mMatrix->translate(positions[cubeCount][0], positions[cubeCount][1], positions[cubeCount][2]);
            mMatrix->rotate(mTransformBean->degreeY, 1, 0, 0);
            mMatrix->rotate(mTransformBean->degreeX, 0, 1, 0);
            mMatrix->translate(positions[cubeCount][0], positions[cubeCount][1],
                               positions[cubeCount][2]);
            glUniformMatrix4fv(mProjectionHandle, 1, GL_FALSE, mMatrix->getProjectionMatrix());
            glUniformMatrix4fv(mCameraHandle, 1, GL_FALSE, mMatrix->getCameraMatrix());
            glUniformMatrix4fv(mTransformHandle, 1, GL_FALSE, mMatrix->getTransformMatrix());
            glUniform3f(mLightAmbient, 1.0f, 1.0f, 1.0f);
            glUniform3f(mLightDiffuse, 1.0f, 1.0f, 1.0f);// 让我们把这个光调暗一点，这样会看起来更自然
            glUniform3f(mLightSpecular, 1.0f, 1.0f, 1.0f);
            glUniform3f(mLightPosition, lightPosition[0], lightPosition[1], lightPosition[2]);
            glUniform3f(mObjectPosition,
                        positions[cubeCount][0], positions[cubeCount][1], positions[cubeCount][2]);
            glUniform3f(mMaterialAmbient, 1.0f, 1.0f, 1.0f);
            glUniform3f(mMaterialDiffuse, 1.0f, 1.0f, 1.0f);
            glUniform3f(mMaterialSpecular, 0.5f, 0.5f, 0.5f);
            glUniform1f(mMaterialShininess, 32.0f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer->getBuffer(i)->pointSize);
            // 解绑VAO
            glBindVertexArray(0);
            checkGLError("glBindVertexArray -");
        }
    }

    // light
    {
        glUseProgram(mShaderLightHandle);
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
            mMatrix->translate(lightPosition[0], lightPosition[1], lightPosition[2]);
//            mMatrix->rotate(mTransformBean->degreeY, 1, 0, 0);
//            mMatrix->rotate(mTransformBean->degreeX, 0, 1, 0);
//            mMatrix->translate(lightPosition[0], lightPosition[1], lightPosition[2]);
            glUniformMatrix4fv(mLightProjectionHandle, 1, GL_FALSE, mMatrix->getProjectionMatrix());
            glUniformMatrix4fv(mLightCameraHandle, 1, GL_FALSE, mMatrix->getCameraMatrix());
            glUniformMatrix4fv(mLightTransformHandle, 1, GL_FALSE, mMatrix->getTransformMatrix());
            glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer->getBuffer(i)->pointSize);
            // 解绑VAO
            glBindVertexArray(0);
            checkGLError("glBindVertexArray -");
        }
    }
}

void PicLight::loadShaderForLight() {
    mShaderLightHandle = createProgram(gLightVertexShader, gLightFragmentShader);

    // 获取投影、Camera、变换句柄
    mLightProjectionHandle = glGetUniformLocation(mShaderLightHandle, "projection");
    mLightCameraHandle = glGetUniformLocation(mShaderLightHandle, "camera");
    mLightTransformHandle = glGetUniformLocation(mShaderLightHandle, "transform");
}

void PicLight::prepareRenderer() {
    if (isUpdateBuffer) {
        isUpdateBuffer = GL_FALSE;
        for (GLuint i = 0; i < mVertexBuffer->getSize(); i++) {
            // 绑定VAO，绑定之后设置顶点坐标相关的值
            glBindVertexArray(mVAO[i]);
            checkGLError("glBindVertexArray +");
            // 绑定顶点坐标并设置顶点坐标
            glBindBuffer(GL_ARRAY_BUFFER, mVBO[i * 2 + 0]);
            checkGLError("glBindBuffer for vertex");
            glBufferData(GL_ARRAY_BUFFER, mVertexBuffer->getBuffer(i)->bufferSize,
                         mVertexBuffer->getBuffer(i)->buffer, GL_DYNAMIC_DRAW);
            checkGLError("glBufferData for vertex");
            // 设置顶点属性指针
            glVertexAttribPointer(SHADER_IN_POSITION, 3, GL_FLOAT, GL_FALSE,
                                  3 * sizeof(GLfloat), (GLvoid *) 0);
            checkGLError("glVertexAttribPointer");
            glEnableVertexAttribArray(SHADER_IN_POSITION);
            checkGLError("glEnableVertexAttribArray");

            // 绑定纹理坐标并设置纹理坐标
            glBindBuffer(GL_ARRAY_BUFFER, mVBO[i * 2 + 1]);
            checkGLError("glBindBuffer for texture");
            glBufferData(GL_ARRAY_BUFFER, mTexCoordBuffer->getBuffer(i)->bufferSize,
                         mTexCoordBuffer->getBuffer(i)->buffer, GL_DYNAMIC_DRAW);
            checkGLError("glBufferData for texture");
            // 设置纹理属性指针
            glVertexAttribPointer(SHADER_IN_TEX_COORDS, 2, GL_FLOAT, GL_FALSE,
                                  2 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(SHADER_IN_TEX_COORDS);

            // 绑定法向量坐标
            glBindBuffer(GL_ARRAY_BUFFER, mVBO[i * 2 + 2]);
            checkGLError("glBindBuffer for normal");
            glBufferData(GL_ARRAY_BUFFER, mNormalBuffer->getBuffer(i)->bufferSize,
                         mNormalBuffer->getBuffer(i)->buffer, GL_DYNAMIC_DRAW);
            checkGLError("glBufferData for normal");
            // 设置法向量属性指针
            glVertexAttribPointer(SHADER_IN_NORMAL_VERTOR, 3, GL_FLOAT, GL_FALSE,
                                  3 * sizeof(GLfloat), (GLvoid *) 0);
            checkGLError("glVertexAttribPointer");
            glEnableVertexAttribArray(SHADER_IN_NORMAL_VERTOR);
            checkGLError("glEnableVertexAttribArray");
            // 解绑VAO
            glBindVertexArray(0);
            checkGLError("glBindVertexArray -");
        }
    }
}