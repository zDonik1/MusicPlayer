#pragma once

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QAndroidBinder>

class Player : public QObject
{
    Q_OBJECT

public:
    Player();

    void setPlaylist(const QVariantList& list);
    void setPlay(bool play);
    void next();
    void previous();
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void changeMusic(int index);

    void setServerBinder(const QAndroidBinder &serverBinder);

public slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    void checkShuffleRepeat();

private:
    QMediaPlaylist m_playlist;
    QMediaPlayer m_mediaPlayer;
    QAndroidBinder m_serverBinder;

    bool m_shuffle = false;
    bool m_repeat = false;
};
