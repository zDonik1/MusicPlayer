#include "rootdirmodel.h"

RootDirModel::RootDirModel()
{

}

const Directory &RootDirModel::getRootDir(const QModelIndex &index) const
{
    return m_rootDirs.at(index.row());
}

void RootDirModel::setRootDirs(const std::vector<Directory> &dirs)
{
    beginResetModel();
    m_rootDirs = std::move(dirs);
    endResetModel();
}

void RootDirModel::addRootDir(const Directory &dir)
{
    beginInsertRows(QModelIndex(), m_rootDirs.size(), m_rootDirs.size());
    m_rootDirs.emplace_back(dir);
    endInsertRows();
}

int RootDirModel::deleteRootDir(const QModelIndex &index)
{
    int id = getRootDir(index).id;
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_rootDirs.erase(m_rootDirs.begin() + index.row());
    endRemoveRows();
    return id;
}

int RootDirModel::rowCount(const QModelIndex &parent) const
{
    return m_rootDirs.size();
}

QVariant RootDirModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Roles::PathRole:
        return m_rootDirs.at(index.row()).dir.path();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> RootDirModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::PathRole] = "r_path";
    return roles;
}
