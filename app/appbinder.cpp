#include "appbinder.h"

#include <QDebug>
#include <QAndroidParcel>

#include <messagetype.h>
#include "datamodel.h"

AppBinder::AppBinder(DataModel &dataModel)
    : m_dataModel(dataModel)
{
}

bool AppBinder::onTransact(int code, const QAndroidParcel &data,
                          const QAndroidParcel &/*reply*/,
                          QAndroidBinder::CallType /*flags*/)
{
    switch (code) {
    case MessageType::DEBUG: {
        qDebug() << "Player:" << data.readVariant().toString();
        break;
    }

    case MessageType::POSITION_CHANGED: {
        m_dataModel.setCurrentMusicPosition(data.readVariant().toLongLong());
        break;
    }
    }
    return true;
}
