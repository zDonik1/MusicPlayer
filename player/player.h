#pragma once

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QAndroidBinder>
#include <QTimer>

class Player : public QObject
{
    Q_OBJECT

public:
    Player();

    void setPlaylist(const QVariantList& list);
    void setPlay(bool play);
    int next();
    int previous();
    void seek(int64_t position);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void changeMusic(int index);

    void setServerBinder(const QAndroidBinder &serverBinder);

public slots:
    void onCurrentIndexChanged(int index);
    void onPositionTimerTimeout();
    void onPlayerStateChanged(QMediaPlayer::State state);

private:
    void checkShuffleRepeat();
    void debug(const QString &message); // helper function for debugging service

private:
    QMediaPlaylist m_playlist;
    QMediaPlayer m_mediaPlayer;
    QAndroidBinder m_serverBinder;
    QTimer m_positionTimer;

    bool m_shuffle = false;
    bool m_repeat = false;

    int m_nextIndex = -1;
};
