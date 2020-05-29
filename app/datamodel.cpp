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
#include "settingsdao.h"
#include "musicaddworker.h"

DataModel::DataModel(DatabaseManager &databaseManager, QObject *parent)
    : QObject(parent)
    , m_databaseManager(databaseManager)
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

    // setting up rootdirs
    m_rootDirModel->setRootDirs(m_databaseManager.getRootDirDAO()->getAll());

    // setting up music
    QVariant value = m_databaseManager.getSettingsDAO()
            ->getValue("current_playlist");
    if (!value.isValid()) {
        m_musicModel->setMusic(-1, {});
    }
    else {
        auto music = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicForPlaylist(value.toInt());
        m_musicModel->setMusic(value.toInt(), music);
        auto playlist = m_databaseManager.getPlaylistDAO()
                ->getPlaylist(value.toInt());
        m_currentPlaylistName = playlist.name;
        emit currentPlaylistNameChanged();
    }
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

MusicModel *DataModel::getMusicModel()
{
    return m_musicModel.get();
}

const QString &DataModel::getCurrentPlaylistName() const
{
    return m_currentPlaylistName;
}

bool DataModel::isPlaying() const
{
    return m_isPlaying;
}

void DataModel::setClientBinder(const QAndroidBinder &clientBinder)
{
    m_clientBinder = clientBinder;
}

void DataModel::play()
{
    if (m_musicModel->getCurrentMusic() == -1)
        return;

    QAndroidParcel sendData;
    m_clientBinder.transact(MessageType::PLAY, sendData);
    m_isPlaying = !m_isPlaying;
    emit isPlayingChanged();
}

void DataModel::next()
{
    QAndroidParcel sendData;
    m_clientBinder.transact(MessageType::NEXT, sendData);
}

void DataModel::previous()
{
    QAndroidParcel sendData;
    m_clientBinder.transact(MessageType::PREVIOUS, sendData);
}

void DataModel::seek(double position)
{
    QAndroidParcel sendData;
    sendData.writeVariant(position);
    m_clientBinder.transact(MessageType::SEEK, sendData);
}

void DataModel::shuffle()
{
    QAndroidParcel sendData;
    m_clientBinder.transact(MessageType::SHUFFLE, sendData);
}

void DataModel::repeat()
{
    QAndroidParcel sendData;
    m_clientBinder.transact(MessageType::REPEAT, sendData);
}

void DataModel::changeMusic(int index)
{
    auto music = m_musicModel->getMusic(m_musicModel->index(index));
    QAndroidParcel data;
    data.writeVariant(QUrl::fromLocalFile(music.path.toString()));
    m_clientBinder.transact(MessageType::MUSIC_CHANGED, data);

    m_musicModel->setCurrentMusic(music.id);
    m_isPlaying = true;
    emit isPlayingChanged();
}

void DataModel::selectPlaylist(int index)
{
    auto playlist = m_playlistModel->getPlaylist(m_playlistModel->index(index));
    auto music = m_databaseManager.getPlaylistMusicDAO()
            ->getMusicForPlaylist(playlist.id);
    m_musicModel->setMusic(playlist.id, music);
    m_databaseManager.getSettingsDAO()->storeValue("current_playlist",
                                                   playlist.id);
    m_currentPlaylistName = playlist.name;
    emit currentPlaylistNameChanged();
}

void DataModel::addPlaylist(QString name)
{
    int id = m_databaseManager.getPlaylistDAO()->createPlaylist(name);
    m_playlistModel->addPlaylist(Playlist{ id, name, 0 });
}

void DataModel::editPlaylist(int index, QString name)
{
    int id = m_playlistModel->editPlaylist(m_playlistModel->index(index), name);
    m_databaseManager.getPlaylistDAO()->updatePlaylist({ id, name });
}

void DataModel::deletePlaylist(int index)
{
    int id = m_playlistModel->deletePlaylist(m_playlistModel->index(index));
    m_databaseManager.getPlaylistDAO()->deletePlaylist(id);
    m_databaseManager.getPlaylistMusicDAO()->deletePlaylist(id);
    if (id == m_musicModel->getCurrentPlaylist()) {
        m_musicModel->setMusic(-1, {});
        m_databaseManager.getSettingsDAO()->storeValue("current_playlist", -1);
    }
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

    int playlistId = m_playlistModel->getPlaylist(m_playlistModel
                                                  ->index(playlistIndex)).id;
    if (playlistId == m_musicModel->getCurrentPlaylist()) {
        auto music = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicForPlaylist(playlistId);
        m_musicModel->setMusic(playlistId, music);
    }
}

void DataModel::addMusicToPlaylist(int musicIndex, int playlistIndex)
{
    auto file = m_dirModel->getFile(m_dirModel->index(musicIndex));
    int playlistId = m_playlistModel->getPlaylist(m_playlistModel
                                                  ->index(playlistIndex)).id;
    int musicId = m_databaseManager.getMusicDAO()->createMusic(file);
    m_databaseManager.getPlaylistMusicDAO()
            ->addMusicToPlaylist(musicId, playlistId);

    if (playlistId == m_musicModel->getCurrentPlaylist()) {
        m_musicModel->addMusic(Music{ musicId, file });
    }
}

void DataModel::deleteMusicFromMemory(int index)
{
    auto file = m_dirModel->getFile(m_dirModel->index(index));
    QFile::remove(file);
    int id = m_databaseManager.getMusicDAO()->findByPath(file);
    m_databaseManager.getMusicDAO()->deleteMusic(id);
}

void DataModel::addMusicRootDir(const QUrl &path)
{
    int id = m_databaseManager.getRootDirDAO()->createDir(path.path());
    m_rootDirModel->addRootDir({ id, path.path() });
    refreshDirs();
}

void DataModel::deleteMusicRootDir(int index)
{
    int id = m_rootDirModel->deleteRootDir(m_rootDirModel->index(index));
    m_databaseManager.getRootDirDAO()->deleteDir(id);
    refreshDirs();
}

void DataModel::dropTables()
{
    m_databaseManager.resetAllDAOs();
    m_dirModel->setupModel({});
    m_rootDirModel->setRootDirs({});
    m_playlistModel->setPlaylists({});
    m_musicModel->setMusic(-1, {});
}
