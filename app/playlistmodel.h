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
    };

public:
    PlaylistModel();

    const Playlist &getPlaylist(const QModelIndex &index) const;

    void setPlaylists(const std::vector<Playlist> &playlists);
    void addPlaylist(const Playlist &playlist);
    int editPlaylist(const QModelIndex &index, const QString &name);
    int deletePlaylist(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<Playlist> m_playlists;
};
