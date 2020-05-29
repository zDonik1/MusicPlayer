#pragma once

#include <QAndroidService>

#include "playerbinder.h"

class Player;

class PlayerService : public QAndroidService
{
public:
    PlayerService(int &argc, char **argv, Player &player);

    QAndroidBinder *onBind(const QAndroidIntent &intent) override;

private:
    Player &m_player;
};
