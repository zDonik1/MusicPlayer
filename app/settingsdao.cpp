#include "settingsdao.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SettingsDAO::SettingsDAO(QSqlDatabase &database)
    : AbstractDAO(database)
{
}

SettingsDAO::~SettingsDAO()
{
}

void SettingsDAO::init()
{
    if (!m_database.tables().contains(tableName())) {
        QSqlQuery query(m_database);
        QString queryString = QStringLiteral
                ("create table %1 (mykey text primary key, myvalue text, "
                 "mytype integer)")
                .arg(tableName());
        query.exec(queryString);
    }

    // init cache
    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("select mykey, myvalue, mytype from %1")
            .arg(tableName());
    query.exec(queryString);
    while (query.next()) {
        QVariant variant;
        switch (query.value(2).toInt()) {
        case QMetaType::QString:
            variant = query.value(1);
            break;

        case QMetaType::Int:
            variant = query.value(1).toString().toInt();
            break;

        case QMetaType::LongLong:
            variant = query.value(1).toString().toLongLong();
            break;

        case QMetaType::Bool:
            variant = static_cast<bool>(query.value(1).toString().toInt());
            break;
        }
        m_map[query.value(0).toString()] = variant;
    }
}

void SettingsDAO::reset()
{
    if (m_database.tables().contains(tableName())) {
        QSqlQuery query(m_database);
        QString queryString = QStringLiteral("drop table %1")
                .arg(tableName());
        query.exec(queryString);
    }
}

QString SettingsDAO::tableName()
{
    return "setting";
}

QVariant SettingsDAO::getValue(const QString &key)
{
    if (key.isEmpty())
        return QVariant();

    return m_map.value(key, QVariant());
}

bool SettingsDAO::storeValue(const QString &key, const QString &value)
{
    return storeValue(key, value, QMetaType::QString);
}

bool SettingsDAO::storeValue(const QString &key, int value)
{
    return storeValue(key, QString::number(value), QMetaType::Int);
}

bool SettingsDAO::storeValue(const QString &key, int64_t value)
{
    return storeValue(key, QString::number(value), QMetaType::LongLong);
}

bool SettingsDAO::storeValue(const QString &key, bool value)
{
    return storeValue(key, QString::number(value ? 1 : 0), QMetaType::Bool);
}

bool SettingsDAO::storeValue(const QString &key, const QString &value,
                             QMetaType::Type type)
{
    if (key.isEmpty() || value.isEmpty())
        return false;

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("replace into %1 (mykey, myvalue, mytype) values ('%2', '%3', %4)")
            .arg(tableName())
            .arg(key)
            .arg(value)
            .arg(type);
    if (!query.exec(queryString)) {
        qDebug() << "Failed storing:" << query.lastError().text();
        return false;
    }

    m_map[key] = value;
    return true;
}
