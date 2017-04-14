#include "transform/transform.h"

Transform::Transform(TransformBean *transformBean) {
    mTransformBean = transformBean;
    mTouch = new Touch();
    mRegion = new Region();
    setDefaultRegion();
}

Transform::~Transform() {
    delete mTouch;
    delete mRegion;
}

void Transform::setDefaultRegion() {
    mRegion->setDefaultFov(FOV_MIN, FOV_MIN, FOV_DEFAULT);
    mRegion->setDefaultX(0, 0, 0);
    mRegion->setDefaultY(0, 0, 30);
    mRegion->setDefaultZ(0, 0, 0);
    reset();
}

void Transform::limit(TransformBean *transformBean) {
    mRegion->limit(transformBean);
}

GLboolean Transform::onTouch(GLuint action, GLuint pointerCount,
                             GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    TransformBean *bean = new TransformBean();
    bean->fov = mTransformBean->fov;
    bean->degreeX = mTransformBean->degreeX;
    bean->degreeY = mTransformBean->degreeY;
    bean->degreeZ = mTransformBean->degreeZ;
    GLboolean result = mTouch->onTouch(bean, action, pointerCount, x1, y1, x2, y2);
    limit(bean);
    mTransformBean->fov = bean->fov;
    mTransformBean->degreeX = bean->degreeX;
    mTransformBean->degreeY = bean->degreeY;
    mTransformBean->degreeZ = bean->degreeZ;
    delete bean;
    return result;
}

void Transform::reset() {
    mRegion->reset();
    mTransformBean->fov = mRegion->fov->value;
    mTransformBean->degreeX = mRegion->degreeX->value;
    mTransformBean->degreeY = mRegion->degreeY->value;
    mTransformBean->degreeZ = mRegion->degreeZ->value;
}