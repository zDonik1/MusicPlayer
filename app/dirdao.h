#pragma once

#include <QDir>
#include <vector>

#include "abstractdao.h"

class DirDAO : public AbstractDAO
{
public:
    DirDAO(QSqlDatabase &database);
    ~DirDAO() override;

    void init() override;
    QString tableName() const override;

    const std::vector<QDir> &getAll() const;

private:
    std::vector<QDir> m_rootDirs;
};
