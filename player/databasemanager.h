#pragma once

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();

private:
    QSqlDatabase m_database;
};
