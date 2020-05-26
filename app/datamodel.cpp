#include "datamodel.h"

#include <QAndroidParcel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStack>
#include <QUrl>
#include <QThread>
#include <QDebug>

#include <messagetype.h>

#include "rootdirdao.h"
#include "playlistdao.h"
#include "musicdao.h"
#include "playlistmusicdao.h"
#include "musicaddworker.h"

DataModel::DataModel(DatabaseManager &databaseManager,
                     const QAndroidBinder &binder, QObject *parent)
    : QObject(parent)
    , m_databaseManager(databaseManager)
    , m_binder(binder)
    , m_dirModel(new DirModel)
    , m_rootDirModel(new RootDirModel)
    , m_playlistModel(new PlaylistModel)
    , m_musicModel(new MusicModel)
{
    connect(m_databaseManager.getPlaylistMusicDAO(),
            &PlaylistMusicDAO::musicAddedToPlaylist,
            m_playlistModel.get(), &PlaylistModel::incrementMusicCount);
    connect(m_databaseManager.getPlaylistMusicDAO(),
            &PlaylistMusicDAO::musicRemovedFromPlaylist,
            m_playlistModel.get(), &PlaylistModel::decrementMusicCount);

    // setting up playlists
    auto playlists = m_databaseManager.getPlaylistDAO()->getAll();
    for (auto &playlist : playlists) {
        playlist.musicCount = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicCount(playlist.id);
    }
    m_playlistModel->setPlaylists(playlists);

    m_rootDirModel->setRootDirs(m_databaseManager.getRootDirDAO()->getAll());

    // TODO: setup musicmodel using settingsdao
}

DirModel *DataModel::getDirModel()
{
    return m_dirModel.get();
}

RootDirModel *DataModel::getRootDirModel()
{
    return m_rootDirModel.get();
}

PlaylistModel *DataModel::getPlaylistModel()
{
    return m_playlistModel.get();
}

void DataModel::play()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::PLAY, sendData, &replyData);
}

void DataModel::next()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::NEXT, sendData, &replyData);
}

void DataModel::previous()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::PREVIOUS, sendData, &replyData);
}

void DataModel::seek(double position)
{
    QAndroidParcel sendData, replyData;
    sendData.writeVariant(position);
    m_binder.transact(MessageType::SEEK, sendData, &replyData);
}

void DataModel::shuffle()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::SHUFFLE, sendData, &replyData);
}

void DataModel::repeat()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::REPEAT, sendData, &replyData);
}

void DataModel::musicChanged(int index)
{
    QAndroidParcel sendData, replyData;
    sendData.writeVariant(index);
    m_binder.transact(MessageType::SEEK, sendData, &replyData);
}

void DataModel::playlistAdded(QString name)
{
    int id = m_databaseManager.getPlaylistDAO()->createPlaylist(name);
    m_playlistModel->addPlaylist({ id, name });
}

void DataModel::playlistEdited(int index, QString name)
{
    int id = m_playlistModel->editPlaylist(m_playlistModel->index(index), name);
    m_databaseManager.getPlaylistDAO()->updatePlaylist({ id, name });
}

void DataModel::playlistDeleted(int index)
{
    int id = m_playlistModel->deletePlaylist(m_playlistModel->index(index));
    m_databaseManager.getPlaylistDAO()->deletePlaylist(id);
}

void DataModel::refreshDirs()
{
    QStack<QDir> stack;
    auto rootDirs = m_databaseManager.getRootDirDAO()->getAll();
    for (const auto &dir : rootDirs)
        stack.push_back(dir.dir);

    // Recurssively traversing root dirs
    std::map<QString, QStringList> dirFilesMap;
    while (!stack.empty()) {
        QDir dir = stack.pop();
        QStringList filters;
        filters << "*.mp3" << "*.m4a" << "*.wav";
        QStringList filenames = dir.entryList(filters, QDir::Files);
        if (!filenames.empty()) {
            dirFilesMap.emplace(dir.absolutePath(), filenames);
        }

        QStringList dirNames = dir.entryList(QDir::Dirs
                                             | QDir::NoDotAndDotDot);

        for (auto dirName : dirNames) {
            stack.push_back(QDir(dir.absolutePath() + "/" + dirName));
        }
    }

    m_dirModel->setupModel(dirFilesMap);
}

void DataModel::toggleDir(int index)
{
    m_dirModel->toggleDir(m_dirModel->index(index));
}

void DataModel::addDirToPlaylist(int dirIndex, int playlistIndex)
{
    // big dirs freeze UI so adding music to DB in worker thread
    QThread *thread = new QThread;
    MusicAddWorker *worker = new MusicAddWorker(
                *m_dirModel, *m_playlistModel,
                *m_databaseManager.getMusicDAO(),
                *m_databaseManager.getPlaylistMusicDAO(),
                dirIndex, playlistIndex);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &MusicAddWorker::process);
    connect(worker, &MusicAddWorker::finished, thread, &QThread::quit);
    connect(worker, &MusicAddWorker::finished, worker, &QThread::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void DataModel::addMusicToPlaylist(int musicIndex, int playlistIndex)
{
    auto file = m_dirModel->getFile(m_dirModel->index(musicIndex));
    int playlistId = m_playlistModel->getPlaylist(m_playlistModel
                                                  ->index(playlistIndex)).id;
    int musicId = m_databaseManager.getMusicDAO()->createMusic(file);
    m_databaseManager.getPlaylistMusicDAO()
            ->addMusicToPlaylist(musicId, playlistId);
}

void DataModel::deleteMusicFromMemory(int index)
{
    auto file = m_dirModel->getFile(m_dirModel->index(index));
    QFile::remove(file);
    int id = m_databaseManager.getMusicDAO()->findByPath(file);
    m_databaseManager.getMusicDAO()->deleteMusic(id);
}

void DataModel::musicRootDirAdded(const QUrl &path)
{
    int id = m_databaseManager.getRootDirDAO()->createDir(path.path());
    m_rootDirModel->addRootDir({ id, path.path() });
    refreshDirs();
}

void DataModel::musicRootDirDeleted(int index)
{
    int id = m_rootDirModel->deleteRootDir(m_rootDirModel->index(index));
    m_databaseManager.getRootDirDAO()->deleteDir(id);
    refreshDirs();
}
