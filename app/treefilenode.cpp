#include "treefilenode.h"

TreeFileNode::TreeFileNode(const QString &filename, TreeFileNode *parent)
    : m_filename(filename)
    , m_parent(parent)
{
}

TreeFileNode::~TreeFileNode()
{
    for (auto child : m_children) {
        delete child;
    }
}

void TreeFileNode::emplaceChild(TreeFileNode *child)
{
    child->setParent(this);
    m_children.emplace_back(child);
}

void TreeFileNode::setParent(TreeFileNode *parent)
{
    m_parent = parent;
}

TreeFileNode *TreeFileNode::getChild(int row)
{
    if (row < 0 || row > static_cast<int>(m_children.size()))
        return nullptr;

    return m_children.at(row);
}

int TreeFileNode::getChildCount() const
{
    return m_children.size();
}

const QString &TreeFileNode::getFilename() const
{
    return m_filename;
}

int TreeFileNode::getRow() const
{
    if (m_parent) {
        int iMax = m_parent->m_children.size();
        for (int i = 0; i < iMax; ++i) {
            if (m_parent->m_children.at(i) == this)
                return i;
        }
    }

    return 0;
}

TreeFileNode *TreeFileNode::getParent()
{
    return m_parent;
}
