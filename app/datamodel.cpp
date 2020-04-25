#include "datamodel.h"

#include <QAndroidParcel>

#include <messagetype.h>

DataModel::DataModel(const QAndroidBinder &binder, QObject *parent)
    : QObject(parent)
    , m_binder(binder)
{
}

void DataModel::play()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::PLAY, sendData, &replyData);
}

void DataModel::next()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::NEXT, sendData, &replyData);
}

void DataModel::previous()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::PREVIOUS, sendData, &replyData);
}

void DataModel::seek(double position)
{
    QAndroidParcel sendData, replyData;
    sendData.writeVariant(position);
    m_binder.transact(MessageType::SEEK, sendData, &replyData);
}

void DataModel::shuffle()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::SHUFFLE, sendData, &replyData);
}

void DataModel::repeat()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::REPEAT, sendData, &replyData);
}

void DataModel::musicChanged(int index)
{
    QAndroidParcel sendData, replyData;
    sendData.writeVariant(index);
    m_binder.transact(MessageType::SEEK, sendData, &replyData);
}
