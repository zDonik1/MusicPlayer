#pragma once

#include <QMediaPlayer>
#include <vector>

#include "music.h"

class MetaDataScanner : public QObject
{
    Q_OBJECT

public:
    MetaDataScanner();

    void getMetaData(const std::vector<Music *> &music);
    void getMetaData(Music *music);

signals:
    void metaDataReady();

private slots:
    void onMetaDataChanged();

private:
    QMediaPlayer m_mediaPlayer;
    std::vector<Music *> m_music;
    int m_counter = -1;
};
