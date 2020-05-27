#include "settingsdao.h"

#include <QSqlQuery>

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
        case QMetaType::Int:
            variant = query.value(1).toString().toInt();
            break;

        case QMetaType::QString:
            variant = query.value(1);
            break;
        }
        m_map[query.value(0).toString()] = variant;
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

bool SettingsDAO::storeValue(const QString &key, const QString &value,
                             QMetaType::Type type)
{
    if (key.isEmpty() || value.isEmpty())
        return false;

    QSqlQuery query(m_database);
    QString queryString = QStringLiteral
            ("insert into %1 (mykey, myvalue, mytype) values ('%2', '%3', %4)")
            .arg(tableName())
            .arg(key)
            .arg(value)
            .arg(type);
    if (!query.exec(queryString))
        return false;

    m_map[key] = value;
    return true;
}
