package com.wq.opemgltest;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.SeekBar;

import com.wq.opemgltest.ui.PlayButton;
import com.wq.opemgltest.ui.TouchButton;
import com.wq.player.VideoLayout;
import com.wq.player.player.PlayManager;

public class BasePlayerActivity extends BaseActivity implements
        View.OnClickListener,
        SeekBar.OnSeekBarChangeListener,
        PlayManager.Listener {
    protected VideoLayout mVideoLayout;
    private FrameLayout mFrameLayout;
    private UIManager mUIManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);

        mVideoLayout = new VideoLayout(this);
        mFrameLayout = (FrameLayout) findViewById(R.id.preview);
        mFrameLayout.addView(mVideoLayout);
        mUIManager = new UIManager(findViewById(R.id.layout_player_ctrl));
        mUIManager.setOnClickListener(this);
        mUIManager.setOnSeekBarChangeListener(this);

        setPlayerCtrlVisibility(true);
        setSource();
    }

    protected void setSource() {

    }

    @Override
    protected void setSystemUiState() {
        setSystemUIFull();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mVideoLayout.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mVideoLayout.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mVideoLayout.onDestroy();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        setPlayerCtrlVisibility(true);
        return mVideoLayout.onTouch(event);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_back:
                finish();
                break;
            case R.id.vr_btn_player_play:
                if (mUIManager.getBtnPlay().getState() == PlayButton.State.Pause) {
                    mVideoLayout.pause();
                } else {
                    mVideoLayout.play();
                }
                break;
            case R.id.vr_btn_angle_reset:
                mVideoLayout.resetTransform();
                break;
            case R.id.vr_btn_touch:
                if (mUIManager.getTouchButton().getState() == TouchButton.State.Touch) {
                } else {
                }
                break;
        }
        setPlayerCtrlVisibility(true);
    }

    private int userPlayerProgress = 0;
    private boolean isSeekDrag = false;

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        switch (seekBar.getId()) {
            case R.id.vr_player_progress:
                userPlayerProgress = progress;
                if (fromUser) setPlayerCtrlVisibility(true);
                break;
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        if (seekBar.getId() == R.id.vr_player_progress) {
            isSeekDrag = true;
        }
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        if (seekBar.getId() == R.id.vr_player_progress) {
            isSeekDrag = false;
            mVideoLayout.seekTo(userPlayerProgress);
        }
    }

    @Override
    public void updatePlayingProgress(int time) {
        if (!isSeekDrag) {
            mUIManager.updatePlayerProgress(time);
        }
    }

    @Override
    public void updateBufferProgress(int percent) {
        mUIManager.updateBufferPrograss(percent);
    }

    @Override
    public void onBufferingStart() {

    }

    @Override
    public void onBufferingFinish() {

    }

    @Override
    public void onCompletion() {
        finish();
    }

    @Override
    public void onPrepareFinish() {
        mUIManager.initPlayerProgress(mVideoLayout.getTotalTime());
    }

    @Override
    public void onPrepareStart() {
    }

    @Override
    public void onFrameUpdate() {

    }

    private void setPlayerCtrlVisibility(boolean visibility) {
        if (visibility) {
//            mUIManager.show();
            sendMsg(MSG_DRAW_PLAYER_CTRL_HIDE, 3000);
        } else {
            mUIManager.hide();
        }
    }

    private static final int MSG_DRAW_PLAYER_CTRL_HIDE = 1;
    private Handler mDelayHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case MSG_DRAW_PLAYER_CTRL_HIDE:
                    if (mVideoLayout.isPlaying()) {
                        setPlayerCtrlVisibility(false);
                    }
                    break;
            }
        }
    };

    private void sendMsg(int msg, int delay) {
        mDelayHandler.removeMessages(msg);
        mDelayHandler.sendEmptyMessageDelayed(msg, delay);
    }

    private void removeMsg() {
        mDelayHandler.removeMessages(MSG_DRAW_PLAYER_CTRL_HIDE);
    }
}
