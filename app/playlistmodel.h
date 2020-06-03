#pragma once

#include <QAbstractListModel>
#include <vector>

#include "playlist.h"

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        NameRole = Qt::UserRole,
        MusicCountRole,
    };

public:
    PlaylistModel();

    const Playlist &getPlaylist(const QModelIndex &index) const;
    int getPlaylistIndex(int id) const;

    void setPlaylists(const std::vector<Playlist> &playlists);
    void addPlaylist(const Playlist &playlist);
    int editPlaylist(const QModelIndex &index, const QString &name);
    int deletePlaylist(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void incrementMusicCount(int playlistId);
    void decrementMusicCount(int playlistId);

private:
    QModelIndex findById(int id);

private:
    std::vector<Playlist> m_playlists;
};
