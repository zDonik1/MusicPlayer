#pragma once

#include <QAndroidServiceConnection>

#include "appbinder.h"

class QApplication;
class DataModel;

class AppConnection : public QAndroidServiceConnection
{
public:
    AppConnection(QApplication &app, DataModel &dataModel);

    void onServiceConnected(const QString &name,
                            const QAndroidBinder &serviceBinder) override;
    void onServiceDisconnected(const QString &name) override;

    const QAndroidBinder &getServerBinder();

private:
    void initializePlayerService();

private:
    DataModel &m_dataModel;
    AppBinder m_serverBinder;
};
