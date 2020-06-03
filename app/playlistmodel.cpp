#include "playlistmodel.h"

#include <QDebug>

PlaylistModel::PlaylistModel()
{
}

const Playlist &PlaylistModel::getPlaylist(const QModelIndex &index) const
{
    return m_playlists.at(index.row());
}

int PlaylistModel::getPlaylistIndex(int id) const
{
    if (id < 0 || m_playlists.size() == 0)
        return -1;

    int i;
    for (i = 0; i < static_cast<int>(m_playlists.size()); ++i) {
        if (m_playlists.at(i).id == id)
            break;
    }

    return i;
}

void PlaylistModel::setPlaylists(const std::vector<Playlist> &playlists)
{
    beginResetModel();
    m_playlists = std::move(playlists);
    endResetModel();
}

void PlaylistModel::addPlaylist(const Playlist &playlist)
{
    beginInsertRows(QModelIndex(), m_playlists.size(), m_playlists.size());
    m_playlists.emplace_back(playlist);
    endInsertRows();
}

int PlaylistModel::editPlaylist(const QModelIndex &index, const QString &name)
{
    auto &playlist = m_playlists.at(index.row());
    playlist.name = name;
    dataChanged(index, index, { Roles::NameRole });
    return playlist.id;
}

int PlaylistModel::deletePlaylist(const QModelIndex &index)
{
    int id = getPlaylist(index).id;
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_playlists.erase(m_playlists.begin() + index.row());
    endRemoveRows();
    return id;
}

int PlaylistModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_playlists.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Roles::MusicCountRole:
        return m_playlists.at(index.row()).musicCount;

    case Qt::DisplayRole:
    case Roles::NameRole:
        return m_playlists.at(index.row()).name;
    }

    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::NameRole] = "r_name";
    roles[Roles::MusicCountRole] = "r_musicCount";
    return roles;
}

void PlaylistModel::incrementMusicCount(int playlistId)
{
    auto index = findById(playlistId);
    if (!index.isValid())
        return;

    ++m_playlists.at(index.row()).musicCount;
    dataChanged(index, index, { Roles::MusicCountRole });
}

void PlaylistModel::decrementMusicCount(int playlistId)
{
    auto index = findById(playlistId);
    if (!index.isValid())
        return;

    --m_playlists.at(index.row()).musicCount;
    dataChanged(index, index, { Roles::MusicCountRole });
}

QModelIndex PlaylistModel::findById(int id)
{
    for (int i = 0; i < static_cast<int>(m_playlists.size()); ++i) {
        if (m_playlists.at(i).id == id) {
            return createIndex(i, 0);
        }
    }

    return QModelIndex();
}
