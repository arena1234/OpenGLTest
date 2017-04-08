#ifndef PIC_LEFT_H
#define PIC_LEFT_H

#include <jni.h>
#include "gl/gl_base.h"
#include "gl/gl_fbo.h"
#include "gl/gl_video.h"
#include "bean/bean.h"
#include "transform/transform.h"

extern "C" {
// left
JNIEXPORT jint JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnSurfaceCreated(JNIEnv *env,
                                                                                jobject obj);
JNIEXPORT void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnSurfaceChanged(JNIEnv *env,
                                                                                jobject obj,
                                                                                jint width,
                                                                                jint height);
JNIEXPORT void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnDrawFrame(JNIEnv *env,
                                                                           jobject obj,
                                                                           jobject bmp);
JNIEXPORT void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeInitApi(JNIEnv *env,
                                                                       jobject obj,
                                                                       jboolean pictureMode);
JNIEXPORT void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeReleaseApi(JNIEnv *env,
                                                                          jobject obj);
JNIEXPORT jboolean JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnTouch(JNIEnv *env,
                                                                           jobject obj,
                                                                           jint action,
                                                                           jint pointCount,
                                                                           jfloat x1,
                                                                           jfloat y1,
                                                                           jfloat x2,
                                                                           jfloat y2);
JNIEXPORT void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeResetTransform(JNIEnv *env,
                                                                              jobject obj);
}
#endif //PIC_LEFT_H
