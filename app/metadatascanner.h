#pragma once

#include <QMediaPlayer>
#include <vector>

#include "music.h"

class MetaDataScanner : public QObject
{
    Q_OBJECT

public:
    MetaDataScanner();

    void getMetaData(std::vector<Music> *music);

signals:
    void metaDataReady();

private slots:
    void onMetaDataChanged();

private:
    QMediaPlayer m_mediaPlayer;
    std::vector<Music> *m_music = nullptr;
    int m_counter = -1;
};
