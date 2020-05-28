#include "playlistmusicdao.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "playlistdao.h"
#include "musicdao.h"

PlaylistMusicDAO::PlaylistMusicDAO(QSqlDatabase &database)
    : AbstractDAO(database)
{
}

PlaylistMusicDAO::~PlaylistMusicDAO()
{
}

void PlaylistMusicDAO::init()
{
    if (!m_database.tables().contains(tableName())) {
        QSqlQuery query(m_database);
        QString queryString = QStringLiteral
                ("create table %1 ("
                 "playlist_id integer references %2 on delete cascade, "
                 "music_id integer references %3 on delete cascade, "
                 "unique(music_id, playlist_id))")
                .arg(tableName())
                .arg(PlaylistDAO::tableName())
                .arg(MusicDAO::tableName());
        if (!query.exec(queryString))
            qDebug() << "Creation of table failed:" << query.lastError().text();
    }
}

void PlaylistMusicDAO::reset()
{
    if (m_database.tables().contains(tableName())) {
        QSqlQuery query(m_database);
        QString queryString = QStringLiteral("drop table %1")
                .arg(tableName());
        query.exec(queryString);
    }
}

QString PlaylistMusicDAO::tableName()
{
    return "playlist_music";
}

int PlaylistMusicDAO::getMusicCount(int playlistId) const
{
    if (playlistId < 0)
        throw std::out_of_range("PlaylistMusicDAO::addMusicToPlaylist: "
                                "playlistId is out of range");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("select count(*) from %1 where playlist_id = %2")
            .arg(tableName())
            .arg(playlistId);
    if (!query.exec(queryString)) {
        qDebug() << "Count failed:" << query.lastError().text();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

std::vector<Music> PlaylistMusicDAO::getMusicForPlaylist(int playlistId) const
{
    if (playlistId < 0)
        throw std::out_of_range("PlaylistMusicDAO::addMusicToPlaylist: "
                                "playlistId is out of range");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("select %1.music_id, %2.path from %1 "
             "inner join %2 on %1.music_id = %2.id where playlist_id = %3")
            .arg(tableName())
            .arg(MusicDAO::tableName())
            .arg(playlistId);
    if (!query.exec(queryString)) {
        qDebug() << "Music select failed:" << query.lastError().text();
    }

    std::vector<Music> result;
    while (query.next()) {
        result.emplace_back(Music{ query.value(0).toInt(),
                                   query.value(1).toString() });
    }
    return result;
}

void PlaylistMusicDAO::deletePlaylist(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistMusicDAO::deletePlaylist: "
                                "id is out of range");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("delete from %1 where playlist_id = %2")
            .arg(tableName())
            .arg(id);
    query.exec(queryString);
}

void PlaylistMusicDAO::addMusicToPlaylist(int musicId, int playlistId)
{
    if (musicId < 0 || playlistId < 0)
        throw std::out_of_range("PlaylistMusicDAO::addMusicToPlaylist: "
                                "musicId or playlistId is out of range");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("insert into %1 (music_id, playlist_id) values (%2, %3)")
            .arg(tableName()).arg(musicId).arg(playlistId);
    if (query.exec(queryString))
        emit musicAddedToPlaylist(playlistId);
}

void PlaylistMusicDAO::deleteMusicFromPlaylist(int musicId, int playlistId)
{
    if (musicId < 0 || playlistId < 0)
        throw std::out_of_range("PlaylistMusicDAO::deleteMusicFromPlaylist: "
                                "musicId or playlistId is out of range");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("delete from %1 where playlist_id = %2 and music_id = %3")
            .arg(tableName())
            .arg(playlistId)
            .arg(musicId);
    if (query.exec(queryString))
        emit musicRemovedFromPlaylist(playlistId);
}
