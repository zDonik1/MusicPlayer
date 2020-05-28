#pragma once

#include <QObject>
#include <QAndroidBinder>
#include <QQmlListProperty>
#include <QDir>

#include "databasemanager.h"
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

public:
    explicit DataModel(DatabaseManager &databaseManager,
                       const QAndroidBinder &binder, QObject *parent = nullptr);

    DirModel *getDirModel();
    RootDirModel *getRootDirModel();
    PlaylistModel *getPlaylistModel();
    MusicModel *getMusicModel();

public slots:
    void play();
    void next();
    void previous();
    void seek(double position);
    void shuffle();
    void repeat();
    void changeMusic(int index);

    void selectPlaylist(int index);
    void addPlaylist(QString name);
    void editPlaylist(int index, QString name);
    void deletePlaylist(int index);

    void refreshDirs();
    void toggleDir(int index);
    void addDirToPlaylist(int dirIndex, int playlistIndex);
    void addMusicToPlaylist(int musicIndex, int playlistIndex);
    void deleteMusicFromMemory(int index);

    void addMusicRootDir(const QUrl &path);
    void deleteMusicRootDir(int index);

    void dropTables();

signals:
    void dirModelChanged();
    void rootDirModelChanged();
    void playlistModelChanged();
    void musicModelChanged();

private:
    DatabaseManager &m_databaseManager;
    const QAndroidBinder &m_binder;

    std::unique_ptr<DirModel> m_dirModel;
    std::unique_ptr<RootDirModel> m_rootDirModel;
    std::unique_ptr<PlaylistModel> m_playlistModel;
    std::unique_ptr<MusicModel> m_musicModel;
};
