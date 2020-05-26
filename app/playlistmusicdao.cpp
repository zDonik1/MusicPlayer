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

QString PlaylistMusicDAO::tableName()
{
    return "playlist_music";
}

int PlaylistMusicDAO::getMusicCount(int playlistId)
{
    if (playlistId < 0)
        throw std::out_of_range("PlaylistMusicDAO::addMusicToPlaylist: "
                                "playlistId is out of range");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("select count(*) from %1 where playlist_id = %2")
            .arg(tableName()).arg(playlistId);
    if (!query.exec(queryString)) {
        qDebug() << "Count failed:" << query.lastError().text();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
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
        throw std::out_of_range("PlaylistMusicDAO::addMusicToPlaylist: "
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
