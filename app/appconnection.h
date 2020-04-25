#pragma once

#include <QAndroidServiceConnection>

#include "appbinder.h"

class AppConnection : public QAndroidServiceConnection
{
public:
    void onServiceConnected(const QString &name, const QAndroidBinder &serviceBinder) override;
    void onServiceDisconnected(const QString &name) override;

    const QAndroidBinder &getServerBinder();

private:
    AppBinder m_binder;
};
