#pragma once

#include <vector>

#include "abstractdao.h"
#include "playlist.h"

class PlaylistDAO : public AbstractDAO
{
public:
    PlaylistDAO(QSqlDatabase &database);
    ~PlaylistDAO() override;

    void init() override;
    QString tableName() const override;

    const std::vector<Playlist> &getAll();
    const Playlist &getPlaylist(int id);
    void createPlaylist(const Playlist &playlist);
    void updatePlaylist(const Playlist &playlist);
    void deletePlaylist(int id);

private:
    void saveToCache(QSqlQuery &result);

private:
    std::vector<Playlist> m_playlists;
    bool m_dirty = true;
};
