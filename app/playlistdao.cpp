#include "playlistdao.h"

#include <QSqlQuery>
#include <QStringList>
#include <QDebug>
#include <QSqlError>
#include <algorithm>

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
                                                   "id integer primary key,"
                                                   "name text"
                                                   ")");
    }
    updateCache();

    int maxId = -1;
    for (const auto &playlist : m_playlists) {
        if (maxId < playlist.id)
            maxId = playlist.id;
    }
    m_idCounter = maxId + 1;
}

QString PlaylistDAO::tableName() const
{
    return "playlist";
}

const std::vector<Playlist> &PlaylistDAO::getAll()
{
    updateCache();
    return m_playlists;
}

const Playlist &PlaylistDAO::getPlaylist(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::getPlaylist: invalid id");

    updateCache();

    auto itr = std::find_if(m_playlists.cbegin(), m_playlists.cend(),
                         [&id] (const Playlist &playlist)
    {
        return playlist.id == id;
    });

    if (itr == m_playlists.end())
        throw std::out_of_range("PlaylistDAO::getPlaylist: invalid id");

    return *itr;
}

int PlaylistDAO::createPlaylist(const QString &name)
{
    if (name == "")
        throw invalid_input();

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral("insert into %1 (id, name) values (%2, '%3')")
            .arg(tableName()).arg(m_idCounter).arg(name);
    query.exec(queryString);
    m_playlists.push_back(Playlist{ m_idCounter, name });
    return m_idCounter++;
}

void PlaylistDAO::updatePlaylist(const Playlist &playlist)
{
    if (playlist.id < 0 || playlist.name == "")
        throw invalid_input();

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral("update %1 set name = '%2' where id = %3")
            .arg(tableName()).arg(playlist.name).arg(playlist.id);
    query.exec(queryString);
    m_dirty = true;
}

void PlaylistDAO::deletePlaylist(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::deletePlaylist: invalid id");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral("delete from %1 where id = %2")
            .arg(tableName()).arg(id);
    query.exec(queryString);
    m_dirty = true;
}

void PlaylistDAO::updateCache()
{
    if (!m_dirty)
        return;

    QSqlQuery result = m_database.exec("select * from " + tableName());
    m_playlists.clear();
    while (result.next()) {
        m_playlists.emplace_back(Playlist{ result.value(0).toInt(),
                                           result.value(1).toString() });
    }
    m_dirty = false;
}
