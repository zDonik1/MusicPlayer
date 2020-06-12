#pragma once

#include <QAndroidBinder>

class DataModel;

class AppBinder : public QAndroidBinder
{
public:
    AppBinder(DataModel &dataModel);

    bool onTransact(int code, const QAndroidParcel &data,
                    const QAndroidParcel &reply,
                    QAndroidBinder::CallType flags) override;

private:
    DataModel &m_dataModel;
};
