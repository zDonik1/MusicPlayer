#include "musicdao.h"

#include <QSqlQuery>
#include <QStringList>
#include <QDebug>

MusicDAO::MusicDAO(QSqlDatabase &database)
    : AbstractDAO(database)
{
}

MusicDAO::~MusicDAO()
{
}

void MusicDAO::init()
{
    if (!m_database.tables().contains(tableName())) {
        qDebug() << "Table is being created";
        QSqlQuery query(m_database);
        query.exec("create table " + tableName() + " ("
                                                   "id integer primary key autoincrement,"
                                                   "path text,"
                                                   "playlist_id integer"
                                                   ")");
    }

    QSqlQuery music = m_database.exec("select * from " + tableName());
    saveToCache(music);
}

QString MusicDAO::tableName() const
{
    return "music";
}

const std::vector<Music> &MusicDAO::getAll()
{
    if (m_dirty) {
        QSqlQuery query = m_database.exec("select * from " + tableName());
        saveToCache(query);
        m_dirty = false;
    }
    return m_music;
}

const Music &MusicDAO::getMusic(int id)
{
    if (id < 0)
        throw std::out_of_range("MusicDAO::getMusic: invalid id");

    // reset cache if dirty
    getAll();

    auto itr = std::find_if(m_music.begin(), m_music.end(),
                         [&id] (const Music &music)
    {
        return music.id == id;
    });

    if (itr == m_music.end())
        throw std::out_of_range("MusicDAO::getMusic: invalid id");

    return *itr;
}

void MusicDAO::createMusic(const Music &music)
{
    if (music.id < 0 || music.path.isEmpty() || music.playlistId < 0)
        throw invalid_input();

    QSqlQuery query(m_database);
    query.exec("insert into " + tableName() + " (id, name, playlist_id) values("
               + music.id + ", " + music.path.url() + ", "
               + music.playlistId + ")");
    m_music.emplace_back(music);
    // not setting dirty to true since its easy to simply add it to cache
}

void MusicDAO::updateMusic(const Music &music)
{
    if (music.id < 0 || music.path.isEmpty() || music.playlistId < 0)
        throw invalid_input();

    QSqlQuery query(m_database);
    query.exec("update " + tableName() + " set path = " + music.path.url()
               + ", playlist_id = " + music.playlistId
               + " where id = " + music.id);
    m_dirty = true;
}

void MusicDAO::deleteMusic(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::deletePlaylist: invalid id");

    QSqlQuery query(m_database);
    query.exec("delete from " + tableName() + " where id = " + id);
    m_dirty = true;
}

void MusicDAO::saveToCache(QSqlQuery &result)
{
    while (result.next()) {
        m_music.emplace_back(Music{ result.value(0).toInt(),
                                    result.value(1).toString(),
                                    result.value(2).toInt() });
    }
}
