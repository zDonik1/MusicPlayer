#include "appconnection.h"

#include <QDebug>
#include <QAndroidParcel>

#include <messagetype.h>
#include "datamodel.h"

AppConnection::AppConnection(DataModel &dataModel)
    : m_dataModel(dataModel)
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

    QVariantList musicVarList;
    auto music = m_dataModel.getMusicModel()->getAllMusic();
    for (auto &m : music)
        musicVarList.push_back(m.path);
    QAndroidParcel playlistData;
    playlistData.writeVariant(musicVarList);
    m_dataModel.getClientBinder().transact(MessageType::LOAD_PLAYLIST,
                                           playlistData);
}

void AppConnection::onServiceDisconnected(const QString &name)
{
    qDebug() << "~~~  onServiceDisconnected" << name;
}

const QAndroidBinder &AppConnection::getServerBinder()
{
    return m_serverBinder;
}
