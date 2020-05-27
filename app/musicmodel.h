#pragma once

#include <QAbstractListModel>
#include <vector>

#include "music.h"

class MusicModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int currentPlaylist READ getCurrentPlaylist
               NOTIFY currentPlaylistChanged)

public:
    enum Roles
    {
        TitleRole = Qt::UserRole
    };

public:
    MusicModel();

    int getCurrentPlaylist();

    void setCurrentPlaylist(int playlistId);
    void setMusic(const std::vector<Music> &music);
    void addMusic(const Music &music);
    void deleteMusic(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void currentPlaylistChanged();

private:
    std::vector<Music> m_music;
    int m_currentPlaylist = -1;
};
