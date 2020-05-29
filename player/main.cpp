#include <QDebug>
#include <QMediaPlayer>

#include "playerservice.h"
#include "player.h"

int main(int argc, char *argv[])
{
    qDebug() << "~~~ I'm alive !!!";
    Player player;
    PlayerService app(argc, argv, player);
    qDebug() << "~~~ Started playing the best track";

    return app.exec();
}
