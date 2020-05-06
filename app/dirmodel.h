#pragma once

#include <QAbstractItemModel>

class TreeFileNode;

class DirModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    DirModel(QObject *parent = nullptr);
    ~DirModel();

    void setDirFilesMap(const std::map<QString, QStringList> &map);

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    TreeFileNode *m_root;
};
