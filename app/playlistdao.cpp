#include "playlistdao.h"

#include <QSqlQuery>
#include <QStringList>
#include <QDebug>

PlaylistDAO::PlaylistDAO(QSqlDatabase &database)
    : AbstractDAO(database)
{
}

PlaylistDAO::~PlaylistDAO()
{
}

void PlaylistDAO::init()
{
    if (!m_database.tables().contains(tableName())) {
        qDebug() << "Table is being created";
        QSqlQuery query(m_database);
        query.exec("create table " + tableName() + " ("
                                                   "id integer primary key autoincrement,"
                                                   "name text"
                                                   ")");
    }

    QSqlQuery playlists = m_database.exec("select * from " + tableName());
    saveToCache(playlists);
}

QString PlaylistDAO::tableName() const
{
    return "playlist";
}

const std::vector<Playlist> &PlaylistDAO::getAll()
{
    if (m_dirty) {
        QSqlQuery playlists = m_database.exec("select * from " + tableName());
        saveToCache(playlists);
        m_dirty = false;
    }
    return m_playlists;
}

const Playlist &PlaylistDAO::getPlaylist(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::getPlaylist: invalid id");

    // reset cache if dirty
    getAll();

    auto itr = std::find_if(m_playlists.cbegin(), m_playlists.cend(),
                         [&id] (const Playlist &playlist)
    {
        return playlist.id == id;
    });

    if (itr == m_playlists.end())
        throw std::out_of_range("PlaylistDAO::getPlaylist: invalid id");

    return *itr;
}

void PlaylistDAO::createPlaylist(const Playlist &playlist)
{
    if (playlist.id < 0 || playlist.name == "")
        throw invalid_input();

    QSqlQuery query(m_database);
    query.exec("insert into " + tableName() + " (id, name) values("
               + playlist.id + ", " + playlist.name + ")");
    m_playlists.emplace_back(playlist);
    // not setting dirty to true since its easy to simply add it to cache
}

void PlaylistDAO::updatePlaylist(const Playlist &playlist)
{
    if (playlist.id < 0 || playlist.name == "")
        throw invalid_input();

    QSqlQuery query(m_database);
    query.exec("update " + tableName() + " set name = " + playlist.name
               + " where id = " + playlist.id);
    m_dirty = true;
}

void PlaylistDAO::deletePlaylist(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::deletePlaylist: invalid id");

    QSqlQuery query(m_database);
    query.exec("delete from " + tableName() + " where id = " + id);
    m_dirty = true;
}

void PlaylistDAO::saveToCache(QSqlQuery &result)
{
    while (result.next()) {
        m_playlists.emplace_back(Playlist{ result.value(0).toInt(),
                                           result.value(1).toString() });
    }
}
