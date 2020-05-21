#include "dirmodel.h"

#include <QDebug>
#include <QDir>

DirModel::DirModel()
{
}

void DirModel::setupModel(const std::map<QString, QStringList> &map)
{
    m_dirEntryInfo.clear();
    beginResetModel();
    for (const auto &pair : map) {
        QVariantHash dirHash;
        dirHash["filename"] = pair.first;
        dirHash["isDir"] = true;
        dirHash["isOpen"] = true;
        m_dirEntryInfo.emplace_back(dirHash);
        for (const auto &filename : pair.second) {
            QVariantHash fileHash;
            fileHash["filename"] = filename;
            fileHash["isDir"] = false;
            fileHash["isOpen"] = true;
            m_dirEntryInfo.emplace_back(fileHash);
        }
    }
    endResetModel();
}

void DirModel::toggleDir(int index)
{
    qDebug() << "~~~ toggling dir at index" << index;
    m_dirEntryInfo.at(index)["isOpen"] = !m_dirEntryInfo.at(index)["isOpen"].toBool();
    int i;
    for (i = index + 1; ; ++i) {
        if (m_dirEntryInfo.at(i)["isDir"].toBool()
                || i >= static_cast<int>(m_dirEntryInfo.size()))
        {
            break;
        }

        auto &isOpen = m_dirEntryInfo.at(i)["isOpen"];
        isOpen = !isOpen.toBool();
    }
    dataChanged(createIndex(index, 0), createIndex(i, 0), { Roles::IsOpen });
}

int DirModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_dirEntryInfo.size();
}

QVariant DirModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Roles::IsDirRole:
        return m_dirEntryInfo.at(index.row())["isDir"];

    case Roles::IsOpen:
        return m_dirEntryInfo.at(index.row())["isOpen"];

    case Roles::NameRole:
    case Qt::DisplayRole: {
        QString filename = m_dirEntryInfo.at(index.row())["filename"].toString();
        if (m_dirEntryInfo.at(index.row())["isDir"].toBool())
            return QDir(filename).dirName();
        else
            return filename;
        break;
    }

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DirModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::IsDirRole] = "r_isDir";
    roles[Roles::IsOpen] = "r_isOpen";
    roles[Roles::NameRole] = "r_name";
    return roles;
}
