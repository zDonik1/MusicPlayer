#include "musicaddworker.h"

#include <QAndroidBinder>
#include <QAndroidParcel>

#include <messagetype.h>
#include "appstate.h"
#include "metadatascanner.h"
#include "dirmodel.h"
#include "playlistmodel.h"
#include "musicmodel.h"
#include "musicdao.h"
#include "playlistmusicdao.h"

MusicAddWorker::MusicAddWorker(AppState &appState,
                               MetaDataScanner &metaDataScanner,
                               QAndroidBinder &clientBinder,
                               DirModel &dirModel, PlaylistModel &playlistModel,
                               MusicModel &musicModel, MusicDAO &musicDAO,
                               PlaylistMusicDAO &playlistMusicDAO, int dirIndex,
                               int playlistIndex, QObject *parent)
    : QObject(parent)
    , m_appState(appState)
    , m_metaDataScanner(metaDataScanner)
    , m_clientBinder(clientBinder)
    , m_dirModel(dirModel)
    , m_playlistModel(playlistModel)
    , m_musicModel(musicModel)
    , m_musicDAO(musicDAO)
    , m_playlistMusicDAO(playlistMusicDAO)
    , m_dirIndex(dirIndex)
    , m_playlistIndex(playlistIndex)
{
}

void MusicAddWorker::process()
{
    auto files = m_dirModel.getFilesInDir(m_dirModel.index(m_dirIndex));
    int playlistId = m_playlistModel.getPlaylist(m_playlistModel
                                                  .index(m_playlistIndex)).id;
    std::vector<Music> music;
    for (auto &file : files) {
        int musicId = m_musicDAO.createMusic(file);
        if (musicId == -1)
            return;

        if (m_playlistMusicDAO.addMusicToPlaylist(musicId, playlistId)) {
            music.emplace_back(Music{ musicId, std::move(file) });
        }
    }

    int playlistIndex = m_playlistModel.getPlaylistIndex(playlistId);
    if (playlistIndex == m_appState.getCurrentPlaylistIndex()) {
        std::vector<Music *> musicPtrs;
        QVariantList musicVarList;
        for (auto &m : music) {
            musicPtrs.push_back(&m);
            musicVarList.push_back(m.path);
            m_musicModel.addMusic(std::move(m));
        }
        QAndroidParcel data;
        data.writeVariant(musicVarList);
        m_clientBinder.transact(MessageType::MUSIC_ADDED, data);

        m_metaDataScanner.getMetaData(musicPtrs);
    }

    emit finished();
}
