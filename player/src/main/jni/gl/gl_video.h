#ifndef GL_VIDEO_H
#define GL_VIDEO_H

#include <GLES3/gl3.h>
#include <GLES/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gl/gl_base.h"
#include "bean/bean_base.h"
#include "log.h"

const char gVideoVertexShader[] =
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

const char gVideoFragmentShader[] =
        "#version 300 es                        \n"
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;               \n"
        "in vec2 TexCoord;                      \n"
        "uniform samplerExternalOES tTexture;   \n"
        "out vec4 color;                        \n"
        "void main() {                          \n"
        "  color = texture(tTexture, TexCoord); \n"
        "}\n";


const GLfloat videoVertex[][12] = {
        {
                -2,   -0.5, -1,
                -2,   0.5, -1,
                -1,  -0.5, -1,
                -1,  0.5, -1,
        },
        {
                -0.5, -0.5, -1,
                -0.5, 0.5, -1,
                0.5, -0.5, -1,
                0.5, 0.5, -1,
        },
        {
                1,    -0.5, -1,
                1,    0.5, -1,
                2,   -0.5, -1,
                2,   0.5, -1,
        },
};
const GLfloat videoTexCoords[][8] = {
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

class Video : public GLBase {
public:
    Video(TransformBean *transformBean);

    ~Video();

protected:
    void updateFrame(Bitmap *bmp);

    void createTexture();

    void updateVertex();

    void updateTexCoord();

    GLuint loadShader();
};

#endif //GL_VIDEO_H
