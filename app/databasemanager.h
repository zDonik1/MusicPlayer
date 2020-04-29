#pragma once

#include <QSqlDatabase>
#include <map>
#include <memory>

#include "abstractdao.h"

class DirDAO;

class DatabaseManager
{
public:
    DatabaseManager();

    QSqlDatabase &getDatabase();
    const DirDAO *getDirDAO();

private:
    QSqlDatabase m_database;
    std::map<QString, std::unique_ptr<AbstractDAO>> m_daos;

    const QString DB_FILENAME = "music.db";
};
