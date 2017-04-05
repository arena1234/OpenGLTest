#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GLES3/gl3.h>
#include <math.h>
#include <stdlib.h>
#include "bean/bean_base.h"
#include "bean/region.h"
#include "transform/touch.h"
#include "log.h"

class Transform {
public:
    Transform(TransformBean *transformBean);

    ~Transform();

    GLboolean onTouch(GLuint action, GLuint pointerCount,
                      GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

    void reset();

    void setDefaultRegion();

    void limit(TransformBean *transformBean);

private:
    TransformBean *mTransformBean;
    Region *mRegion;
    Touch *mTouch;
};

#endif //TRANSFORM_H
