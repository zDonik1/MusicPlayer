#pragma once

#include <QUrl>
#include <QImage>

struct MusicMetaData
{
    QString title = "";
    qint64 duration = 0;
    QImage image{};
};

struct Music
{
    int id = -1;
    QUrl path{""};
    MusicMetaData metaData{};
};
