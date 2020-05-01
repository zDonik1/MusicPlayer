#include "dirmodel.h"

#include <QDir>

DirModel::DirModel()
{

}

void DirModel::setDirFilesMap(const std::map<QString, QStringList> &map)
{
    beginResetModel();
    m_dirFilesMap = std::move(map);
    endResetModel();
}

int DirModel::rowCount(const QModelIndex &/*parent*/) const
{
    int sum = 0;
    for (const auto &pair : m_dirFilesMap) {
        sum += pair.second.size();
    }
    return sum;
}

QVariant DirModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int size;
    QString dir;
    for (const auto &pair : m_dirFilesMap) {
        size = pair.second.size();
        if (row < size) {
            dir = pair.first;
            break;
        }
        row -= size;
    }

    switch (role) {
    case Roles::DirRole:
        return QDir(dir).dirName();

    case Roles::FilenameRole:
    case Qt::DisplayRole:
        return m_dirFilesMap.at(dir).at(row);

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DirModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::DirRole] = "dir";
    roles[Roles::FilenameRole] = "filename";
    return roles;
}
