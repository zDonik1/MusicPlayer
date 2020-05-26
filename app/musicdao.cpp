#include "musicdao.h"

#include <QSqlQuery>
#include <QSqlError>
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
        QSqlQuery query(m_database);
        QString queryString = QStringLiteral
                ("create table %1 (id integer primary key, path text unique)")
                .arg(tableName());
        if (!query.exec(queryString))
            qDebug() << "Creation of table failed:" << query.lastError().text();
    }
    updateCache();

    int maxId = -1;
    for (const auto &music : m_music) {
        if (maxId < music.id)
            maxId = music.id;
    }
    m_idCounter = maxId + 1;
}

QString MusicDAO::tableName()
{
    return "music";
}

const std::vector<Music> &MusicDAO::getAll()
{
    updateCache();
    return m_music;
}

int MusicDAO::findByPath(const QUrl &path) const
{
    return std::find_if(m_music.cbegin(), m_music.cend(),
                        [&path] (const Music &music)
    {
        return music.path == path;
    })->id;
}

int MusicDAO::createMusic(const QUrl &path)
{
    if (!path.isValid())
        throw invalid_input();

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("insert into %1 (id, path) values (%2, '%3')")
            .arg(tableName())
            .arg(m_idCounter)
            .arg(path.url());
    if (!query.exec(queryString)) {
        queryString = QStringLiteral("select id from %1 where path = '%2'")
                .arg(tableName())
                .arg(path.url());
        query.exec(queryString);
        query.next();
        return query.value(0).toInt();
    }

    m_music.emplace_back(Music{ m_idCounter, path });
    return m_idCounter++;
}

void MusicDAO::updateMusic(const Music &music)
{
    if (music.id < 0 || music.path.isEmpty())
        throw invalid_input();

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("update %1 set path = '%2' where id = %3")
            .arg(tableName())
            .arg(music.path.url())
            .arg(music.id);
    query.exec(queryString);
    m_dirty = true;
}

void MusicDAO::deleteMusic(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::deletePlaylist: invalid id");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral("delete from %1 where id = %2")
            .arg(tableName())
            .arg(id);
    query.exec(queryString);
    m_dirty = true;
}

void MusicDAO::updateCache()
{
    if (!m_dirty)
        return;

    QSqlQuery result = m_database.exec("select id, path from " + tableName());
    m_music.clear();
    while (result.next()) {
        m_music.emplace_back(Music{ result.value(0).toInt(),
                                    result.value(1).toString() });
    }
    m_dirty = false;
}
