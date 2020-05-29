#include "appbinder.h"

#include <QDebug>
#include <QAndroidParcel>

#include <messagetype.h>

bool AppBinder::onTransact(int code, const QAndroidParcel &data,
                          const QAndroidParcel &reply,
                          QAndroidBinder::CallType flags)
{
    switch (code) {
    case MessageType::DEBUG: {
        qDebug() << "Player:" << data.readVariant().toString();
    }
    }
    return true;
}
