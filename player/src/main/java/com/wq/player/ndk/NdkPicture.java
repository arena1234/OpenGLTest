package com.wq.player.ndk;

import android.graphics.Bitmap;

/**
 * Created by qiangwang on 3/6/17.
 */

public class NdkPicture {
    public NdkPicture(boolean pictureMode) {
        nativeInitApi(pictureMode);
    }

    public int onSurfaceCreated() {
        return nativeOnSurfaceCreated();
    }

    public void onSurfaceChanged(int width, int height) {
        nativeOnSurfaceChanged(width, height);
    }

    public void onDrawFrame(Bitmap bmp) {
        nativeOnDrawFrame(bmp);
    }

    public void releaseApi() {
        nativeReleaseApi();
    }

    public boolean onTouch(int action, int pointCount, float x1, float y1, float x2, float y2) {
        return nativeOnTouch(action, pointCount, x1, y1, x2, y2);
    }

    public void resetTransform(){
        nativeResetTransform();
    }

    private native int nativeOnSurfaceCreated();

    private native void nativeOnSurfaceChanged(int width, int height);

    private native void nativeOnDrawFrame(Bitmap bmp);

    private native void nativeInitApi(boolean pictureMode);

    private native void nativeReleaseApi();

    private native boolean nativeOnTouch(int action, int pointCount,
                                         float x1, float y1, float x2, float y2);

    private native void nativeResetTransform();

    static {
        System.loadLibrary("player_jni");
    }
}
