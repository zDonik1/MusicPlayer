package com.zdonik.musicplayer;

import com.zdonik.musicplayer.PlayerActionReceiver;

import android.content.Intent;
import android.content.Context;
import android.app.PendingIntent;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.Notification.MediaStyle;
import android.app.NotificationChannel;
import android.R.drawable;
import android.widget.Toast;
import android.util.Log;
import android.os.Build;

import org.qtproject.qt5.android.bindings.QtService;
import net.vplay.helper.VPlayApplication;
import net.vplay.helper.VPlayActivity;

public class PlayerService extends QtService {
    private static PlayerService service = null;

    private static NotificationManager mNM;

    private static int NOTIFICATION = 1;
    private String CHANNEL_ID = "zdonik.music";

    private static boolean isPlaying = false;
    private static Notification.Builder notificationBuilder;

    private PendingIntent prevPendingIntent;
    private PendingIntent pausePendingIntent;
    private PendingIntent nextPendingIntent;
    private PendingIntent closePendingIntent;

    public static PlayerService instance() {
        return service;
    }

    public static void startMyService(Context ctx) {
        ctx.startService(new Intent(ctx, PlayerService.class));
    }

    @Override
    public void onCreate() {
        super.onCreate();

        service = this;
        mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);

        createNotificationChannel();
        showNotification();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
        return START_STICKY;
    }

    private void showNotification() {
        Intent prevIntent = new Intent(this, PlayerActionReceiver.class);
        prevIntent.setAction("com.zdonik.musicplayer.PREVIOUS");
        prevPendingIntent = PendingIntent.getBroadcast(this, 0, prevIntent, 0);

        Intent pauseIntent = new Intent(this, PlayerActionReceiver.class);
        pauseIntent.setAction("com.zdonik.musicplayer.PAUSE");
        pausePendingIntent = PendingIntent.getBroadcast(this, 0, pauseIntent, 0);

        Intent nextIntent = new Intent(this, PlayerActionReceiver.class);
        nextIntent.setAction("com.zdonik.musicplayer.NEXT");
        nextPendingIntent = PendingIntent.getBroadcast(this, 0, nextIntent, 0);

        Intent closeIntent = new Intent(this, PlayerActionReceiver.class);
        closeIntent.setAction("com.zdonik.musicplayer.CLOSE");
        closePendingIntent = PendingIntent.getBroadcast(this, 0, closeIntent, 0);

        // The PendingIntent to launch our activity if the user selects this notification
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
            new Intent(this, VPlayActivity.class), 0);

        // Set the info for the views that show in the notification panel.
        notificationBuilder = new Notification.Builder(this, CHANNEL_ID)
            .setVisibility(Notification.VISIBILITY_PUBLIC)
            .setStyle(new Notification.MediaStyle()
                .setShowActionsInCompactView(0, 1, 2))
//            .setLargeIcon(albumArtBitmap)
            .setOnlyAlertOnce(true)
            .setSmallIcon(17301635)  // the status icon
            .setTicker("Music started")  // the status text
            .setContentTitle("Playing music")  // the label of the entry
            .setContentText("")  // the contents of the entry
            .setContentIntent(contentIntent);  // The intent to send when the entry is clicked
        setActions();

        startForeground(NOTIFICATION, notificationBuilder.build());
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = "Music";
            String description = "Notification for music being played";
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            mNM.createNotificationChannel(channel);
        }
    }

    public void setPlay(boolean play) {
        isPlaying = play;
        updateNotification();
    }

    private void updateNotification() {
        setActions();
        mNM.notify(NOTIFICATION, notificationBuilder.build());
    }

    public void quit() {
        stopForeground(true);
    }

    private void setActions() {
        Notification.Action prevAction =
            new Notification.Action(17301541, "Previous", prevPendingIntent);
        Notification.Action pauseAction =
            new Notification.Action(isPlaying ? 17301539 : 17301540,
                "Pause", pausePendingIntent);
        Notification.Action nextAction =
            new Notification.Action(17301538, "Next", nextPendingIntent);
        Notification.Action closeAction =
            new Notification.Action(17301560, "Close", closePendingIntent);
        notificationBuilder.setActions(prevAction, pauseAction,
            nextAction, closeAction);
    }
}
