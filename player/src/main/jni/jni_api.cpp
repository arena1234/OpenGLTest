#include "jni_api.h"

GLRenderer *mGLDisplay = NULL;
Transform *mTransform = NULL;
Bean *mBean = NULL;
Bitmap *mBitmap = NULL;

jint JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnSurfaceCreated(JNIEnv *env,
                                                                      jobject obj) {
    LOGD("[jni_api]onSurfaceCreated");
    return mGLDisplay->onSurfaceCreated();
}

void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnSurfaceChanged(JNIEnv *env,
                                                                      jobject obj,
                                                                      jint width,
                                                                      jint height) {
    LOGD("[jni_api]onSurfaceChanged");
    mGLDisplay->onSurfaceChanged(width, height);
}

void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnDrawFrame(JNIEnv *env,
                                                                 jobject obj,
                                                                 jobject bmp) {
    if (bmp != NULL) {
        AndroidBitmapInfo bitmapInfo;
        int ret;
        if ((ret = AndroidBitmap_getInfo(env, bmp, &bitmapInfo)) < 0) {
            LOGE("[jni_api]AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }
        if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("[jni_api]Bitmap format is not RGBA_8888!");
            return;
        }

        if ((ret = AndroidBitmap_lockPixels(env, bmp, (void **) (&(mBitmap->pixels)))) < 0) {
            LOGE("[jni_api]AndroidBitmap_lockPixels() failed ! error=%d", ret);
            return;
        }
        AndroidBitmap_unlockPixels(env, bmp);
        mBitmap->bitmapInfo = bitmapInfo;
        mGLDisplay->onDrawFrame(mBitmap);
    } else {
        mGLDisplay->onDrawFrame(NULL);
    }
}

void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeInitApi(JNIEnv *env,
                                                             jobject obj,
                                                             jboolean pictureMode) {
    mBean = new Bean();
    mTransform = new Transform(mBean->getTransformBean());
    mBitmap = new Bitmap();
    if (pictureMode) {
        mGLDisplay = new Picture(mBean->getTransformBean());
    } else {
        mGLDisplay = new Video(mBean->getTransformBean());
    }
}

void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeReleaseApi(JNIEnv *env,
                                                                jobject obj) {
    LOGD("[jni_api]release");
    if (mGLDisplay != NULL) {
        delete mGLDisplay;
    }
    if (mTransform != NULL) {
        delete mTransform;
    }
    if (mBean != NULL) {
        delete mBean;
    }
    if (mBitmap != NULL) {
        delete mBitmap;
    }
}

jboolean JNICALL Java_com_wq_player_ndk_NdkPicture_nativeOnTouch(JNIEnv *env,
                                                                 jobject obj,
                                                                 jint action,
                                                                 jint pointCount,
                                                                 jfloat x1,
                                                                 jfloat y1,
                                                                 jfloat x2,
                                                                 jfloat y2) {
    //LOGD("onTouch action=%d, count=%d, 1(%f, %f), 2(%f, %f)", action, pointCount, x1, y1, x2, y2);
    mTransform->onTouch(action, pointCount, x1, y1, x2, y2);
    return false;
}

void JNICALL Java_com_wq_player_ndk_NdkPicture_nativeResetTransform(JNIEnv *env,
                                                                    jobject obj) {
    mTransform->reset();
}