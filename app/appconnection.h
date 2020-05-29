#pragma once

#include <QAndroidServiceConnection>

#include "appbinder.h"

class DataModel;

class AppConnection : public QAndroidServiceConnection
{
public:
    AppConnection(DataModel &dataModel);

    void onServiceConnected(const QString &name,
                            const QAndroidBinder &serviceBinder) override;
    void onServiceDisconnected(const QString &name) override;

    const QAndroidBinder &getServerBinder();

private:
    AppBinder m_serverBinder;

    DataModel &m_dataModel;
};
