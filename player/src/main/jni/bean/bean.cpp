#include "bean/bean.h"

Bean::Bean() {
    mTransformBean = new TransformBean();

    setTransformBean(0, 0, 0, FOV_DEFAULT);
}

Bean::~Bean() {
    delete mTransformBean;
}

void Bean::setTransformBean(TransformBean bean) {
    LOGD("[bean:setTransformBean]last bean degree(%f, %f, %f), zoom=%f",
         mTransformBean->degreeX, mTransformBean->degreeY,
         mTransformBean->degreeZ, mTransformBean->fov);
    if (mTransformBean->degreeX != bean.degreeX) {
        mTransformBean->degreeX = bean.degreeX;
    }

    if (mTransformBean->degreeY != bean.degreeY) {
        mTransformBean->degreeY = bean.degreeY;
    }
    if (mTransformBean->degreeZ != bean.degreeZ) {
        mTransformBean->degreeZ = bean.degreeZ;
    }
    if (mTransformBean->fov != bean.fov) {
        mTransformBean->fov = bean.fov;
    }
    LOGD("[bean:setTransformBean]curr bean degree(%f, %f, %f), zoom=%f",
         mTransformBean->degreeX, mTransformBean->degreeY,
         mTransformBean->degreeZ, mTransformBean->fov);
}

void Bean::setTransformBean(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat fov) {
    LOGD("[bean:setTransformBean]last bean degree(%f, %f, %f), zoom=%f",
         mTransformBean->degreeX, mTransformBean->degreeY,
         mTransformBean->degreeZ, mTransformBean->fov);
    if (mTransformBean->degreeX != rx) {
        mTransformBean->degreeX = rx;
    }

    if (mTransformBean->degreeY != ry) {
        mTransformBean->degreeY = ry;
    }
    if (mTransformBean->degreeZ != rz) {
        mTransformBean->degreeZ = rz;
    }
    if (mTransformBean->fov != fov) {
        mTransformBean->fov = fov;
    }
    LOGD("[bean:setTransformBean]curr bean degree(%f, %f, %f), zoom=%f",
         mTransformBean->degreeX, mTransformBean->degreeY,
         mTransformBean->degreeZ, mTransformBean->fov);
}

TransformBean *Bean::getTransformBean() {
    return mTransformBean;
}