#pragma once

#include "abstractdao.h"
#include "music.h"

class PlaylistMusicDAO : public AbstractDAO
{
    Q_OBJECT

public:
    PlaylistMusicDAO(QSqlDatabase &database);
    ~PlaylistMusicDAO() override;

    void init() override;
    static QString tableName();

    int getMusicCount(int playlistId) const;
    std::vector<Music> getMusicForPlaylist(int playlistId) const;

    void addMusicToPlaylist(int musicId, int playlistId);
    void deleteMusicFromPlaylist(int musicId, int playlistId);

signals:
    void musicAddedToPlaylist(int playlistId);
    void musicRemovedFromPlaylist(int playlistId);
};
