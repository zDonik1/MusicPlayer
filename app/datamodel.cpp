#include "datamodel.h"

#include <QAndroidParcel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStack>

#include <messagetype.h>

#include "dirdao.h"

DataModel::DataModel(DatabaseManager &databaseManager,
                     const QAndroidBinder &binder, QObject *parent)
    : QObject(parent)
    , m_databaseManager(databaseManager)
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
    QStack<QDir> stack;
    auto rootDirs = m_databaseManager.getDirDAO()->getAll();
    for (auto dir : rootDirs) {
        stack.push_back(dir);
    }

    // Recurssively traversing root dirs
    while (!stack.empty()) {
        QDir dir = stack.pop();
        QStringList filenames = dir.entryList(QDir::Files);
        if (!filenames.empty()) {
            m_filenames.emplace(dir.absolutePath(), filenames);
        }

        QStringList dirNames = dir.entryList(QDir::Dirs
                                             | QDir::NoDotAndDotDot);
        for (auto dirName : dirNames) {
            stack.push_back(QDir(dir.absolutePath() + "/" + dirName));
        }
    }

    // Converting map to json array
    QJsonArray fileList;
    for (const auto &pair : m_filenames) {
        for (const auto &filename : pair.second) {
            QJsonObject musicFile;
            musicFile["dir"] = QDir(pair.first).dirName();
            musicFile["filename"] = filename;
            fileList.push_back(musicFile);
        }
    }
    m_dirModel = QJsonDocument(fileList).toJson();
}
