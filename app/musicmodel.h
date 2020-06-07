#pragma once

#include <QAbstractListModel>
#include <vector>

#include "music.h"

class MusicModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        TitleRole = Qt::UserRole,
        DurationRole,
        IsDefaultImageRole
    };

public:
    MusicModel();

    Music getMusic(const QModelIndex &index) const;
    const std::vector<Music> &getAllMusic() const;
    std::vector<Music> &getAllMusic();

    void setMusic(int playlistId, const std::vector<Music> &music);
    Music &addMusic(const Music &music);
    void deleteMusic(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public:
    QString getCurrentTitle();
    qint64 getCurrentDuration();
    bool isCurrentDefaultImage();

public slots:
    void updateModelMetaData();

signals:
    void allMusicChanged(int playlistId);

private:
    std::vector<Music> m_music;
};
