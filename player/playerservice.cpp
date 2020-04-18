#include "playerservice.h"

#include <QDebug>

PlayerService::PlayerService(int &argc, char **argv)
    : QAndroidService(argc, argv)
{
}

QAndroidBinder *PlayerService::onBind(const QAndroidIntent &/*intent*/)
{
    qDebug() << "~~~ create onBind !!!";
    return new PlayerBinder();
}
