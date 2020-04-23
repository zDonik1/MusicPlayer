#include "databasemanager.h"

#include <QDebug>
#include <QStandardPaths>

DatabaseManager::DatabaseManager()
{
    m_database.addDatabase("QSQLITE");
    m_database.setDatabaseName(QStandardPaths::writableLocation(
                                   QStandardPaths::AppDataLocation) + "/");
    if (!m_database.open()) {
        qDebug() << "Could not open database";
    }
}
