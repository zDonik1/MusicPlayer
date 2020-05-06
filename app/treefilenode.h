#pragma once

#include <QString>
#include <vector>

class TreeFileNode
{
public:
    explicit TreeFileNode(const QString &filename,
                          TreeFileNode *parent = nullptr);
    ~TreeFileNode();

    void emplaceChild(TreeFileNode *child);
    void setParent(TreeFileNode *parent);

    TreeFileNode *getChild(int row);
    int getChildCount() const;
    const QString &getFilename() const;
    int getRow() const;
    TreeFileNode *getParent();

private:
    std::vector<TreeFileNode *> m_children;
    QString m_filename;
    TreeFileNode *m_parent;
};
