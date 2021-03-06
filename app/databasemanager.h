#pragma once

#include <QSqlDatabase>
#include <map>
#include <memory>

#include "abstractdao.h"

class RootDirDAO;
class PlaylistDAO;
class MusicDAO;
class PlaylistMusicDAO;
class SettingsDAO;

class DatabaseManager
{
public:
    DatabaseManager();

    QSqlDatabase &getDatabase();
    RootDirDAO *getRootDirDAO();
    PlaylistDAO *getPlaylistDAO();
    MusicDAO *getMusicDAO();
    PlaylistMusicDAO *getPlaylistMusicDAO();
    SettingsDAO *getSettingsDAO();

    void resetAllDAOs();

private:
    QSqlDatabase m_database;
    std::map<QString, std::unique_ptr<AbstractDAO>> m_daos;

    const QString DB_FILENAME = "music.db";
};
