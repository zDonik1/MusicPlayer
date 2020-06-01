#include "musicmodel.h"

#include <QUrl>
#include <QDebug>

MusicModel::MusicModel()
{
}

int MusicModel::getCurrentPlaylist() const
{
    return m_currentPlaylist;
}

int MusicModel::getCurrentMusic() const
{
    return m_currentMusic;
}

Music MusicModel::getMusic(const QModelIndex &index) const
{
    return m_music.at(index.row());
}

const std::vector<Music> &MusicModel::getAllMusic() const
{
    return m_music;
}

std::vector<Music> &MusicModel::getAllMusic()
{
    return m_music;
}

void MusicModel::setCurrentMusic(int currentMusic)
{
    m_currentMusic = currentMusic;
}

void MusicModel::setMusic(int playlistId, const std::vector<Music> &music)
{
    if (playlistId < -1)
        return;

    beginResetModel();
    m_music = std::move(music);
    endResetModel();

    m_currentPlaylist = playlistId;
    emit currentPlaylistChanged();
}

void MusicModel::addMusic(const Music &music)
{
    beginInsertRows(QModelIndex(), m_music.size(), m_music.size());
    m_music.emplace_back(music);
    endInsertRows();
}

void MusicModel::deleteMusic(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_music.erase(m_music.begin() + index.row());
    endRemoveRows();
}

int MusicModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_music.size();
}

QVariant MusicModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Roles::TitleRole:
        if (m_music.at(index.row()).metaData.title.isEmpty())
            return QUrl(m_music.at(index.row()).path).fileName();
        else
            return m_music.at(index.row()).metaData.title;

    case Roles::DurationRole:
        return m_music.at(index.row()).metaData.duration;

    case Roles::IsDefaultImageRole:
        return m_music.at(index.row()).metaData.image.isNull();
    }

    return QVariant();
}

QHash<int, QByteArray> MusicModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::TitleRole] = "r_title";
    roles[Roles::DurationRole] = "r_duration";
    roles[Roles::IsDefaultImageRole] = "r_isDefaultImage";
    return roles;
}

void MusicModel::updateModelMetaData()
{
    dataChanged(createIndex(0, 0), createIndex(m_music.size() - 1, 0),
    { Roles::TitleRole, Roles::DurationRole, Roles::IsDefaultImageRole });
}
