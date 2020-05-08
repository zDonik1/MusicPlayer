#include "dirmodel.h"

#include <QDebug>
#include <QDir>

DirModel::DirModel()
{
}

void DirModel::setupModel(const std::map<QString, QStringList> &map)
{
    m_entryInfo.clear();
    beginResetModel();
    for (const auto &pair : map) {
        QVariantHash dirHash;
        dirHash["filename"] = pair.first;
        dirHash["isDir"] = true;
        dirHash["isOpen"] = true;
        m_entryInfo.emplace_back(dirHash);
        for (const auto &filename : pair.second) {
            QVariantHash fileHash;
            fileHash["filename"] = filename;
            fileHash["isDir"] = false;
            fileHash["isOpen"] = true;
            m_entryInfo.emplace_back(fileHash);
        }
    }
    endResetModel();
}

void DirModel::toggleDir(int index)
{
    qDebug() << "~~~ toggling dir at index" << index;
    m_entryInfo.at(index)["isOpen"] = !m_entryInfo.at(index)["isOpen"].toBool();
    int i;
    for (i = index + 1; ; ++i) {
        if (m_entryInfo.at(i)["isDir"].toBool()
                || i >= static_cast<int>(m_entryInfo.size()))
        {
            break;
        }

        auto &isOpen = m_entryInfo.at(i)["isOpen"];
        isOpen = !isOpen.toBool();
    }
    dataChanged(createIndex(index, 0), createIndex(i, 0), { Roles::IsOpen });
}

int DirModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_entryInfo.size();
}

QVariant DirModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Roles::IsDirRole:
        return m_entryInfo.at(index.row())["isDir"];

    case Roles::IsOpen:
        return m_entryInfo.at(index.row())["isOpen"];

    case Roles::NameRole:
    case Qt::DisplayRole: {
        QString filename = m_entryInfo.at(index.row())["filename"].toString();
        if (m_entryInfo.at(index.row())["isDir"].toBool())
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
