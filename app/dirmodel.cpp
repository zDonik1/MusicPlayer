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

void DirModel::toggleDir(const QModelIndex &index)
{
    m_dirEntryInfo.at(index.row())["isOpen"]
            = !m_dirEntryInfo.at(index.row())["isOpen"].toBool();
    int i;
    for (i = index.row() + 1; ; ++i) {
        if (i >= static_cast<int>(m_dirEntryInfo.size())
                || m_dirEntryInfo.at(i)["isDir"].toBool())
        {
            break;
        }

        auto &isOpen = m_dirEntryInfo.at(i)["isOpen"];
        isOpen = !isOpen.toBool();
    }
    dataChanged(index, createIndex(i, 0), { Roles::IsOpen });
}

std::unique_ptr<std::vector<QString>> DirModel::getFilesInDir(
        const QModelIndex &index) const
{
    auto result = std::make_unique<std::vector<QString>>();
    if (!m_dirEntryInfo.at(index.row())["isDir"].toBool())
        return result;

    QString dirpath = m_dirEntryInfo.at(index.row())["filename"].toString();
    for (int i = index.row() + 1; ; ++i) {
        if (i >= static_cast<int>(m_dirEntryInfo.size())
                || m_dirEntryInfo.at(i)["isDir"].toBool())
        {
            break;
        }

        result->push_back(dirpath
                          + m_dirEntryInfo.at(i)["filename"].toString());
    }
    return result;
}

QString DirModel::getFile(const QModelIndex &index) const
{
    for (int i = index.row() - 1; i >= 0; --i) {
        if (m_dirEntryInfo.at(i)["isDir"].toBool()) {
            return m_dirEntryInfo.at(i)["filename"].toString()
                    + m_dirEntryInfo.at(index.row())["filename"].toString();
        }
    }
    return QString();
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
