#include "appconnection.h"

#include <QDebug>
#include <QAndroidParcel>

void AppConnection::onServiceConnected(const QString &name, const QAndroidBinder &serviceBinder)
{
    qDebug() << "~~~  onServiceConnected" << name;

    {
        // test binder
        QAndroidParcel sendData, replyData;
        sendData.writeBinder(m_binder);
        serviceBinder.transact(1, sendData, &replyData);
        qDebug() << replyData.readVariant();
    }

    {

    }

    {
        // test any data
        QAndroidParcel sendData, replyData;
        sendData.writeData("Here goes any data we like");
        serviceBinder.transact(4, sendData, &replyData);
        qDebug() << replyData.readVariant();
    }
}

void AppConnection::onServiceDisconnected(const QString &name)
{
    qDebug() << "~~~  onServiceDisconnected" << name;
}

const QAndroidBinder &AppConnection::getServerBinder()
{
    return m_binder;
}
