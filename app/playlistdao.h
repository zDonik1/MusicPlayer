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
    int createPlaylist(const QString &name);
    void updatePlaylist(const Playlist &playlist);
    void deletePlaylist(int id);

private:
    void updateCache();

private:
    std::vector<Playlist> m_playlists;
    bool m_dirty = true;
    int m_idCounter = 0;
};
