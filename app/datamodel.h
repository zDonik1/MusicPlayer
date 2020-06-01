#pragma once

#include <QObject>
#include <QAndroidBinder>
#include <QQmlListProperty>
#include <QDir>

#include "databasemanager.h"
#include "metadatascanner.h"
#include "musicimageprovider.h"

#include "dirmodel.h"
#include "rootdirmodel.h"
#include "playlistmodel.h"
#include "musicmodel.h"

class DataModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DirModel *dirModel READ getDirModel NOTIFY dirModelChanged)
    Q_PROPERTY(RootDirModel *rootDirModel READ getRootDirModel
               NOTIFY rootDirModelChanged)
    Q_PROPERTY(PlaylistModel *playlistModel READ getPlaylistModel
               NOTIFY playlistModelChanged)
    Q_PROPERTY(MusicModel *musicModel READ getMusicModel
               NOTIFY musicModelChanged)

    Q_PROPERTY(QString currentPlaylistName READ getCurrentPlaylistName
               NOTIFY currentPlaylistNameChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isShuffle READ getShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool isRepeat READ getRepeat NOTIFY repeatChanged)
    Q_PROPERTY(qint64 musicDuration READ getCurrentMusicDuration
               NOTIFY currentMusicDurationChanged)
    Q_PROPERTY(qint64 musicPosition READ getCurrentMusicPosition
               NOTIFY currentMusicPositionChanged)

public:
    explicit DataModel(DatabaseManager &databaseManager,
                       QObject *parent = nullptr);

    const QAndroidBinder &getClientBinder() const;

    void setClientBinder(const QAndroidBinder &clientBinder);
    void setMusicImageProvider(MusicImageProvider *imageProvider);
    void setCurrentMusicPosition(int64_t position);

public:
    DirModel *getDirModel();
    RootDirModel *getRootDirModel();
    PlaylistModel *getPlaylistModel();
    MusicModel *getMusicModel();

    const QString &getCurrentPlaylistName() const;
    bool isPlaying() const;
    bool getShuffle() const;
    bool getRepeat() const;
    qint64 getCurrentMusicDuration() const;
    qint64 getCurrentMusicPosition() const;

public:
    Q_INVOKABLE void play();
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();
    Q_INVOKABLE void seek(double position);
    Q_INVOKABLE void shuffle();
    Q_INVOKABLE void repeat();
    Q_INVOKABLE void changeMusic(int index);

    Q_INVOKABLE void selectPlaylist(int index);
    Q_INVOKABLE void addPlaylist(QString name);
    Q_INVOKABLE void editPlaylist(int index, QString name);
    Q_INVOKABLE void deletePlaylist(int index);

    Q_INVOKABLE void refreshDirs();
    Q_INVOKABLE void toggleDir(int index);
    Q_INVOKABLE void addDirToPlaylist(int dirIndex, int playlistIndex);
    Q_INVOKABLE void addMusicToPlaylist(int musicIndex, int playlistIndex);
    Q_INVOKABLE void deleteMusicFromMemory(int index);

    Q_INVOKABLE void addMusicRootDir(const QUrl &path);
    Q_INVOKABLE void deleteMusicRootDir(int index);

public slots:
    void dropTables();
    void onMetaDataReady();

signals:
    void imagesReady();

    void dirModelChanged();
    void rootDirModelChanged();
    void playlistModelChanged();
    void musicModelChanged();

    void currentPlaylistNameChanged();
    void isPlayingChanged();
    void shuffleChanged();
    void repeatChanged();
    void currentMusicDurationChanged();
    void currentMusicPositionChanged();

private:
    void initializePlayer();
    void setupPlayerToPlaylist(int id);
    void updateOnMusicChanged(int index);

private:
    DatabaseManager &m_databaseManager;
    MusicImageProvider *m_imageProvider = nullptr;

    QAndroidBinder m_clientBinder;
    MetaDataScanner m_metaDataScanner;

    std::unique_ptr<DirModel> m_dirModel;
    std::unique_ptr<RootDirModel> m_rootDirModel;
    std::unique_ptr<PlaylistModel> m_playlistModel;
    std::unique_ptr<MusicModel> m_musicModel;

    QString m_currentPlaylistName;
    bool m_isPlaying = false;
    bool m_shuffle = false;
    bool m_repeat = false;
    int64_t m_currentMusicDuration = 0;
    int64_t m_currentMusicPosition = 0;
};
