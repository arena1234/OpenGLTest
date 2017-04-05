#ifndef BEAN_H
#define BEAN_H

#include <GLES3/gl3.h>
#include "transform/transform.h"
#include "bean/bean_base.h"
#include "gl/gl_picture.h"
#include "log.h"

class Bean {
public:
    Bean();

    ~Bean();

    void setTransformBean(TransformBean bean);

    void setTransformBean(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat fov);

    TransformBean *getTransformBean();

private:
    TransformBean *mTransformBean;
};

#endif //BEAN_H
