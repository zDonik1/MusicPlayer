#pragma once

#include <QUrl>

struct Music
{
    int id = -1;
    QUrl path{""};
    int playlistId = -1;
};