#pragma once

#include <QAndroidService>

#include "playerbinder.h"

class PlayerService : public QAndroidService
{
public:
    PlayerService(int &argc, char **argv);

    QAndroidBinder *onBind(const QAndroidIntent &intent) override;
};
