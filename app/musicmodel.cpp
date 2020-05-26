#include "musicmodel.h"

#include <QUrl>

MusicModel::MusicModel()
{
}

void MusicModel::setMusic(const std::vector<Music> &music)
{
    beginResetModel();
    m_music = std::move(music);
    endResetModel();
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
        return QUrl(m_music.at(index.row()).path).fileName();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> MusicModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::TitleRole] = "r_title";
    return roles;
}
