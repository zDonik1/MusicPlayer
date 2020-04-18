#include <QDebug>

#include "playerservice.h"



int main(int argc, char *argv[])
{
    qDebug() << "~~~ I'm alive !!!";
    PlayerService app(argc, argv);
    return app.exec();
}
