#include "appbinder.h"

#include <QDebug>
#include <QAndroidParcel>

bool AppBinder::onTransact(int code, const QAndroidParcel &data,
                          const QAndroidParcel &reply,
                          QAndroidBinder::CallType flags)
{
    qDebug() << "~~~  client: onTransact " << code << data.readVariant() << int(flags);
    reply.writeData("Yupee");
    return true;
}
