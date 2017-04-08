#ifndef GL_BASE_H
#define GL_BASE_H

#include <GLES3/gl3.h>
#include <android/bitmap.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include "log.h"
#include "gl/gl_matrix.h"
#include "gl/gl_bitmap.h"
#include "bean/bean_base.h"
#include "bean/float_buffer.h"
#include "file/file.h"

#define STR(s) #s
#define STRV(s) STR(s)
#define SHADER_IN_POSITION          0
#define SHADER_IN_TEX_COORDS        1
#define SHADER_IN_NORMAL_VERTOR     2

class GLBase {
public:
    GLBase(TransformBean *transformBean);

    virtual ~GLBase();

    void onSurfaceChanged(int w, int h);

    GLint onSurfaceCreated();

    void onDrawFrame(Bitmap *bmp);

protected:
    TransformBean *mTransformBean;
    FloatBuffer *mVertexBuffer;     // 顶点坐标缓存
    FloatBuffer *mTexCoordBuffer;   // 纹理坐标缓存
    Matrix *mMatrix;                // 矩阵(投影、变换、Camera)
    GLint mProjectionHandle;        // 投影矩阵的句柄
    GLint mCameraHandle;            // Camera矩阵的句柄
    GLint mTransformHandle;         // 变换矩阵的句柄
    GLint mLightHandle;             // 变换矩阵的句柄
    GLuint mShaderProgramHandle;    // 着色器句柄
    GLuint *mTextureId;             // 纹理ID
    GLuint *mVBO;
    GLuint *mVAO;
    GLuint mWindowWidth;
    GLuint mWindowHeight;
    Point3 *mLight;

    GLboolean isUpdateBuffer;

    GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

    GLboolean checkGLError(const char *op);

    virtual void updateFrame(Bitmap *bmp) = 0;

    virtual void createTexture() = 0;

    virtual void updateVertex() = 0;

    virtual void updateTexCoord() = 0;

    virtual void updateLight(GLfloat light);

    virtual GLuint loadShader() = 0;

    virtual void prepareRenderer();

private :

    GLuint loadShader(GLenum shaderType, const char *pSource);

    void updateBuffer();
};

#endif //GL_BASE_H
