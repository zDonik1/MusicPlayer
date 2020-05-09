#pragma once

#include <vector>

#include "abstractdao.h"
#include "music.h"

class MusicDAO : public AbstractDAO
{
public:
    MusicDAO(QSqlDatabase &database);
    ~MusicDAO() override;

    void init() override;
    QString tableName() const override;

    const std::vector<Music> &getAll();
    const Music &getMusic(int id);
    void createMusic(const Music &music);
    void updateMusic(const Music &music);
    void deleteMusic(int id);

private:
    void saveToCache(QSqlQuery &result);

private:
    bool m_dirty = true;
    std::vector<Music> m_music;
};
