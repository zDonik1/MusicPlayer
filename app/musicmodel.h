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
        TitleRole = Qt::UserRole
    };

public:
    MusicModel();

    void setMusic(const std::vector<Music> &music);
    void addMusic(const Music &music);
    void deleteMusic(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<Music> m_music;
};
