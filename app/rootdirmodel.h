#pragma once

#include <QAbstractListModel>

#include <directory.h>

class RootDirModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
       PathRole = Qt::UserRole,
    };

public:
    RootDirModel();

    const Directory &getRootDir(const QModelIndex &index) const;

    void setRootDirs(const std::vector<Directory> &dirs);
    void addRootDir(const Directory &dir);
    int deleteRootDir(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<Directory> m_rootDirs;
};
