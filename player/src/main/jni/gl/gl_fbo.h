#ifndef GL_FBO_H
#define GL_FBO_H

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


const char gPicVertexShader[] =
        "#version 300 es                                                    \n"
                "layout (location = "STRV(SHADER_IN_POSITION)") in vec3 position;   \n"
                "layout (location = "STRV(SHADER_IN_TEX_COORDS)") in vec2 texCoord; \n"
                "out vec2 TexCoord;                                                 \n"
                "uniform mat4 projection;                                           \n"
                "uniform mat4 camera;                                               \n"
                "uniform mat4 transform;                                            \n"
                "void main() {                                                      \n"
                "  gl_Position = projection*camera*transform*vec4(position, 1.0);   \n"
                "  TexCoord = vec2(texCoord.s, 1.0-texCoord.t);                     \n"
                "}\n";

const char gPicFragmentShader[] =
        "#version 300 es                        \n"
                "precision mediump float;               \n"
                "in vec2 TexCoord;                      \n"
                "uniform sampler2D tTexture;            \n"
                "uniform vec3 light;                    \n"
                "out vec4 color;                        \n"
                "void main() {                          \n"
                "  color = vec4(light, 1.0) * texture(tTexture, TexCoord); \n"
                "}\n";

const char gFboVertexShader[] =
        "#version 300 es                                                    \n"
                "layout (location = "STRV(SHADER_IN_POSITION)") in vec3 position;   \n"
                "layout (location = "STRV(SHADER_IN_TEX_COORDS)") in vec2 texCoord; \n"
                "out vec2 TexCoord;                                                 \n"
//                "uniform mat4 projection;                                           \n"
//                "uniform mat4 camera;                                               \n"
//                "uniform mat4 transform;                                            \n"
                "void main() {                                                      \n"
//                "  gl_Position = projection*camera*transform*vec4(position, 1.0);   \n"
                "  gl_Position = vec4(position, 1.0);   \n"
                "  TexCoord = vec2(texCoord.s, 1.0-texCoord.t);                     \n"
                "}\n";

const char gFboFragmentShader[] =
        "#version 300 es                        \n"
                "precision mediump float;               \n"
                "in vec2 TexCoord;                      \n"
                "uniform sampler2D tTexture;            \n"
                "out vec4 color;                        \n"
                "void main() {                          \n"
                "  color = texture(tTexture, TexCoord); \n"
                "}\n";

const Point3 A = {-0.5, 0.5, 0.5};
const Point3 B = {-0.5, -0.5, 0.5};
const Point3 C = {0.5, -0.5, 0.5};
const Point3 D = {0.5, 0.5, 0.5};
const Point3 E = {-0.5, 0.5, -0.5};
const Point3 F = {-0.5, -0.5, -0.5};
const Point3 G = {0.5, -0.5, -0.5};
const Point3 H = {0.5, 0.5, -0.5};

const GLfloat picVertex[][12] = {
        {// 左
                F.x, F.y, F.z,
                E.x, E.y, E.z,
                B.x, B.y, B.z,
                A.x, A.y, A.z,
        },
        {// 前
                B.x, B.y, B.z,
                A.x, A.y, A.z,
                C.x, C.y, C.z,
                D.x, D.y, D.z,
        },
        {// 上
                A.x, A.y, A.z,
                E.x, E.y, E.z,
                D.x, D.y, D.z,
                H.x, H.y, H.z,
        },
        {// 下
                F.x, F.y, F.z,
                B.x, B.y, B.z,
                G.x, G.y, G.z,
                C.x, C.y, C.z,
        },
        {//右
                C.x, C.y, C.z,
                D.x, D.y, D.z,
                G.x, G.y, G.z,
                H.x, H.y, H.z,
        },
        {// 后
                G.x, G.y, G.z,
                H.x, H.y, H.z,
                F.x, F.y, F.z,
                E.x, E.y, E.z,
        },
};
const GLfloat picTexCoords[][8] = {
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        },
};

const GLfloat fboVertex[] = {
        B.x, B.y, B.z,
        A.x, A.y, A.z,
        C.x, C.y, C.z,
        D.x, D.y, D.z,
};
const GLfloat fboTexCoords[] = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0,
};

class GLFbo {
public:
    GLFbo(TransformBean *transformBean);

    virtual ~GLFbo();

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

    void updateFrame(Bitmap *bmp);

    void createTexture();

    void updateVertex();

    void updateTexCoord();

    void updateLight(GLfloat light);

    GLuint loadShader();

    void prepareRenderer();

private :

    GLuint loadShader(GLenum shaderType, const char *pSource);

    void updateBuffer();

    GLboolean bFirstFrame;
    GLboolean bSecondFrame;
//    GLint mFboProjectionHandle;        // 投影矩阵的句柄
//    GLint mFboCameraHandle;            // Camera矩阵的句柄
//    GLint mFboTransformHandle;         // 变换矩阵的句柄
    GLuint mFboShaderProgramHandle;    // 着色器句柄

    GLuint *mFboVBO;
    GLuint *mFboVAO;

    GLuint *mFboId;
};

#endif //GL_FBO_H
