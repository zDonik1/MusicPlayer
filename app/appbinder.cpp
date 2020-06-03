#include "appbinder.h"

#include <QDebug>
#include <QAndroidParcel>

#include <messagetype.h>
#include "datamodel.h"
#include "appstate.h"

AppBinder::AppBinder(DataModel &dataModel)
    : m_dataModel(dataModel)
{
}

bool AppBinder::onTransact(int code, const QAndroidParcel &data,
                          const QAndroidParcel &/*reply*/,
                          QAndroidBinder::CallType /*flags*/)
{
    auto &appState = m_dataModel.getAppState();
    switch (code) {
    case MessageType::DEBUG: {
        qDebug() << "Player:" << data.readVariant().toString();
        break;
    }

    case MessageType::POSITION_CHANGED: {
        appState.setCurrentMusicPosition(data.readVariant().toLongLong());
        break;
    }

    case MessageType::MUSIC_CHANGED: {
        int musicIndex = data.readVariant().toInt();
        appState.setCurrentMusicIndex(musicIndex);
        auto *musicModel = m_dataModel.getMusicModel();
        int64_t duration = musicModel
                ->getMusic(musicModel->index(musicIndex)).metaData.duration;
        appState.setCurrentMusicDuration(duration);
        appState.setCurrentMusicPosition(0);
        break;
    }
    }
    return true;
}
