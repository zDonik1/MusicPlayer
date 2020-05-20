#pragma once

#include <QSqlDatabase>
#include <map>
#include <memory>

#include "abstractdao.h"

class DirDAO;
class PlaylistDAO;
class MusicDAO;

class DatabaseManager
{
public:
    DatabaseManager();

    QSqlDatabase &getDatabase();
    const DirDAO *getDirDAO() const;
    PlaylistDAO *getPlaylistDAO();
    MusicDAO *getMusicDAO();

private:
    QSqlDatabase m_database;
    std::map<QString, std::unique_ptr<AbstractDAO>> m_daos;

    const QString DB_FILENAME = "music.db";
};
