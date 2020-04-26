#include "playerbinder.h"

#include <QDebug>
#include <QAndroidParcel>
#include <QStack>

#include <messagetype.h>

#include "dirdao.h"

PlayerBinder::PlayerBinder(DatabaseManager &databaseMgr)
    : m_databaseMgr(databaseMgr)
{
}

bool PlayerBinder::onTransact(int code, const QAndroidParcel &data,
                              const QAndroidParcel &reply,
                              QAndroidBinder::CallType flags)
{
    qDebug() << "~~~  onTransact" << code << int(flags);
    switch (code) {
    case MessageType::PLAY: {
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
        break;
    }
    }
    return true;
}
