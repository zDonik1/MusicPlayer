#pragma once

#include <QAbstractListModel>

class DirModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        DirRole = Qt::UserRole,
        FilenameRole
    };

public:
    DirModel();

    void setDirFilesMap(const std::map<QString, QStringList> &map);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::map<QString, QStringList> m_dirFilesMap;
};
