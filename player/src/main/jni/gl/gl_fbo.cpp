#include "gl/gl_fbo.h"

GLFbo::GLFbo(TransformBean *transformBean) {
    LOGD("[GLBase] +");
    bFirstFrame = true;
    mTextureId = (GLuint *) malloc(1 * sizeof(GLuint));
    mTransformBean = transformBean;
    mTexCoordBuffer = new FloatBuffer();
    mVertexBuffer = new FloatBuffer();
    mMatrix = new Matrix();
    mLight = new Point3();
    updateLight(1.0);
}

GLFbo::~GLFbo() {
    free(mTextureId);
    free(mVBO);
    free(mVAO);
    delete mTexCoordBuffer;
    delete mVertexBuffer;
    delete mMatrix;
    delete mLight;
    LOGD("[GLBase] -");
}

GLboolean GLFbo::checkGLError(const char *op) {
    GLint error = glGetError();
    if (error != GL_NO_ERROR) {
        LOGE("[GLBase:ES_ERROR]%s, glError (0x%x)", op, error);
        return GL_FALSE;
    }
    return GL_TRUE;
}

GLuint GLFbo::loadShader(GLenum shaderType, const char *pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char *buf = (char *) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint GLFbo::createProgram(const char *pVertexSource, const char *pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        LOGE("[GLBase:createProgram]vertex shader fail");
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        LOGE("[GLBase:createProgram]fragment shader fail");
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, pixelShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char *buf = (char *) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("[GLBase:createProgram]Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void GLFbo::updateBuffer() {
    updateTexCoord();
    updateVertex();
    isUpdateBuffer = GL_TRUE;
}

void GLFbo::updateLight(GLfloat light) {
    mLight->x = light;
    mLight->y = light;
    mLight->z = light;
}

void GLFbo::updateVertex() {
    LOGD("[Picture:updateVertex]");
    mTexCoordBuffer->updateBuffer((GLfloat *) picTexCoords, sizeof(picTexCoords),
                                  sizeof(picTexCoords[0]), 2);
}

void GLFbo::updateTexCoord() {
    LOGD("[Picture:updateTexCoord]");
    mVertexBuffer->updateBuffer((GLfloat *) picVertex, sizeof(picVertex),
                                sizeof(picVertex[0]), 3);
}

GLuint GLFbo::loadShader() {
    GLuint shaderHandle = createProgram(gPicVertexShader, gPicFragmentShader);
    // 获取投影、Camera、变换句柄
    mProjectionHandle = glGetUniformLocation(shaderHandle, "projection");
    mCameraHandle = glGetUniformLocation(shaderHandle, "camera");
    mTransformHandle = glGetUniformLocation(shaderHandle, "transform");
    mLightHandle = glGetUniformLocation(shaderHandle, "light");


    mFboShaderProgramHandle = createProgram(gFboVertexShader, gFboFragmentShader);
    // 获取投影、Camera、变换句柄
//    mFboProjectionHandle = glGetUniformLocation(mFboShaderProgramHandle, "projection");
//    mFboCameraHandle = glGetUniformLocation(mFboShaderProgramHandle, "camera");
//    mFboTransformHandle = glGetUniformLocation(mFboShaderProgramHandle, "transform");
    return shaderHandle;
}

void GLFbo::createTexture() {
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

void GLFbo::updateFrame(Bitmap *bmp) {

}

void GLFbo::prepareRenderer() {
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
            // 解绑VAO
            glBindVertexArray(0);
            checkGLError("glBindVertexArray -");
        }

        glBindVertexArray(mFboVAO[0]);
        checkGLError("glBindVertexArray +");
        // 绑定顶点坐标并设置顶点坐标
        glBindBuffer(GL_ARRAY_BUFFER, mFboVBO[0]);
        checkGLError("glBindBuffer for vertex");
        glBufferData(GL_ARRAY_BUFFER, sizeof(fboVertex), fboVertex, GL_DYNAMIC_DRAW);
        checkGLError("glBufferData for vertex");
        // 设置顶点属性指针
        glVertexAttribPointer(SHADER_IN_POSITION, 3, GL_FLOAT, GL_FALSE,
                              3 * sizeof(GLfloat), (GLvoid *) 0);
        checkGLError("glVertexAttribPointer");
        glEnableVertexAttribArray(SHADER_IN_POSITION);
        checkGLError("glEnableVertexAttribArray");

        // 绑定纹理坐标并设置纹理坐标
        glBindBuffer(GL_ARRAY_BUFFER, mFboVBO[1]);
        checkGLError("glBindBuffer for texture");
        glBufferData(GL_ARRAY_BUFFER, sizeof(fboTexCoords), fboTexCoords, GL_DYNAMIC_DRAW);
        checkGLError("glBufferData for texture");
        // 设置纹理属性指针
        glVertexAttribPointer(SHADER_IN_TEX_COORDS, 2, GL_FLOAT, GL_FALSE,
                              2 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(SHADER_IN_TEX_COORDS);
        // 解绑VAO
        glBindVertexArray(0);
        checkGLError("glBindVertexArray -");
    }
}

GLint GLFbo::onSurfaceCreated() {
    updateBuffer();
    mShaderProgramHandle = loadShader();
    createTexture();

    GLuint size = mTexCoordBuffer->getSize();
    LOGD("[GLBase:onSurfaceCreated]VAO size = %d", size);
    mVAO = (GLuint *) malloc(size * sizeof(GLuint));
    mVBO = (GLuint *) malloc(size * 2 * sizeof(GLuint));
    // 获取顶点数组对象VAO(Vertex Array Object)和顶点缓冲对象VBO(Vertex Buffer Objects)
    glGenVertexArrays(size, mVAO);
    checkGLError("glGenVertexArrays");
    glGenBuffers(size * 3, mVBO);
    checkGLError("glGenBuffers");

    mFboVAO = (GLuint *) malloc(sizeof(GLuint));
    mFboVBO = (GLuint *) malloc(2 * sizeof(GLuint));
    glGenVertexArrays(size, mFboVAO);
    checkGLError("glGenVertexArrays");
    glGenBuffers(size * 3, mFboVBO);
    checkGLError("glGenBuffers");

    return mTextureId[0];
}

void GLFbo::onSurfaceChanged(int w, int h) {
    mWindowWidth = w;
    mWindowHeight = h;
    mMatrix->perspective(45, (GLfloat) w / (GLfloat) h, 0.1, 100);
    mMatrix->lookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    glViewport(0, 0, w, h);
    checkGLError("glViewport");

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLFbo::onDrawFrame(Bitmap *bmp) {
    prepareRenderer();
    if (bFirstFrame && bmp != NULL) {
        bFirstFrame = false;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     bmp->bitmapInfo.width, bmp->bitmapInfo.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, bmp->pixels);
        checkGLError("glTexImage2D");
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mShaderProgramHandle);
    for (GLuint i = 0; i < mVertexBuffer->getSize(); i++) {
        // 绑定VAO，绑定之后开始绘制
        glBindVertexArray(mVAO[i]);
        checkGLError("glBindVertexArray +");

        // 投影、Camera、变换赋值
        mMatrix->perspective(mTransformBean->fov, (GLfloat) mWindowWidth / (GLfloat) mWindowHeight,
                             0.1,
                             100);
        mMatrix->setIdentity();
//        mMatrix->translate(0, 0, -1.5);
        mMatrix->rotate(mTransformBean->degreeY, 1, 0, 0);
        mMatrix->rotate(mTransformBean->degreeX, 0, 1, 0);
        mMatrix->translate(-2, 0, -1.5);
        glUniformMatrix4fv(mProjectionHandle, 1, GL_FALSE, mMatrix->getProjectionMatrix());
        glUniformMatrix4fv(mCameraHandle, 1, GL_FALSE, mMatrix->getCameraMatrix());
        glUniformMatrix4fv(mTransformHandle, 1, GL_FALSE, mMatrix->getTransformMatrix());
        glUniform3f(mLightHandle, mLight->x, mLight->y, mLight->z);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer->getBuffer(i)->pointSize);
        // 解绑VAO
        glBindVertexArray(0);
        checkGLError("glBindVertexArray -");
    }

    glUseProgram(mFboShaderProgramHandle);
    //for (GLuint i = 0; i < mVertexBuffer->getSize(); i++) {
        // 绑定VAO，绑定之后开始绘制
        glBindVertexArray(mFboVAO[0]);
        checkGLError("glBindVertexArray +");

        // 投影、Camera、变换赋值
        mMatrix->perspective(mTransformBean->fov, (GLfloat) mWindowWidth / (GLfloat) mWindowHeight,
                             0.1,
                             100);
        mMatrix->setIdentity();
        mMatrix->translate(8, 0, -12);
//        glUniformMatrix4fv(mProjectionHandle, 1, GL_FALSE, mMatrix->getProjectionMatrix());
//        glUniformMatrix4fv(mCameraHandle, 1, GL_FALSE, mMatrix->getCameraMatrix());
//        glUniformMatrix4fv(mTransformHandle, 1, GL_FALSE, mMatrix->getTransformMatrix());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // 解绑VAO
        glBindVertexArray(0);
        checkGLError("glBindVertexArray -");
    //}
}