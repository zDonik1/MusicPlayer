#include "dirmodel.h"

#include <QDir>

#include "treefilenode.h"

DirModel::DirModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

DirModel::~DirModel()
{
    delete m_root;
}

void DirModel::setDirFilesMap(const std::map<QString, QStringList> &map)
{
    beginResetModel();
    for (const auto &pair : map) {
        TreeFileNode *dir = new TreeFileNode(pair.first);
        for (const auto &filename : pair.second) {
            dir->emplaceChild(new TreeFileNode(filename));
        }
        m_root->emplaceChild(dir);
    }
    endResetModel();
}

QVariant DirModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeFileNode *item = static_cast<TreeFileNode *>(index.internalPointer());
    return item->getFilename();
}

QModelIndex DirModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeFileNode *parentItem;
    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TreeFileNode *>(parent.internalPointer());

    TreeFileNode *childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex DirModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeFileNode *childItem
            = static_cast<TreeFileNode *>(child.internalPointer());
    TreeFileNode *parentItem = childItem->getParent();
    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->getRow(), 0, parentItem);
}

int DirModel::rowCount(const QModelIndex &parent) const
{
    TreeFileNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TreeFileNode *>(parent.internalPointer());

    return parentItem->getChildCount();
}

int DirModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QHash<int, QByteArray> DirModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "rDisplay";
    return roles;
}
