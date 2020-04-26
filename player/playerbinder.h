#pragma once

#include <QAndroidBinder>
#include <QDir>
#include <QMap>

class QAndroidParcel;

class PlayerBinder : public QAndroidBinder
{
public:
    bool onTransact(int code, const QAndroidParcel &data,
                    const QAndroidParcel &reply,
                    QAndroidBinder::CallType flags) override;
};
