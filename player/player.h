#pragma once

#include <QMediaPlayer>
#include <QAndroidBinder>

class Player : public QObject
{
    Q_OBJECT

public:
    Player();

    void playOrPause();
    void changeMusic(const QUrl &url);

    void setServerBinder(const QAndroidBinder &serverBinder);

public slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer m_mediaPlayer;
    QAndroidBinder m_serverBinder;
};
