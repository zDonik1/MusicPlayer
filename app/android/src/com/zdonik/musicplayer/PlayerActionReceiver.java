package com.zdonik.musicplayer;

import com.zdonik.musicplayer.PlayerService;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

class NativeControls {
    public static native void previous();
    public static native void playPause();
    public static native void next();
    public static native void close();
}

public class PlayerActionReceiver extends BroadcastReceiver {
    private String TAG = "PlayerActionReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (action.equals("com.zdonik.musicplayer.PREVIOUS")) {
            NativeControls.previous();
            return;
        }
        if (action.equals("com.zdonik.musicplayer.PAUSE")) {
            NativeControls.playPause();
            return;
        }
        if (action.equals("com.zdonik.musicplayer.NEXT")) {
            NativeControls.next();
            return;
        }
        if (action.equals("com.zdonik.musicplayer.CLOSE")) {
            NativeControls.close();
            PlayerService.instance().quit();
            return;
        }
    }
}
