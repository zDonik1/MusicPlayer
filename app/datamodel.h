#pragma once

#include <QObject>
#include <QAndroidBinder>
#include <QDir>

#include "metadatascanner.h"

#include "dirmodel.h"
#include "rootdirmodel.h"
#include "playlistmodel.h"
#include "musicmodel.h"

class DatabaseManager;
class AppState;
class MusicImageProvider;

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

public:
    explicit DataModel(DatabaseManager &getDatabaseManager,
                       AppState &appState,
                       QObject *parent = nullptr);
    ~DataModel();

    const QAndroidBinder &getClientBinder() const;
    AppState &getAppState() const;

    void setClientBinder(const QAndroidBinder &clientBinder);
    void setMusicImageProvider(MusicImageProvider *imageProvider);

public:
    DirModel *getDirModel();
    RootDirModel *getRootDirModel();
    PlaylistModel *getPlaylistModel();
    MusicModel *getMusicModel();

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

    Q_INVOKABLE void dropTables();
    Q_INVOKABLE void onPlayerPageLoaded();

public slots:
    void onMetaDataReady();

signals:
    void imagesReady();

    void dirModelChanged();
    void rootDirModelChanged();
    void playlistModelChanged();
    void musicModelChanged();

private slots:
    void onAllMusicChanged(int playlistId);

private:
    void fetchMetaDataForAllMusic();
    void updateCurrentMusicData();

private:
    DatabaseManager &m_databaseManager;
    AppState &m_appState;
    MusicImageProvider *m_imageProvider = nullptr;

    QAndroidBinder m_clientBinder;
    MetaDataScanner m_metaDataScanner;

    std::unique_ptr<DirModel> m_dirModel;
    std::unique_ptr<RootDirModel> m_rootDirModel;
    std::unique_ptr<PlaylistModel> m_playlistModel;
    std::unique_ptr<MusicModel> m_musicModel;
};
