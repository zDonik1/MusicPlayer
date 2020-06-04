#pragma once

#include <QObject>

class SettingsDAO;

// AppState class stores and manages the state of the application
// and also provides access to the state in QML

class AppState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentPlaylist READ getCurrentPlaylistIndex
               NOTIFY currentPlaylistIndexChanged)
    Q_PROPERTY(QString currentPlaylistName READ getCurrentPlaylistName
               NOTIFY currentPlaylistNameChanged)

    Q_PROPERTY(int currentMusic READ getCurrentMusicIndex
               NOTIFY currentMusicIndexChanged)
    Q_PROPERTY(qint64 musicDuration READ getCurrentMusicDuration
               NOTIFY currentMusicDurationChanged)
    Q_PROPERTY(qint64 musicPosition READ getCurrentMusicPosition
               NOTIFY currentMusicPositionChanged)
    Q_PROPERTY(QString musicTitle READ getCurrentMusicTitle
               NOTIFY currentMusicTitleChanged)
    Q_PROPERTY(QString isMusicDefaultImage READ isCurrentMusicDefaultImage
               NOTIFY isCurrentMusicDefaultImageChanged)

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isShuffle READ getShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool isRepeat READ getRepeat NOTIFY repeatChanged)


public:
    AppState(SettingsDAO &settings);

    void load();
    void save();

public:
    int getCurrentPlaylistIndex() const;
    const QString &getCurrentPlaylistName() const;

    int getCurrentMusicIndex() const;
    qint64 getCurrentMusicDuration() const;
    qint64 getCurrentMusicPosition() const;
    QString getCurrentMusicTitle() const;
    bool isCurrentMusicDefaultImage() const;

    bool isPlaying() const;
    bool getShuffle() const;
    bool getRepeat() const;


    void setCurrentPlaylistIndex(int index);
    void setCurrentPlaylistName(const QString &name);

    void setCurrentMusicIndex(int index);
    void setCurrentMusicDuration(int64_t duration);
    void setCurrentMusicPosition(int64_t position);
    void setCurrentMusicTitle(QString title);
    void setIsCurrentMusicDefaultImage(bool isDefaultImage);

    void setIsPlaying(bool playing);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);

signals:
    void currentPlaylistIndexChanged();
    void currentPlaylistNameChanged();

    void currentMusicIndexChanged();
    void currentMusicDurationChanged();
    void currentMusicPositionChanged();
    void currentMusicTitleChanged();
    void isCurrentMusicDefaultImageChanged();

    void isPlayingChanged();
    void shuffleChanged();
    void repeatChanged();

private:
    SettingsDAO &m_settings;

    int m_currentPlaylistIndex = -1;
    QString m_currentPlaylistName = "";

    int m_currentMusicIndex = -1;
    int64_t m_currentMusicDuration = 0;
    int64_t m_currentMusicPosition = 0;
    QString m_currentMusicTitle = "";
    bool m_isCurrentMusicDefaultImage = true;

    bool m_isPlaying = false;
    bool m_shuffle = false;
    bool m_repeat = false;
};
