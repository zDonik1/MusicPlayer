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
#include <QTimer>
#include <QDebug>

#include <messagetype.h>

#include "databasemanager.h"
#include "appstate.h"
#include "musicimageprovider.h"
#include "rootdirdao.h"
#include "playlistdao.h"
#include "musicdao.h"
#include "playlistmusicdao.h"
#include "settingsdao.h"
#include "musicaddworker.h"

DataModel::DataModel(DatabaseManager &databaseManager, AppState &appState,
                     QObject *parent)
    : QObject(parent)
    , m_databaseManager(databaseManager)
    , m_appState(appState)
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

    connect(m_musicModel.get(), &MusicModel::allMusicChanged,
            this, &DataModel::onAllMusicChanged);

    auto playlists = m_databaseManager.getPlaylistDAO()->getAll();
    for (auto &playlist : playlists) {
        playlist.musicCount = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicCount(playlist.id);
    }
    m_playlistModel->setPlaylists(playlists);

    m_rootDirModel->setRootDirs(m_databaseManager.getRootDirDAO()->getAll());

    m_appState.load();
    if (m_appState.getCurrentPlaylistIndex() == -1)
        m_musicModel->setMusic(-1, {});
    else {
        auto playlist = m_databaseManager.getPlaylistDAO()
                ->getPlaylist(m_appState.getCurrentPlaylistIndex());
        auto music = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicForPlaylist(playlist.id);
        m_musicModel->setMusic(playlist.id, music);

        fetchMetaDataForAllMusic();
    }
}

DataModel::~DataModel()
{
    m_appState.save();
}

const QAndroidBinder &DataModel::getClientBinder() const
{
    return m_clientBinder;
}

AppState &DataModel::getAppState() const
{
    return m_appState;
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

void DataModel::setClientBinder(const QAndroidBinder &clientBinder)
{
    m_clientBinder = clientBinder;
}

void DataModel::setMusicImageProvider(MusicImageProvider *imageProvider)
{
    m_imageProvider = imageProvider;
}

void DataModel::play()
{
    if (m_appState.getCurrentMusicIndex() == -1)
        return;

    bool isPlaying = !m_appState.isPlaying();
    m_appState.setIsPlaying(isPlaying);
    QAndroidParcel data;
    data.writeVariant(isPlaying);
    m_clientBinder.transact(MessageType::PLAY, data);
}

void DataModel::next()
{
    QAndroidParcel reply;
    m_clientBinder.transact(MessageType::NEXT, QAndroidParcel(), &reply);
}

void DataModel::previous()
{
    QAndroidParcel reply;
    m_clientBinder.transact(MessageType::PREVIOUS, QAndroidParcel(), &reply);
}

void DataModel::seek(double position)
{
    int64_t currentMusicPosition = m_appState.getCurrentMusicDuration() * position;
    m_appState.setCurrentMusicPosition(currentMusicPosition);
    QAndroidParcel sendData;
    sendData.writeVariant(static_cast<qint64>(currentMusicPosition));
    m_clientBinder.transact(MessageType::SEEK, sendData);
}

void DataModel::shuffle()
{
    bool shuffle = m_appState.getShuffle();
    m_appState.setShuffle(shuffle);
    QAndroidParcel data;
    data.writeVariant(shuffle);
    m_clientBinder.transact(MessageType::SHUFFLE, data);
}

void DataModel::repeat()
{
    bool repeat = m_appState.getRepeat();
    m_appState.setRepeat(repeat);
    QAndroidParcel data;
    data.writeVariant(repeat);
    m_clientBinder.transact(MessageType::REPEAT, data);
}

void DataModel::changeMusic(int index)
{
    if (index == m_appState.getCurrentMusicIndex()) {
        play();
        return;
    }

    QAndroidParcel data;
    data.writeVariant(index);
    m_clientBinder.transact(MessageType::MUSIC_CHANGED, data);
    m_appState.setIsPlaying(true);
}

void DataModel::selectPlaylist(int index)
{
    if (index == m_appState.getCurrentPlaylistIndex())
        return;

    auto playlist = m_playlistModel->getPlaylist(m_playlistModel->index(index));
    auto music = m_databaseManager.getPlaylistMusicDAO()
            ->getMusicForPlaylist(playlist.id);
    m_musicModel->setMusic(playlist.id, music);
    m_appState.setCurrentMusicIndex(-1);

    fetchMetaDataForAllMusic();

    QVariantList musicVarList;
    for (auto &m : music)
        musicVarList.push_back(std::move(m.path));
    QAndroidParcel data;
    data.writeVariant(musicVarList);
    m_clientBinder.transact(MessageType::LOAD_PLAYLIST, data);

    m_appState.setCurrentPlaylistName(playlist.name);
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
    if (id == m_appState.getCurrentPlaylistIndex()) {
        m_musicModel->setMusic(-1, {});
        m_appState.setCurrentPlaylistIndex(-1);
        m_appState.setCurrentMusicIndex(-1);
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
    if (playlistId == m_appState.getCurrentPlaylistIndex()) {
        auto music = m_databaseManager.getPlaylistMusicDAO()
                ->getMusicForPlaylist(playlistId);
        m_musicModel->setMusic(playlistId, music);

        QVariantList musicVarList;
        for (auto &m : music)
            musicVarList.push_back(m.path);
        QAndroidParcel data;
        data.writeVariant(musicVarList);
        m_clientBinder.transact(MessageType::MUSIC_ADDED, data);

        fetchMetaDataForAllMusic();
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

    if (playlistIndex == m_appState.getCurrentPlaylistIndex()) {
        auto &addedMusic = m_musicModel->addMusic(Music{ musicId, file });

        QVariantList musicVarList;
        musicVarList.push_back(file);
        QAndroidParcel data;
        data.writeVariant(musicVarList);
        m_clientBinder.transact(MessageType::MUSIC_ADDED, data);

        m_metaDataScanner.getMetaData(&addedMusic);
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

    m_appState.setCurrentPlaylistIndex(-1);
    m_appState.setCurrentPlaylistName("");
    m_appState.setCurrentMusicIndex(-1);
    m_appState.setCurrentMusicDuration(0);
    m_appState.setCurrentMusicPosition(0);
}

void DataModel::onPlayerPageLoaded()
{
    // emitting singal after a delay since slider doesn't get updated
    QTimer::singleShot(270, [this] ()
    {
        m_appState.setCurrentMusicPosition(
                    m_appState.getCurrentMusicPosition());
    });
}

void DataModel::onMetaDataReady()
{
    m_musicModel->updateModelMetaData();

    if (m_appState.getCurrentMusicIndex() != -1) {
        updateCurrentMusicData();
    }

    auto &music = m_musicModel->getAllMusic();
    std::vector<QPixmap> pixmaps;
    for (auto &m : music) {
        pixmaps.push_back(QPixmap::fromImage(m.metaData.image));
    }
    m_imageProvider->setMusicImages(std::move(pixmaps));
}

void DataModel::onAllMusicChanged(int playlistId)
{
    m_appState.setCurrentPlaylistIndex(
                m_playlistModel->getPlaylistIndex(playlistId));
}

void DataModel::fetchMetaDataForAllMusic()
{
    auto &music = m_musicModel->getAllMusic();
    std::vector<Music *> musicPtrs;
    for (auto &m : music)
        musicPtrs.emplace_back(&m);
    m_metaDataScanner.getMetaData(std::move(musicPtrs));
}

void DataModel::updateCurrentMusicData()
{
    auto index = m_musicModel->index(m_appState.getCurrentMusicIndex());
    QString title = m_musicModel
            ->data(index, MusicModel::TitleRole).toString();
    int64_t duration = m_musicModel
            ->data(index, MusicModel::DurationRole).toLongLong();
    bool isDefaultImage = m_musicModel
            ->data(index, MusicModel::IsDefaultImageRole).toBool();
    m_appState.setCurrentMusicTitle(title);
    m_appState.setCurrentMusicDuration(duration);
    m_appState.setIsCurrentMusicDefaultImage(isDefaultImage);
}
