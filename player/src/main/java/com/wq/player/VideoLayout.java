package com.wq.player;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.LinearLayout;

import com.wq.player.opengl.VideoSurfaceView;
import com.wq.player.player.PlayManager;

/**
 * Created by qiangwang on 1/16/17.
 */

public class VideoLayout extends LinearLayout implements PlayManager.Listener, VideoSurfaceView.TextureIdListener {
    private static final String TAG = "VR_VideoLayout";
    private VideoSurfaceView mSurfaceView = null;
    private PlayState mPlayState = PlayState.UnPrepare;
    private PlayManager mPlayManager = null;
    private PlayManager.Listener mPlayerListener;
    private boolean isInitSource = false;
    private boolean isPictureMode = false;    // 图片模式、视频模式
    private SurfaceTexture mSurfaceTexture;
    private int fps = 30;

    public VideoLayout(Context context) {
        super(context);
        init();
    }

    public VideoLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        setPlayState(PlayState.UnPrepare);
    }

    private void initSurface(boolean isPictureMode) {
        LayoutParams lp = new LayoutParams(
                LayoutParams.MATCH_PARENT,
                LayoutParams.MATCH_PARENT);
        lp.weight = 1;
        mSurfaceView = new VideoSurfaceView(getContext(), isPictureMode);
        mSurfaceView.setTextureIdListener(this);
        addView(mSurfaceView, lp);
    }

    public void setVideoSource(final String path, PlayManager.Listener listener) {
        if (isInitSource) return;
        initSurface(false);

        isInitSource = true;
        mPlayManager = new PlayManager();
        isPictureMode = false;
        removeMsg();
        setPlayState(PlayState.UnPrepare);
        mPlayerListener = listener;
        mPlayManager.initMedia(Uri.parse(path), getContext());
        L.d(TAG, "setVideoSource path=" + path);
    }

    public void setUseBitmap(PlayManager.Listener listener) {
        if (isInitSource) return;
        initSurface(true);

        isInitSource = true;
        isPictureMode = true;
        mPlayerListener = listener;
        release();
        setPlayState(PlayState.UnPrepare);
        setFps(30);
        removeMsg();
        L.d(TAG, "setUseBitmap");

        prepare(-1);
        onPrepareStart();
        onPrepareFinish();
    }

    public void updateFrame(Bitmap bmp) {
        if (isPictureMode) {
            mSurfaceView.updateFrame(bmp);
        }
    }

    public void setFps(int fps) {
        if (fps < 20) {
            fps = 20;
        }
        this.fps = fps;
        L.d(TAG, "setFps fps=" + fps);
    }

    private boolean isUsePlayer() {
        return !isPictureMode && mPlayManager != null;
    }

    public void resetTransform() {
        mSurfaceView.resetTransform();
    }

    private void addListener() {
        if (!isUsePlayer()) return;
        mPlayManager.setListener(this);
    }

    private void removeListener() {
        if (!isUsePlayer()) return;
        mPlayManager.setListener(null);
    }

    public void onResume() {
        L.d(TAG, "onResume");
        addListener();
        mSurfaceView.resume();
        play();
    }

    public void onPause() {
        L.d(TAG, "onPause");
        removeListener();
        mSurfaceView.pause();
        pause();
        removeMsg();
    }

    public void onDestroy() {
        L.d(TAG, "onDestroy");
        mSurfaceView.destroy();
        removeView(mSurfaceView);
        release();
    }

    public boolean onTouch(MotionEvent event) {
        return mSurfaceView.onTouch(event);
    }

    private void setPlayState(PlayState state) {
        mPlayState = state;
        L.d(TAG, "setPlayState : " + mPlayState);
    }

    private PlayState getPlayState() {
        return mPlayState;
    }

    @Override
    public void onTextureId(int id) {
        prepare(id);
    }

    @Override
    public void updatePlayingProgress(int time) {
        if (mPlayerListener != null) mPlayerListener.updatePlayingProgress(time);
    }

    @Override
    public void updateBufferProgress(int percent) {
        if (mPlayerListener != null) mPlayerListener.updateBufferProgress(percent);
    }

    @Override
    public void onBufferingStart() {
        if (mPlayerListener != null) mPlayerListener.onBufferingStart();
    }

    @Override
    public void onBufferingFinish() {
        if (mPlayerListener != null) mPlayerListener.onBufferingFinish();
    }

    @Override
    public void onCompletion() {
        L.d(TAG, "PlayManager onCompletion");
        if (mPlayerListener != null) mPlayerListener.onCompletion();
    }

    @Override
    public void onPrepareStart() {
        L.d(TAG, "PlayManager onPrepareStart");
        if (mPlayerListener != null) mPlayerListener.onPrepareStart();
    }

    @Override
    public void onPrepareFinish() {
        L.d(TAG, "PlayManager onPrepareFinish");
        setPlayState(PlayState.Prepared);
        play();
        if (mPlayerListener != null) mPlayerListener.onPrepareFinish();
    }

    @Override
    public void onFrameUpdate() {
        //L.d(TAG, "onFrameUpdate");
        mSurfaceView.requestRender();
        if (mPlayerListener != null) mPlayerListener.onFrameUpdate();
    }

    private void prepare(int textureId) {
        if (getPlayState() == PlayState.UnPrepare) {
            setPlayState(PlayState.Preparing);
            if (isUsePlayer()) {
                addListener();
                mSurfaceTexture = new SurfaceTexture(textureId);
                mSurfaceView.setSurfaceTexture(mSurfaceTexture);
                mPlayManager.prepareMediaSurface(mSurfaceTexture);
            }
        }
    }

    private void release() {
        if (getPlayState() != PlayState.UnPrepare) {
            setPlayState(PlayState.Release);
            removeMsg();
            if (isUsePlayer()) {
                mPlayManager.setListener(null);
                mPlayManager.stop();
                mPlayManager.release();
                mPlayManager = null;
            }
        }
    }

    public void play() {
        if (getPlayState() == PlayState.Prepared || getPlayState() == PlayState.Pause) {
            setPlayState(PlayState.Playing);
            removeMsg();
            if (isUsePlayer()) {
                mPlayManager.play();
            }
        }
    }

    public void pause() {
        if (getPlayState() == PlayState.Playing) {
            setPlayState(PlayState.Pause);
            removeMsg();
            refreshFrame();
            if (isUsePlayer()) {
                mPlayManager.pause();
            }
        }
    }

    public boolean isPlaying() {
        return getPlayState() == PlayState.Playing;
    }

    public void seekTo(int ms) {
        if (!isUsePlayer()) return;
        mPlayManager.seekTo(ms);
    }

    public int getTotalTime() {
        if (!isUsePlayer()) return 0;
        return mPlayManager.getTotalTime();
    }

    public int getCurrentTime() {
        if (!isUsePlayer()) return 0;
        return mPlayManager.getCurrentTime();
    }

    private enum PlayState {
        UnPrepare,
        Preparing,
        Prepared,
        Playing,
        Pause,
        Release,
    }

    private static final int MSG_PAUSE_REFRESH_FRAME = 1;
    Handler mRefreshHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case MSG_PAUSE_REFRESH_FRAME:
                    mSurfaceView.requestRender();
                    refreshFrame();
                    break;
            }
        }
    };

    private void refreshFrame() {
        mRefreshHandler.sendEmptyMessageDelayed(MSG_PAUSE_REFRESH_FRAME, (long) (1000.0 / fps));
    }

    private void removeMsg() {
        mRefreshHandler.removeMessages(MSG_PAUSE_REFRESH_FRAME);
    }
}
