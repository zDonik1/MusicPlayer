#pragma once

#include <QObject>
#include <QAndroidBinder>
#include <QQmlListProperty>
#include <QDir>

#include "databasemanager.h"

class DataModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dirModel READ getDirModel NOTIFY dirModelChanged)

public:
    explicit DataModel(DatabaseManager &databaseManager,
                       const QAndroidBinder &binder, QObject *parent = nullptr);

    QString &getDirModel();

public slots:
    void play();
    void next();
    void previous();
    void seek(double position);
    void shuffle();
    void repeat();
    void musicChanged(int index);

    void refreshDirs();

signals:
    void dirModelChanged();

private:
    DatabaseManager &m_databaseManager;
    const QAndroidBinder &m_binder;

    QString m_dirModel;
    std::map<QString, QStringList> m_filenames;
};
