#pragma once

#include <QAbstractListModel>

class DirModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IsDirRole = Qt::UserRole,
        IsOpen,
        NameRole,
    };

public:
    DirModel();

    void setupModel(const std::map<QString, QStringList> &map);
    void toggleDir(const QModelIndex &index);

    std::unique_ptr<std::vector<QString>> getFilesInDir(
            const QModelIndex &index) const;
    QString getFile(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    // hash contains filename [QString], isDir [bool], isOpen[bool]
    std::vector<QVariantHash> m_dirEntryInfo;
};
