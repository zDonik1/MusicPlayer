#include "playerbinder.h"

#include <QDebug>
#include <QAndroidParcel>
#include <QStack>

#include <messagetype.h>

#include "player.h"

PlayerBinder::PlayerBinder(Player &player)
    : m_player(player)
{
}

bool PlayerBinder::onTransact(int code, const QAndroidParcel &data,
                              const QAndroidParcel &reply,
                              QAndroidBinder::CallType flags)
{
    qDebug() << "~~~ onTransact" << code << int(flags);
    switch (code) {
    case MessageType::BINDER: {
        m_player.setServerBinder(data.readBinder());
        reply.writeBinder(*(new PlayerBinder(m_player)));
    }

    case MessageType::PLAY: {
        m_player.playOrPause();
        break;
    }

    case MessageType::NEXT: {
        break;
    }

    case MessageType::PREVIOUS: {
        break;
    }

    case MessageType::SEEK: {
        break;
    }

    case MessageType::SHUFFLE: {
        break;
    }

    case MessageType::REPEAT: {
        break;
    }

    case MessageType::MUSIC_CHANGED: {
        m_player.changeMusic(data.readVariant().toUrl());
        m_player.playOrPause();
        break;
    }
    }
    return true;
}
