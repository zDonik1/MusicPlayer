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

    case MessageType::LOAD_PLAYLIST: {
        m_player.setPlaylist(data.readVariant().toList());
    }

    case MessageType::PLAY: {
        m_player.setPlay(data.readVariant().toBool());
        break;
    }

    case MessageType::NEXT: {
        reply.writeVariant(m_player.next());
        break;
    }

    case MessageType::PREVIOUS: {
        reply.writeVariant(m_player.previous());
        break;
    }

    case MessageType::SEEK: {
        m_player.seek(data.readVariant().toLongLong());
        break;
    }

    case MessageType::SHUFFLE: {
        m_player.setShuffle(data.readVariant().toBool());
        break;
    }

    case MessageType::REPEAT: {
        m_player.setRepeat(data.readVariant().toBool());
        break;
    }

    case MessageType::MUSIC_CHANGED: {
        m_player.changeMusic(data.readVariant().toInt());
        m_player.setPlay(true);
        break;
    }
    }
    return true;
}
