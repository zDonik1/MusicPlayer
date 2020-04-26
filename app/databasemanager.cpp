#include "databasemanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDir>

#include "dirdao.h"

using namespace std;

DatabaseManager::DatabaseManager()
{
    m_database.addDatabase("QSQLITE");
    m_database.setDatabaseName(QStandardPaths::writableLocation(
                                   QStandardPaths::AppDataLocation) + "/");

    QString pathToDb = QStandardPaths::standardLocations(
                QStandardPaths::AppDataLocation).first();
    QDir dir;
    dir.mkpath(pathToDb);

    m_database.setDatabaseName(pathToDb + "/music");
    if (!m_database.open()) {
        qDebug() << "Could not open database";
    }

    m_daos["dir"] = make_unique<DirDAO>(m_database);

    for (const auto &pair : m_daos) {
        pair.second->init();
    }

    // TODO: copy db file to appdatalocation for android and ios
}

QSqlDatabase &DatabaseManager::getDatabase()
{
    return m_database;
}

const DirDAO *DatabaseManager::getDirDAO()
{
    return dynamic_cast<DirDAO *>(m_daos["dir"].get());
}
