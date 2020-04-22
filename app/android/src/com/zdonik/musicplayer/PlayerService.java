package com.zdonik.musicplayer;

import android.content.Intent;
import android.content.Context;
import android.app.PendingIntent;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.R.drawable;
import android.widget.Toast;
import android.util.Log;
import android.os.Build;

import org.qtproject.qt5.android.bindings.QtService;
import net.vplay.helper.VPlayApplication;

public class PlayerService extends QtService {
    private NotificationManager mNM;

    private int NOTIFICATION = 1;
    private String CHANNEL_ID = "zdonik.music";

    public static void startMyService(Context ctx) {
        ctx.startService(new Intent(ctx, PlayerService.class));
    }

    @Override
    public void onCreate() {
        super.onCreate();

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
        CharSequence text = "Service started";

        // The PendingIntent to launch our activity if the user selects this notification
        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, VPlayApplication.class), 0);

        // Set the info for the views that show in the notification panel.
        Notification notification = new Notification.Builder(this, CHANNEL_ID)
                .setSmallIcon(17301635)  // the status icon
                .setTicker(text)  // the status text
                .setContentTitle("Best music started")  // the label of the entry
                .setContentText(text)  // the contents of the entry
                .setContentIntent(contentIntent)  // The intent to send when the entry is clicked
                .build();

        startForeground(NOTIFICATION, notification);
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
}
