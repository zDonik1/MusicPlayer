#include "playerbinder.h"

#include <QDebug>
#include <QAndroidParcel>

bool PlayerBinder::onTransact(int code, const QAndroidParcel &data,
                              const QAndroidParcel &reply,
                              QAndroidBinder::CallType flags)
{
    qDebug() << "~~~  onTransact" << code << int(flags);
    switch (code) {
    case 1: {
        QAndroidBinder binder = data.readBinder();
        qDebug() << "~~~  onTransact client sent its binder !";

        // use it to sendback some data
        QAndroidParcel someData, myreply;
        someData.writeVariant(QVariant("Data from server"));
        binder.transact(2, someData, &myreply);
        qDebug() << "~~~  onTransact reply for client's binder" << myreply.readData();
        reply.writeVariant(11);
    }
        break;

    case 3:
        qDebug() << "~~~  onTransact client sent a QVariant" << data.readVariant();
        reply.writeVariant(33);
        break;

    default:
        qDebug() << "~~~  onTransact client sent unknow data" << data.readData();
        reply.writeVariant(555);
        break;
    }
    return true;
}
