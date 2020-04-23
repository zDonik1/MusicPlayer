#pragma once

#include <QUrl>
#include <vector>

class MusicDAO
{
public:
    MusicDAO();

    std::vector<QUrl> getAll();
};
