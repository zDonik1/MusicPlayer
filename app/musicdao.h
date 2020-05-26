#pragma once

#include <vector>

#include "abstractdao.h"
#include "music.h"

class MusicDAO : public AbstractDAO
{
    Q_OBJECT

public:
    MusicDAO(QSqlDatabase &database);
    ~MusicDAO() override;

    void init() override;
    static QString tableName();

    const std::vector<Music> &getAll();
    int findByPath(const QUrl &path) const;
    int createMusic(const QUrl &path);
    void updateMusic(const Music &music);
    void deleteMusic(int id);

private:
    void updateCache();

private:
    std::vector<Music> m_music;
    bool m_dirty = true;
    int m_idCounter = 0;
};
