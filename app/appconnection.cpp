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
    QAndroidParcel data, reply;
    data.writeBinder(m_serverBinder);
    serviceBinder.transact(MessageType::BINDER, data, &reply);
    m_dataModel.setClientBinder(reply.readBinder());
}

void AppConnection::onServiceDisconnected(const QString &name)
{
    qDebug() << "~~~  onServiceDisconnected" << name;
}

const QAndroidBinder &AppConnection::getServerBinder()
{
    return m_serverBinder;
}
