#include "appconnection.h"

#include <QDebug>
#include <QApplication>
#include <QAndroidParcel>

#include <messagetype.h>
#include "datamodel.h"
#include "appstate.h"

AppConnection::AppConnection(QApplication &app, DataModel &dataModel)
    : m_dataModel(dataModel)
    , m_serverBinder(app, dataModel)
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
    auto &appState = m_dataModel.getAppState();

    QVariantList musicVarList;
    auto music = m_dataModel.getMusicModel()->getAllMusic();
    for (auto &m : music)
        musicVarList.push_back(m.path);
    QVariantMap map;
    map["playlist"] = musicVarList;
    map["music_index"] = appState.getCurrentMusicIndex();
    map["music_position"] = appState.getCurrentMusicPosition();
    QAndroidParcel playlistData;
    playlistData.writeVariant(map);
    clientBinder.transact(MessageType::LOAD_PLAYLIST, playlistData);

    QAndroidParcel shuffleData;
    shuffleData.writeVariant(appState.getShuffle());
    clientBinder.transact(MessageType::SHUFFLE, shuffleData);
    QAndroidParcel repeatData;
    repeatData.writeVariant(appState.getRepeat());
    clientBinder.transact(MessageType::REPEAT, repeatData);
}
