#pragma once

#include <QString>
#include <QSqlDatabase>

class AbstractDAO
{
public:
    AbstractDAO(QSqlDatabase &database)
        : m_database(database)
    {
    }

    virtual ~AbstractDAO()
    {
    }

    virtual void init() = 0;
    virtual QString tableName() = 0;

protected:
    QSqlDatabase &m_database;
};
