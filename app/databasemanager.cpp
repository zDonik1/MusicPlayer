#include "databasemanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDir>

#include "dirdao.h"

using namespace std;

DatabaseManager::DatabaseManager()
    : m_database(QSqlDatabase::addDatabase("QSQLITE"))
{
    // TODO: debug this whole function
    QFile assetDbFile(":/" + DB_FILENAME);
    QString destinationDbFile
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
              .append("/" + DB_FILENAME);
    qDebug() << "assetdbfile:" << assetDbFile.fileName();
    qDebug() << "destionationDbFile:" << destinationDbFile;

    if (!QFile::exists(destinationDbFile)) {
        qDebug() << "Db file doesn't exist";
        if (!assetDbFile.copy(destinationDbFile))
            qDebug() << "Couldn't copy db file";
        QFile::setPermissions(destinationDbFile, QFile::WriteOwner
                                                     | QFile::ReadOwner);
    }

    QString pathToDb = QStandardPaths::standardLocations(
                QStandardPaths::AppDataLocation).first();
    QDir dir;
    dir.mkpath(pathToDb);

    m_database.setDatabaseName(pathToDb + "/" + DB_FILENAME);
    qDebug() << "database name: " << m_database.databaseName();
    if (!m_database.open()) {
        qDebug() << "Could not open database";
    }

    m_daos["dir"] = make_unique<DirDAO>(m_database);

    for (const auto &pair : m_daos) {
        pair.second->init();
    }
}

QSqlDatabase &DatabaseManager::getDatabase()
{
    return m_database;
}

const DirDAO *DatabaseManager::getDirDAO()
{
    return dynamic_cast<DirDAO *>(m_daos["dir"].get());
}
