#ifndef BEAN_BASE_H
#define BEAN_BASE_H

#include <GLES3/gl3.h>

#define TO_DEGREES(a)       (a) * 180 / M_PI
#define TO_RADIANS(a)       (a) / 180 * M_PI

static const GLfloat FOV_MIN = 20;
static const GLfloat FOV_MAX = 70;
static const GLfloat FOV_DEFAULT = 54;

typedef struct {
    GLfloat degreeX;
    GLfloat degreeY;
    GLfloat degreeZ;
    GLfloat fov;
} TransformBean;

typedef struct {
    GLfloat x;
    GLfloat y;
} Point2;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Point3;

#endif //BEAN_BASE_H
