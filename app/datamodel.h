#pragma once

#include <QObject>
#include <QAndroidBinder>
#include <QQmlListProperty>
#include <QDir>

#include "databasemanager.h"
#include "dirmodel.h"
#include "playlistmodel.h"

class DataModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DirModel *dirModel READ getDirModel NOTIFY dirModelChanged)
    Q_PROPERTY(PlaylistModel *playlistModel READ getPlaylistModel
               NOTIFY playlistModelChanged)

public:
    explicit DataModel(DatabaseManager &databaseManager,
                       const QAndroidBinder &binder, QObject *parent = nullptr);

    DirModel *getDirModel();
    PlaylistModel *getPlaylistModel();

public slots:
    void play();
    void next();
    void previous();
    void seek(double position);
    void shuffle();
    void repeat();
    void musicChanged(int index);

    void refreshDirs();
    void toggleDir(int index);

    void playlistAdded(QString name);
    void playlistEdited(int index, QString name);
    void playlistDeleted(int index);

signals:
    void dirModelChanged();
    void playlistModelChanged();

private:
    DatabaseManager &m_databaseManager;
    const QAndroidBinder &m_binder;

    std::unique_ptr<DirModel> m_dirModel;
    std::unique_ptr<PlaylistModel> m_playlistModel;
};
