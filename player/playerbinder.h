#pragma once

#include <QAndroidBinder>
#include <QDir>
#include <QMap>
#include <QMediaContent>

class QAndroidParcel;
class Player;

class PlayerBinder : public QAndroidBinder
{

public:
    PlayerBinder(Player &player);

    bool onTransact(int code, const QAndroidParcel &data,
                    const QAndroidParcel &reply,
                    QAndroidBinder::CallType flags) override;

private:
    Player &m_player;
};
