#pragma once

#include <QAbstractListModel>
#include <vector>

#include "music.h"

class MusicModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int currentPlaylist READ getCurrentPlaylist
               NOTIFY currentPlaylistChanged)
    Q_PROPERTY(int currentMusic READ getCurrentMusicIndex
               NOTIFY currentMusicIndexChanged)

public:
    enum Roles
    {
        TitleRole = Qt::UserRole,
        DurationRole,
        IsDefaultImageRole
    };

public:
    MusicModel();

    int getCurrentPlaylist() const;
    int getCurrentMusicIndex() const;
    Music getCurrentMusic() const;
    Music getMusic(const QModelIndex &index) const;
    const std::vector<Music> &getAllMusic() const;
    std::vector<Music> &getAllMusic();

    void setCurrentMusicIndex(int index);
    void setMusic(int playlistId, const std::vector<Music> &music);
    void addMusic(const Music &music);
    void deleteMusic(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void updateModelMetaData();

signals:
    void currentPlaylistChanged();
    void currentMusicIndexChanged();

private:
    std::vector<Music> m_music;
    int m_currentPlaylist = -1;
    int m_currentMusicIndex = -1;
};
