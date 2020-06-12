#pragma once

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QAndroidBinder>
#include <QAndroidJniObject>
#include <QTimer>

class Player : public QObject
{
    Q_OBJECT

public:
    Player();
    ~Player();

    void debug(const QString &message); // helper function for debugging service
    QList<QMediaContent> varListToMediaContentList(const QVariantList &list);
    bool load();
    void save();

    const QAndroidBinder &getServerBinder() const;
    bool getPlay() const;
    int musicCount() const;
    bool isStopped() const;

    void setPlaylist(const QList<QMediaContent> &musicUrls);
    void addMusicToPlaylist(const QList<QMediaContent> &musicUrls);
    void clearPlaylist();
    void sendPlaybackState();

    void setPlay(bool play);
    void next();
    void previous();
    void seek(int64_t position);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void setMusicIndex(int index, bool update = true);

    void setServerBinder(const QAndroidBinder &serverBinder);

private slots:
    void onCurrentIndexChanged(int index);
    void onPlayerStateChanged(QMediaPlayer::State state);
    void onPositionTimerTimeout();

private:
    void setRandomIndex(bool forced);
    void sendChangedIndex();

private:
    QMediaPlaylist m_playlist;
    QMediaPlayer m_mediaPlayer;
    QAndroidBinder m_serverBinder;
    QTimer m_positionTimer;
    QAndroidJniObject m_javaService;

    bool m_shuffle = false;
    bool m_repeat = false;

    bool m_generateRandomIndex = true;
    bool m_updateMusicIndexOnce = true;

    const QString playlistFilename = "playlist.pl";
    const QString settingsFilename = "settings.dat";
};
