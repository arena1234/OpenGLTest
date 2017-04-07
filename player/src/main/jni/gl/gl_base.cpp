#include <bean/bean_base.h>
#include "gl/gl_base.h"

GLBase::GLBase(TransformBean *transformBean) {
    LOGD("[GLBase] +");
    mTextureId = (GLuint *) malloc(1 * sizeof(GLuint));
    mTransformBean = transformBean;
    mTexCoordBuffer = new FloatBuffer();
    mVertexBuffer = new FloatBuffer();
    mMatrix = new Matrix();
    mLight = new Point3();
    updateLight(1.0);
}

GLBase::~GLBase() {
    free(mTextureId);
    free(mVBO);
    free(mVAO);
    delete mTexCoordBuffer;
    delete mVertexBuffer;
    delete mMatrix;
    delete mLight;
    LOGD("[GLBase] -");
}

GLboolean GLBase::checkGLError(const char *op) {
    GLint error = glGetError();
    if (error != GL_NO_ERROR) {
        LOGE("[GLBase:ES_ERROR]%s, glError (0x%x)", op, error);
        return GL_FALSE;
    }
    return GL_TRUE;
}

GLuint GLBase::loadShader(GLenum shaderType, const char *pSource) {
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

GLuint GLBase::createProgram(const char *pVertexSource, const char *pFragmentSource) {
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

void GLBase::updateBuffer() {
    updateTexCoord();
    updateVertex();
    isUpdateBuffer = GL_TRUE;
}

void GLBase ::updateLight(GLfloat light) {
    mLight->x = light;
    mLight->y = light;
    mLight->z = light;
}

void GLBase::prepareRenderer() {
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
    }
}

GLint GLBase::onSurfaceCreated() {
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
    glGenBuffers(size * 2, mVBO);
    checkGLError("glGenBuffers");

    return mTextureId[0];
}

void GLBase::onSurfaceChanged(int w, int h) {
    mWindowWidth = w;
    mWindowHeight = h;
    mMatrix->perspective(45, (GLfloat) w / (GLfloat) h, 0.1, 100);
    mMatrix->lookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    glViewport(0, 0, w, h);
    checkGLError("glViewport");

    glEnable(GL_DEPTH_TEST);
}

void GLBase::onDrawFrame(Bitmap *bmp) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prepareRenderer();

    glUseProgram(mShaderProgramHandle);
    updateFrame(bmp);
}