#include "datamodel.h"

// so many includes in this class, don't you think?

#include <QAndroidParcel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStack>
#include <QUrl>
#include <QThread>
#include <QMediaPlayer>
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
    connect(&m_metaDataScanner, &MetaDataScanner::metaDataReady,
            this, &DataModel::imagesReady);
    connect(&m_metaDataScanner, &MetaDataScanner::metaDataReady,
            this, &DataModel::onMetaDataReady);

    auto playlists = m_databaseManager.getPlaylistDAO()->getAll();
    for (auto &playlist : playlists) {
        playlist.musicCount = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicCount(playlist.id);
    }
    m_playlistModel->setPlaylists(playlists);

    m_rootDirModel->setRootDirs(m_databaseManager.getRootDirDAO()->getAll());

    initializePlayer();
}

const QAndroidBinder &DataModel::getClientBinder() const
{
    return m_clientBinder;
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

bool DataModel::getShuffle() const
{
    return m_shuffle;
}

bool DataModel::getRepeat() const
{
    return m_repeat;
}

qint64 DataModel::getCurrentMusicDuration() const
{
    return m_currentMusicDuration;
}

qint64 DataModel::getCurrentMusicPosition() const
{
    return m_currentMusicPosition;
}

void DataModel::setClientBinder(const QAndroidBinder &clientBinder)
{
    m_clientBinder = clientBinder;
}

void DataModel::setMusicImageProvider(MusicImageProvider *imageProvider)
{
    m_imageProvider = imageProvider;
}

void DataModel::setCurrentMusicPosition(int64_t position)
{
    m_currentMusicPosition = position;
    emit currentMusicPositionChanged();
}

void DataModel::play()
{
    if (m_musicModel->getCurrentMusicIndex() == -1)
        return;

    m_isPlaying = !m_isPlaying;
    emit isPlayingChanged();
    QAndroidParcel data;
    data.writeVariant(m_isPlaying);
    m_clientBinder.transact(MessageType::PLAY, data);
}

void DataModel::next()
{
    QAndroidParcel reply;
    m_clientBinder.transact(MessageType::NEXT, QAndroidParcel(), &reply);
    updateOnMusicChanged(reply.readVariant().toInt());
}

void DataModel::previous()
{
    QAndroidParcel reply;
    m_clientBinder.transact(MessageType::PREVIOUS, QAndroidParcel(), &reply);
    updateOnMusicChanged(reply.readVariant().toInt());
}

void DataModel::seek(double position)
{
    m_currentMusicPosition = m_currentMusicDuration * position;
    QAndroidParcel sendData;
    sendData.writeVariant(static_cast<qint64>(m_currentMusicPosition));
    m_clientBinder.transact(MessageType::SEEK, sendData);
}

void DataModel::shuffle()
{
    m_shuffle = !m_shuffle;
    emit shuffleChanged();
    QAndroidParcel data;
    data.writeVariant(m_shuffle);
    m_clientBinder.transact(MessageType::SHUFFLE, data);
    m_databaseManager.getSettingsDAO()->storeValue("shuffle", m_shuffle);
}

void DataModel::repeat()
{
    m_repeat = !m_repeat;
    emit repeatChanged();
    QAndroidParcel data;
    data.writeVariant(m_repeat);
    m_clientBinder.transact(MessageType::REPEAT, data);
    m_databaseManager.getSettingsDAO()->storeValue("repeat", m_repeat);
}

void DataModel::changeMusic(int index)
{
    QAndroidParcel data;
    data.writeVariant(index);
    m_clientBinder.transact(MessageType::MUSIC_CHANGED, data);

    updateOnMusicChanged(index);
    m_isPlaying = true;
    emit isPlayingChanged();
}

void DataModel::selectPlaylist(int index)
{
    auto playlist = m_playlistModel->getPlaylist(m_playlistModel->index(index));
    auto music = m_databaseManager.getPlaylistMusicDAO()
            ->getMusicForPlaylist(playlist.id);
    m_musicModel->setMusic(playlist.id, music);
    m_metaDataScanner.getMetaData(&m_musicModel->getAllMusic());

    QVariantList musicVarList;
    for (auto &m : music)
        musicVarList.push_back(std::move(m.path));
    QAndroidParcel data;
    data.writeVariant(musicVarList);
    m_clientBinder.transact(MessageType::LOAD_PLAYLIST, data);

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

        QVariantList musicVarList;
        for (auto &m : music)
            musicVarList.push_back(m.path);
        QAndroidParcel data;
        data.writeVariant(musicVarList);
        m_clientBinder.transact(MessageType::MUSIC_ADDED, data);
    }
}

void DataModel::addMusicToPlaylist(int musicIndex, int playlistIndex)
{
    auto file = m_dirModel->getFile(m_dirModel->index(musicIndex));
    int playlistId = m_playlistModel->getPlaylist(m_playlistModel
                                                  ->index(playlistIndex)).id;
    int musicId = m_databaseManager.getMusicDAO()->createMusic(file);
    bool success = m_databaseManager.getPlaylistMusicDAO()
            ->addMusicToPlaylist(musicId, playlistId);
    if (!success)
        return;

    if (playlistId == m_musicModel->getCurrentPlaylist()) {
        m_musicModel->addMusic(Music{ musicId, file });

        QVariantList musicVarList;
        musicVarList.push_back(file);
        QAndroidParcel data;
        data.writeVariant(musicVarList);
        m_clientBinder.transact(MessageType::MUSIC_ADDED, data);
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

void DataModel::onMetaDataReady()
{
    m_musicModel->updateModelMetaData();
    m_currentMusicDuration = m_musicModel->getCurrentMusic().metaData.duration;
    emit currentMusicDurationChanged();

    auto &music = m_musicModel->getAllMusic();
    std::vector<QPixmap> pixmaps;
    for (auto &m : music) {
        pixmaps.push_back(QPixmap::fromImage(m.metaData.image));
    }
    m_imageProvider->setMusicImages(std::move(pixmaps));
}

void DataModel::initializePlayer()
{
    QVariant currentPlaylistVariant = m_databaseManager.getSettingsDAO()
            ->getValue("current_playlist");
    if (!currentPlaylistVariant.isValid()) {
        m_musicModel->setMusic(-1, {});
    }
    else {
        auto playlist = m_databaseManager.getPlaylistDAO()
                ->getPlaylist(currentPlaylistVariant.toInt());
        auto music = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicForPlaylist(playlist.id);
        m_musicModel->setMusic(playlist.id, music);
        m_metaDataScanner.getMetaData(&m_musicModel->getAllMusic());

        m_currentPlaylistName = playlist.name;
        emit currentPlaylistNameChanged();

        QVariant currentMusicVariant = m_databaseManager.getSettingsDAO()
                ->getValue("current_music_index");
        if (currentMusicVariant.isValid())
            updateOnMusicChanged(currentMusicVariant.toInt());
        else
            updateOnMusicChanged(-1);
    }

    m_shuffle = m_databaseManager.getSettingsDAO()
            ->getValue("shuffle").toBool();
    emit shuffleChanged();
    m_repeat = m_databaseManager.getSettingsDAO()->getValue("repeat").toBool();
    emit repeatChanged();
}

void DataModel::updateOnMusicChanged(int index)
{
    m_musicModel->setCurrentMusicIndex(index);
    m_currentMusicDuration = m_musicModel->getCurrentMusic().metaData.duration;
    m_currentMusicPosition = 0;
    emit currentMusicDurationChanged();
    emit currentMusicPositionChanged();
    m_databaseManager.getSettingsDAO()->storeValue("current_music_index",
                                                   index);
}
