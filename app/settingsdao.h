#pragma once

#include <QVariantMap>

#include "abstractdao.h"

class SettingsDAO : public AbstractDAO
{
public:
    SettingsDAO(QSqlDatabase &database);
    ~SettingsDAO() override;

    void init() override;
    void reset() override;
    static QString tableName();

    QVariant getValue(const QString &key) const;
    bool storeValue(const QString &key, const QString &value);
    bool storeValue(const QString &key, int value);
    bool storeValue(const QString &key, int64_t value);
    bool storeValue(const QString &key, bool value);

private:
    bool storeValue(const QString &key, const QString &value,
                    QMetaType::Type type);

private:
    QVariantMap m_map;
};
