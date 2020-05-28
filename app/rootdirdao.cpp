#include "rootdirdao.h"

#include <QSqlQuery>
#include <QVariant>
#include <QStandardPaths>
#include <QDebug>

RootDirDAO::RootDirDAO(QSqlDatabase &database)
    : AbstractDAO(database)
{
}

RootDirDAO::~RootDirDAO()
{
}

void RootDirDAO::init()
{
    if (!m_database.tables().contains(tableName())) {
        qDebug() << "Table is being created";
        QSqlQuery query(m_database);
        query.exec("create table " + tableName() + " ("
                                                   "id integer primary key,"
                                                   "path text"
                                                   ")");

        query.exec("insert into " + tableName() + " (path) values ('"
                   + QStandardPaths::writableLocation(
                       QStandardPaths::MusicLocation) + "')");
    }
    updateCache();

    int maxId = -1;
    for (const auto &dir : m_rootDirs) {
        if (maxId < dir.id)
            maxId = dir.id;
    }
    m_idCounter = maxId + 1;
}

void RootDirDAO::reset()
{
    if (m_database.tables().contains(tableName())) {
        QSqlQuery query(m_database);
        QString queryString = QStringLiteral("drop table %1")
                .arg(tableName());
        query.exec(queryString);
    }
}

QString RootDirDAO::tableName()
{
    return "rootdir";
}

const std::vector<Directory> &RootDirDAO::getAll()
{
    updateCache();
    return m_rootDirs;
}

int RootDirDAO::createDir(const QDir &dir)
{
    QSqlQuery query(m_database);
    QString queryString = QStringLiteral("insert into %1 (path) values ('%2')")
            .arg(tableName()).arg(dir.path());
    query.exec(queryString);
    m_rootDirs.emplace_back(Directory{ m_idCounter, dir });
    return m_idCounter++;
}

void RootDirDAO::deleteDir(int id)
{
    if (id < 0)
        throw std::out_of_range("PlaylistDAO::deleteDir: invalid id");

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral("delete from %1 where id = %2")
            .arg(tableName()).arg(id);
    query.exec(queryString);
    m_dirty = true;
}

void RootDirDAO::updateCache()
{
    if (!m_dirty)
        return;

    QSqlQuery result = m_database.exec("select id, path from " + tableName());
    m_rootDirs.clear();
    while (result.next()) {
        m_rootDirs.emplace_back(Directory{ result.value(0).toInt(),
                                           result.value(1).toString() });
    }
    m_dirty = false;
}
