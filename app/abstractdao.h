#pragma once

#include <QObject>
#include <QString>
#include <QSqlDatabase>

#include <exception>

class invalid_input : public std::bad_exception
{
};

class AbstractDAO : public QObject
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

protected:
    QSqlDatabase &m_database;
};
