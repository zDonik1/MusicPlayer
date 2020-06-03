#include "appconnection.h"

#include <QDebug>
#include <QAndroidParcel>

#include <messagetype.h>
#include "datamodel.h"
#include "appstate.h"

AppConnection::AppConnection(DataModel &dataModel)
    : m_dataModel(dataModel)
    , m_serverBinder(dataModel)
{
}

void AppConnection::onServiceConnected(const QString &name,
                                       const QAndroidBinder &serviceBinder)
{
    qDebug() << "~~~  onServiceConnected" << name;
    QAndroidParcel binderData, reply;
    binderData.writeBinder(m_serverBinder);
    serviceBinder.transact(MessageType::BINDER, binderData, &reply);
    m_dataModel.setClientBinder(reply.readBinder());

    initializePlayerService();
}

void AppConnection::onServiceDisconnected(const QString &name)
{
    qDebug() << "~~~  onServiceDisconnected" << name;
}

const QAndroidBinder &AppConnection::getServerBinder()
{
    return m_serverBinder;
}

void AppConnection::initializePlayerService()
{
    auto &clientBinder = m_dataModel.getClientBinder();

    QVariantList musicVarList;
    auto music = m_dataModel.getMusicModel()->getAllMusic();
    for (auto &m : music)
        musicVarList.push_back(m.path);
    QAndroidParcel playlistData;
    playlistData.writeVariant(musicVarList);
    clientBinder.transact(MessageType::LOAD_PLAYLIST, playlistData);

    auto &appState = m_dataModel.getAppState();
    QAndroidParcel musicIndexData;
    musicIndexData.writeVariant(appState.getCurrentMusicIndex());
    clientBinder.transact(MessageType::LOAD_MUSIC_INDEX, musicIndexData);

    QAndroidParcel musicPositionData;
    musicPositionData.writeVariant(appState.getCurrentMusicPosition());
    clientBinder.transact(MessageType::SEEK, musicPositionData);

    QAndroidParcel shuffleData;
    shuffleData.writeVariant(appState.getShuffle());
    clientBinder.transact(MessageType::SHUFFLE, shuffleData);
    QAndroidParcel repeatData;
    repeatData.writeVariant(appState.getRepeat());
    clientBinder.transact(MessageType::REPEAT, repeatData);
}
