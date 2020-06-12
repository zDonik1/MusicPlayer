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
        break;
    }

    case MessageType::LOAD_PLAYLIST: {
        m_player.initServiceObject();

        // checking whether the service just started or is resuming
        if (m_player.isStopped()) {
            auto map = data.readVariant().toMap();
            m_player.setPlaylist(m_player.varListToMediaContentList(
                                     map.value("playlist").toList()));
            m_player.setMusicIndex(map.value("music_index").toInt(), false);
            m_player.seek(map.value("music_position").toLongLong());

            // load to check if there is any local playback state saved
            // ... which is of higher priority than the state saved by app
            if (m_player.load())
                // if loaded, send state back to app
                m_player.sendPlaybackState();
        }
        else
            m_player.sendPlaybackState();
        break;
    }

    case MessageType::MUSIC_ADDED: {
        m_player.addMusicToPlaylist(m_player.varListToMediaContentList(
                                        data.readVariant().toList()));
        break;
    }

    case MessageType::CURRENT_PLAYLIST_DELETED: {
        m_player.clearPlaylist();
        m_player.save();
        break;
    }

    case MessageType::PLAY: {
        m_player.setPlay(data.readVariant().toBool());
        break;
    }

    case MessageType::NEXT: {
        m_player.next();
        break;
    }

    case MessageType::PREVIOUS: {
        m_player.previous();
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
        m_player.setMusicIndex(data.readVariant().toInt());
        m_player.setPlay(true);
        break;
    }

    case MessageType::PLAYLIST_SELECTED: {
        m_player.setPlaylist(m_player.varListToMediaContentList(
                                 data.readVariant().toList()));
        break;
    }
    }
    return true;
}
