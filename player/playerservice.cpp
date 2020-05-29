#include "playerservice.h"

#include <QDebug>

PlayerService::PlayerService(int &argc, char **argv, Player &player)
    : QAndroidService(argc, argv)
    , m_player(player)
{
}

QAndroidBinder *PlayerService::onBind(const QAndroidIntent &/*intent*/)
{
    qDebug() << "~~~ create onBind !!!";
    return new PlayerBinder(m_player);
}
