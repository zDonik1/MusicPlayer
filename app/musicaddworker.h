#pragma once

#include <QObject>

class DirModel;
class PlaylistModel;
class MusicDAO;
class PlaylistMusicDAO;

class MusicAddWorker : public QObject
{
    Q_OBJECT

public:
    MusicAddWorker(DirModel &dirModel, PlaylistModel &playlistModel,
                   MusicDAO &musicDAO, PlaylistMusicDAO &playlistMusicDAO,
                   int dirIndex, int playlistIndex, QObject *parent = nullptr);

public slots:
    void process();

signals:
    void finished();

private:
     DirModel &m_dirModel;
     PlaylistModel &m_playlistModel;
     MusicDAO &m_musicDAO;
     PlaylistMusicDAO &m_playlistMusicDAO;
     int m_dirIndex;
     int m_playlistIndex;
};
