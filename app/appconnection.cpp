#include "appconnection.h"

#include <QDebug>
#include <QAndroidParcel>

#include <messagetype.h>
#include "datamodel.h"

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
    QVariantList musicVarList;
    auto music = m_dataModel.getMusicModel()->getAllMusic();
    for (auto &m : music)
        musicVarList.push_back(m.path);
    QAndroidParcel playlistData;
    playlistData.writeVariant(musicVarList);
    m_dataModel.getClientBinder().transact(MessageType::LOAD_PLAYLIST,
                                           playlistData);

    QAndroidParcel shuffleData;
    shuffleData.writeVariant(m_dataModel.getShuffle());
    m_dataModel.getClientBinder().transact(MessageType::SHUFFLE, shuffleData);
    QAndroidParcel repeatData;
    repeatData.writeVariant(m_dataModel.getRepeat());
    m_dataModel.getClientBinder().transact(MessageType::REPEAT, repeatData);
}
