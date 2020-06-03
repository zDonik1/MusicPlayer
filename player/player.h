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

    void setPlaylist(const QList<QMediaContent> &musicUrls);
    void addMusicToPlaylist(const QList<QMediaContent> &musicUrls);

    void setPlay(bool play);
    void next();
    void previous();
    void seek(int64_t position);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void setMusicIndex(int index, bool update = true);

    void setServerBinder(const QAndroidBinder &serverBinder);

public slots:
    void onCurrentIndexChanged(int index);
    void onPlayerStateChanged(QMediaPlayer::State state);
    void onPositionTimerTimeout();

private:
    void debug(const QString &message); // helper function for debugging service
    void setRandomIndex(bool forced);
    void sendChangedIndex();

private:
    QMediaPlaylist m_playlist;
    QMediaPlayer m_mediaPlayer;
    QAndroidBinder m_serverBinder;
    QTimer m_positionTimer;

    bool m_shuffle = false;
    bool m_repeat = false;

    bool m_generateRandomIndex = true;
    bool m_updateMusicIndex = true;
};
