#pragma once

#include <QAndroidBinder>

class AppBinder : public QAndroidBinder
{
public:
    bool onTransact(int code, const QAndroidParcel &data,
                    const QAndroidParcel &reply,
                    QAndroidBinder::CallType flags) override;
};
