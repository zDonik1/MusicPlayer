#pragma once

#include <QObject>
#include <QAndroidBinder>
#include <QQmlListProperty>
#include <QDir>

#include "databasemanager.h"
#include "dirmodel.h"

class DataModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DirModel *dirModel READ getDirModel NOTIFY dirModelChanged)

public:
    explicit DataModel(DatabaseManager &databaseManager,
                       const QAndroidBinder &binder, QObject *parent = nullptr);

    DirModel *getDirModel();

public slots:
    void play();
    void next();
    void previous();
    void seek(double position);
    void shuffle();
    void repeat();
    void musicChanged(int index);

    void refreshDirs();
    void toggleDir(int index);

signals:
    void dirModelChanged();

private:
    DatabaseManager &m_databaseManager;
    const QAndroidBinder &m_binder;

    DirModel m_dirModel;
};
