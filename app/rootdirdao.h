#pragma once

#include <vector>

#include "abstractdao.h"
#include "directory.h"

class RootDirDAO : public AbstractDAO
{
public:
    RootDirDAO(QSqlDatabase &database);
    ~RootDirDAO() override;

    void init() override;
    static QString tableName();

    const std::vector<Directory> &getAll();
    int getId(const QDir &dir) const;

    int createDir(const QDir &dir);
    void deleteDir(int id);

private:
    void updateCache();

private:
    std::vector<Directory> m_rootDirs;
    bool m_dirty = true;
    int m_idCounter = 0;
};
