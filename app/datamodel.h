#pragma once

#include <QObject>
#include <QAndroidBinder>

class DataModel : public QObject
{
    Q_OBJECT

public:
    explicit DataModel(const QAndroidBinder &binder, QObject *parent = nullptr);

public slots:
    void play();
    void next();
    void previous();
    void seek(double position);
    void shuffle();
    void repeat();
    void musicChanged(int index);

private:
    const QAndroidBinder &m_binder;
};
