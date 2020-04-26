#include "datamodel.h"

#include <QAndroidParcel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>

#include <messagetype.h>

DataModel::DataModel(const QAndroidBinder &binder, QObject *parent)
    : QObject(parent)
    , m_binder(binder)
{
}

QString &DataModel::getDirModel()
{
    return m_dirModel;
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

void DataModel::refreshDirs()
{
    QAndroidParcel sendData, replyData;
    m_binder.transact(MessageType::REFRESH_DIRS, sendData, &replyData);
    QVariantMap dirContents = replyData.readVariant().toMap();

    QJsonArray fileList;
    auto keys = dirContents.keys();
    for (const auto &key : keys) {
        auto values = dirContents[key].toList();
        for (const auto &file : values) {
            QJsonObject musicFile;
            musicFile["dir"] = QDir(key).dirName();
            musicFile["filename"] = file.toString();
            fileList.push_back(musicFile);
        }
    }
    m_dirModel = QJsonDocument(fileList).toJson();
}
