#pragma once

#include <QAndroidBinder>
#include <QDir>
#include <QMap>

#include "databasemanager.h"

class QAndroidParcel;

class PlayerBinder : public QAndroidBinder
{
public:
    PlayerBinder(DatabaseManager &databaseMgr);

    bool onTransact(int code, const QAndroidParcel &data,
                    const QAndroidParcel &reply,
                    QAndroidBinder::CallType flags) override;

private:
    DatabaseManager &m_databaseMgr;

    QVariantMap m_filenames;
};
