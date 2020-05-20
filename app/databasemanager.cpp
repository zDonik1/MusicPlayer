#include "databasemanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDir>

#include "dirdao.h"
#include "playlistdao.h"
#include "musicdao.h"

using namespace std;

DatabaseManager::DatabaseManager()
    : m_database(QSqlDatabase::addDatabase("QSQLITE"))
{
    QFile assetDbFile(":/" + DB_FILENAME);
    QString destinationDbFile
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
              .append("/" + DB_FILENAME);

    if (!QFile::exists(destinationDbFile)) {
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

    // setting up daos
    m_daos["dir"] = make_unique<DirDAO>(m_database);
    m_daos["playlist"] = make_unique<PlaylistDAO>(m_database);
    m_daos["music"] = make_unique<MusicDAO>(m_database);

    for (const auto &pair : m_daos) {
        pair.second->init();
    }
}

QSqlDatabase &DatabaseManager::getDatabase()
{
    return m_database;
}

const DirDAO *DatabaseManager::getDirDAO() const
{
    return dynamic_cast<DirDAO *>(m_daos.at("dir").get());
}

PlaylistDAO *DatabaseManager::getPlaylistDAO()
{
    return dynamic_cast<PlaylistDAO *>(m_daos.at("playlist").get());
}

MusicDAO *DatabaseManager::getMusicDAO()
{
    return dynamic_cast<MusicDAO *>(m_daos.at("music").get());
}
