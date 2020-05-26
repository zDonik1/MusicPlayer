#include "musicaddworker.h"

#include "dirmodel.h"
#include "playlistmodel.h"
#include "musicdao.h"
#include "playlistmusicdao.h"

MusicAddWorker::MusicAddWorker(DirModel &dirModel, PlaylistModel &playlistModel,
                               MusicDAO &musicDAO,
                               PlaylistMusicDAO &playlistMusicDAO, int dirIndex,
                               int playlistIndex, QObject *parent)
    : QObject(parent)
    , m_dirModel(dirModel)
    , m_playlistModel(playlistModel)
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
    for (auto &file : *files) {
        int musicId = m_musicDAO.createMusic(file);
        if (musicId == -1)
            return;

        m_playlistMusicDAO.addMusicToPlaylist(musicId, playlistId);
    }

    emit finished();
}
