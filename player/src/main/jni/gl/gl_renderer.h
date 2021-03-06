#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <GLES3/gl3.h>
#include <stdlib.h>
#include <math.h>

#include "log.h"
#include "gl/gl_matrix.h"
#include "gl/gl_bitmap.h"
#include "gl/gl_base.h"
#include "bean/bean_base.h"
#include "bean/float_buffer.h"

typedef struct {
    FloatBuffer *pVertexBuffer;         // 顶点坐标缓存
    FloatBuffer *pVertexEndBuffer;      // 顶点坐标缓存
    FloatBuffer *pTextureBuffer;        // 纹理坐标缓存
    TransformBean *pTransformBean;      // 移动旋转等数据
    Matrix *pMatrix;                    // 矩阵(投影、变换、Camera)
    GLuint mProgramHandle;              // 着色器句柄
    GLint mProjectionHandle;            // 投影矩阵的句柄
    GLint mCameraHandle;                // Camera矩阵的句柄
    GLint mTransformHandle;             // 变换矩阵的句柄
    GLint mLightHandle;                 // 变换矩阵的句柄
    GLint mAnimHandle;                  // 变换矩阵的句柄
    GLuint mTextureId;                  //
    GLboolean bUpdateBuffer;
    GLenum eTextureTarget;
    GLfloat mAnimFrameCount;

    GLuint *pVBO;
    GLuint *pVAO;

    void init() {
        pVertexBuffer = new FloatBuffer();
        pVertexEndBuffer = new FloatBuffer();
        pTextureBuffer = new FloatBuffer();
        pTransformBean = NULL;
        pMatrix = new Matrix();
        mProgramHandle = 0;
        mProjectionHandle = -1;
        mCameraHandle = -1;
        mTransformHandle = -1;
        mLightHandle = -1;
        mAnimHandle = -1;
        mAnimFrameCount = 0;
        mTextureId = 0;
        eTextureTarget = GL_TEXTURE_2D;
        pVBO = NULL;
        pVAO = NULL;
    }

    void clear() {
        if (pTextureBuffer != NULL) {
            delete pTextureBuffer;
            pTextureBuffer = NULL;
        }
        if (pVertexBuffer != NULL) {
            delete pVertexBuffer;
            pVertexBuffer = NULL;
        }
        if (pVertexEndBuffer != NULL) {
            delete pVertexEndBuffer;
            pVertexEndBuffer = NULL;
        }
        if (pMatrix != NULL) {
            delete pMatrix;
            pMatrix = NULL;
        }
        if (pVAO != NULL) {
            free(pVAO);
            pVAO = NULL;
        }
        if (pVBO != NULL) {
            free(pVBO);
            pVBO = NULL;
        }
    }
} GLBean;

const char gRectVertexShader[] =
        "#version 300 es                                                    \n"
        "layout (location = "STRV(SHADER_IN_POSITION)") in vec3 position;   \n"
        "layout (location = "STRV(SHADER_IN_TEX_COORDS)") in vec2 texCoord; \n"
        "out vec2 TexCoord;                                                 \n"
//        "uniform mat4 projection;                                           \n"
//        "uniform mat4 camera;                                               \n"
//        "uniform mat4 transform;                                            \n"
        "void main() {                                                      \n"
//        "  gl_Position = projection*camera*transform*vec4(position, 1.0);   \n"
        "  gl_Position = vec4(position, 1.0);   \n"
        "  TexCoord = vec2(texCoord.s, texCoord.t);                     \n"
        "}\n";

const char gRectFragmentShader[] =
        "#version 300 es                        \n"
        "precision mediump float;               \n"
        "in vec2 TexCoord;                      \n"
        "uniform sampler2D tTexture;            \n"
        "out vec4 color;                        \n"
        "void main() {                          \n"
        "  color = texture(tTexture, TexCoord); \n"
        "}\n";

const GLfloat rectVertex[][12] = {
        {
                -1, -1, 0.5,
                -1, 1, 0.5,
                1, -1, 0.5,
                1, 1, 0.5,
        }
};
const GLfloat rectTexture[][8] = {
        {
                0.0, 0.0,
                0.0, 1.0,
                1.0, 0.0,
                1.0, 1.0,
        }
};

class GLRenderer : public GLBase {
public:
    GLRenderer(TransformBean *bean);

    virtual ~GLRenderer();

    void onSurfaceChanged(GLuint w, GLuint h);

    GLint onSurfaceCreated();

    void onDrawFrame(Bitmap *bmp);

protected:
    GLBean *pBeanProcess;
    GLuint mWindowWidth;
    GLuint mWindowHeight;

    virtual void loadShader();

    virtual void prepareDraw(Bitmap *bmp);

    virtual void prepareProcessBuffer();

private :
    GLBean *pBeanDisplay;
    GLuint mFBOId;

    void configTexture(GLuint w, GLuint h);

    void prepareFBO();

    void prepareDisplayBuffer();

    void updateBuffer(GLBean *glBean);

    void draw(GLBean *glBean);
};

#endif //GL_RENDERER_H
