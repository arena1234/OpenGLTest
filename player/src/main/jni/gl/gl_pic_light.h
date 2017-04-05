#ifndef GL_PIC_LIGHT_H
#define GL_PIC_LIGHT_H

#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gl/gl_base.h"
#include "bean/bean_base.h"
#include "log.h"


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
                "out vec4 color;                        \n"
                "void main() {                          \n"
                "  color = texture(tTexture, TexCoord); \n"
                "}\n";

const GLfloat picVertex[][108] = {
        {
                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, -0.5f,  0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,

                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,

                0.5f,  0.5f,  0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,

                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f,  0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f,  0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
        }
};
const GLfloat picTexCoords[][72] = {
        {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,

                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,

                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                01.0f, 0.0f,

                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 1.0f,

                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 1.0f
        }
};

class PicLight : public GLBase {
public:
    PicLight(TransformBean *transformBean);

    ~PicLight();

protected:
    void updateFrame(Bitmap *bmp);

    void createTexture();

    void updateVertex();

    void updateTexCoord();

    GLuint loadShader();

private:
    GLboolean bFirstFrame;
};

#endif //GL_PIC_LIGHT_H
