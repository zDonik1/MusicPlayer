#include "playlistmodel.h"

#include <QDebug>

PlaylistModel::PlaylistModel()
{
}

const Playlist &PlaylistModel::getPlaylist(const QModelIndex &index) const
{
    return m_playlists.at(index.row());
}

void PlaylistModel::setPlaylists(const std::vector<Playlist> &playlists)
{
    beginResetModel();
    m_playlists = std::move(playlists);
    endResetModel();
    qDebug() << "playlist:";
    for (const auto &playlist : m_playlists) {
        qDebug() << playlist.name;
    }
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

    qDebug() << "size:" << m_playlists.size();
    qDebug() << "index:" << index.row() << m_playlists.at(index.row()).name;
    switch (role) {
    case Qt::DisplayRole:
    case Roles::NameRole:
        return m_playlists.at(index.row()).name;
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::NameRole] = "r_name";
    return roles;
}
