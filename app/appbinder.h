#pragma once

#include <QAndroidBinder>

class QApplication;
class DataModel;

class AppBinder : public QAndroidBinder
{
public:
    AppBinder(QApplication &app, DataModel &dataModel);

    bool onTransact(int code, const QAndroidParcel &data,
                    const QAndroidParcel &reply,
                    QAndroidBinder::CallType flags) override;

private:
    QApplication &m_app;
    DataModel &m_dataModel;
};
