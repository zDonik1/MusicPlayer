#pragma once

#include <QVariantMap>

#include "abstractdao.h"

class SettingsDAO : public AbstractDAO
{
public:
    SettingsDAO(QSqlDatabase &database);
    ~SettingsDAO() override;

    void init() override;
    static QString tableName();

    QVariant getValue(const QString &key);
    bool storeValue(const QString &key, const QString &value);
    bool storeValue(const QString &key, int value);

private:
    bool storeValue(const QString &key, const QString &value,
                    QMetaType::Type type);

private:
    QVariantMap m_map;
};