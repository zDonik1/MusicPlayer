#include "playerbinder.h"

#include <QDebug>
#include <QAndroidParcel>
#include <QStack>

#include <messagetype.h>

#include "dirdao.h"

PlayerBinder::PlayerBinder(DatabaseManager &databaseMgr)
    : m_databaseMgr(databaseMgr)
{
}

bool PlayerBinder::onTransact(int code, const QAndroidParcel &data,
                              const QAndroidParcel &reply,
                              QAndroidBinder::CallType flags)
{
    qDebug() << "~~~  onTransact" << code << int(flags);
    switch (code) {
    case MessageType::PLAY: {
        break;
    }

    case MessageType::NEXT: {
        break;
    }

    case MessageType::PREVIOUS: {
        break;
    }

    case MessageType::SEEK: {
        break;
    }

    case MessageType::SHUFFLE: {
        break;
    }

    case MessageType::REPEAT: {
        break;
    }

    case MessageType::MUSIC_CHANGED: {
        break;
    }

    case MessageType::REFRESH_DIRS: {
        QStack<QDir> stack;
        auto rootDirs = m_databaseMgr.getDirDAO()->getAll();
        for (auto dir : rootDirs) {
            stack.push_back(dir);
        }

        while (!stack.empty()) {
            QDir dir = stack.pop();
            QStringList fileNames = dir.entryList(QDir::Files);
            if (!fileNames.empty()) {
                m_filenames[dir.absolutePath()] = fileNames;
            }

            QStringList dirNames = dir.entryList(QDir::Dirs
                                                 | QDir::NoDotAndDotDot);
            for (auto dirName : dirNames) {
                stack.push_back(QDir(dir.absolutePath() + "/" + dirName));
            }
        }

        reply.writeVariant(m_filenames);
        break;
    }
    }
    return true;
}
