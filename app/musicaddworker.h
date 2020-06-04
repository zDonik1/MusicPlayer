#pragma once

#include <QObject>

class AppState;
class MetaDataScanner;
class QAndroidBinder;
class DirModel;
class PlaylistModel;
class MusicModel;
class MusicDAO;
class PlaylistMusicDAO;

class MusicAddWorker : public QObject
{
    Q_OBJECT

public:
    MusicAddWorker(AppState &appState, MetaDataScanner &metaDataScanner,
                   QAndroidBinder &clientBinder,
                   DirModel &dirModel, PlaylistModel &playlistModel,
                   MusicModel &musicModel, MusicDAO &musicDAO,
                   PlaylistMusicDAO &playlistMusicDAO, int dirIndex,
                   int playlistIndex, QObject *parent = nullptr);

public slots:
    void process();

signals:
    void finished();

private:
    AppState &m_appState;
    MetaDataScanner &m_metaDataScanner;
    QAndroidBinder &m_clientBinder;
    DirModel &m_dirModel;
    PlaylistModel &m_playlistModel;
    MusicModel &m_musicModel;
    MusicDAO &m_musicDAO;
    PlaylistMusicDAO &m_playlistMusicDAO;
    int m_dirIndex;
    int m_playlistIndex;
};
