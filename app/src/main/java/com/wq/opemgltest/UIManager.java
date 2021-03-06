package com.wq.opemgltest;

import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import com.wq.opemgltest.ui.PlayButton;
import com.wq.opemgltest.ui.TouchButton;

/**
 * Created by qiangwang on 8/31/16.
 */
public class UIManager {
    private View mParent;
    private RelativeLayout mLayout;
    private PlayButton mBtnPlay;
    private SeekBar mSeekPlayer;
    private TextView mTextCurrTime, mTextTotalTime;
    private Button mBtnReset;
    private TouchButton mBtnTouch;
    private ImageButton mBtnExit;

    public UIManager(View parent) {
        mParent = parent;
        mLayout = (RelativeLayout) mParent.findViewById(R.id.layout_player_ctrl);
        mBtnPlay = (PlayButton) mParent.findViewById(R.id.vr_btn_player_play);
        mBtnReset = (Button) mParent.findViewById(R.id.vr_btn_angle_reset);
        mSeekPlayer = (SeekBar) mParent.findViewById(R.id.vr_player_progress);
        mTextCurrTime = (TextView) mParent.findViewById(R.id.vr_seekbar_curr_time);
        mTextTotalTime = (TextView) mParent.findViewById(R.id.vr_seekbar_total_time);
        mBtnTouch = (TouchButton) mParent.findViewById(R.id.vr_btn_touch);
        mBtnExit = (ImageButton) mParent.findViewById(R.id.btn_back);
    }

    public void initPlayerProgress(int totalTime) {

        mSeekPlayer.setMax(totalTime);
        mSeekPlayer.setProgress(0);
        mTextTotalTime.setText(msToTime(totalTime));
        mTextCurrTime.setText(msToTime(0));
    }

    public void updatePlayerProgress(int currTime) {
        mSeekPlayer.setProgress(currTime);
        mTextCurrTime.setText(msToTime(currTime));
    }

    public void updateBufferPrograss(int percent) {
        mSeekPlayer.setSecondaryProgress(percent * mSeekPlayer.getMax() / 100);
    }

    public Button getBtnReset() {
        return mBtnReset;
    }

    public PlayButton getBtnPlay() {
        return mBtnPlay;
    }

    public TouchButton getTouchButton() {
        return mBtnTouch;
    }

    public void show() {
        mLayout.setVisibility(View.VISIBLE);
    }

    public void hide() {
        mLayout.setVisibility(View.GONE);
    }

    public boolean isShowing() {
        return mLayout.getVisibility() == View.VISIBLE;
    }

    public void setOnClickListener(View.OnClickListener listener) {
        mBtnPlay.setOnClickListener(listener);
        mBtnReset.setOnClickListener(listener);
        mBtnTouch.setOnClickListener(listener);
        mBtnExit.setOnClickListener(listener);
    }

    public void setOnSeekBarChangeListener(SeekBar.OnSeekBarChangeListener listener) {
        mSeekPlayer.setOnSeekBarChangeListener(listener);
    }

    private String msToTime(int ms) {
        ms = ms / 1000;
        if (ms % 60 < 10)
            return "" + ms / 60 + ":0" + ms % 60;
        else
            return "" + ms / 60 + ":" + ms % 60;
    }
}
