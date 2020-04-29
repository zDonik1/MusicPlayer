#include <QDebug>

#include "playerservice.h"

#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    qDebug() << "~~~ I'm alive !!!";
    PlayerService app(argc, argv);

    QMediaPlayer player;
    player.setMedia(QUrl("qrc:/res/Generdyn - Destiny (ft. Krigare).mp3"));
    player.setVolume(50);
//    player.play();
    qDebug() << "~~~ Started playing the best track";

    return app.exec();
}
