#include "dirdao.h"

#include <QSqlQuery>
#include <QVariant>
#include <QStandardPaths>
#include <QDebug>

DirDAO::DirDAO(QSqlDatabase &database)
    : AbstractDAO(database)
{
}

DirDAO::~DirDAO()
{
}

void DirDAO::init()
{
    if (!m_database.tables().contains(tableName())) {
        qDebug() << "Table is being created";
        QSqlQuery query(m_database);
        query.exec("create table " + tableName() + " ("
                   "id integer primary key autoincrement,"
                   "path text"
                   ")");

        query.prepare("insert into " + tableName() + " (path) values ('"
                      + QStandardPaths::writableLocation(
                          QStandardPaths::MusicLocation) + "')");
    }

    QSqlQuery dirs = m_database.exec("select path from " + tableName());
    while (dirs.next()) {
        m_rootDirs.emplace_back(QDir(dirs.value(0).toString()));
    }
}

QString DirDAO::tableName() const
{
    return "rootdir";
}

const std::vector<QDir> &DirDAO::getAll() const
{
    return m_rootDirs;
}
