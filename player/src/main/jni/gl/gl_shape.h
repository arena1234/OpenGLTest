#ifndef GL_SHAPE_H
#define GL_SHAPE_H

#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "bean/bean_base.h"
#include "log.h"

#define POINT_SIZE_TEXTURE  2
#define POINT_SIZE_VERTEX   3
#define POINT_X             100
#define POINT_Y             2

class Shape{
public:
    GLfloat *textureBuffer;
    GLfloat *vertexBufferStart;
    GLfloat *vertexBufferEnd;
    GLuint textureSize;
    GLuint vertexSize;

    Shape();

    ~Shape();

    void cylinder();

private:
    GLuint64 getCurrentTime();
};

#endif //GL_SHAPE_H
