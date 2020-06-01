#include <QApplication>
#include <FelgoApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidJniObject>

#include "appconnection.h"
#include "datamodel.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<DirModel>("Models", 1, 0, "DirModel");
    qmlRegisterType<PlaylistModel>("Models", 1, 0, "PlaylistModel");

    // Starting service
    qDebug() << "~~~ calling startService";
    QAndroidJniObject::callStaticMethod<void>(
                "com/zdonik/musicplayer/PlayerService",
                "startMyService",
                "(Landroid/content/Context;)V",
                QtAndroid::androidActivity().object());
    qDebug() << "~~~ ending call startService";

    // Initializing DataModel
    DatabaseManager databaseManager;
    DataModel dataModel(databaseManager);

    // Connecting to service
    AppConnection connection(dataModel);
    qDebug() << "~~~  try to bind service";
    if (QtAndroid::bindService(
                    QAndroidIntent(QtAndroid::androidActivity(),
                                   "com.zdonik.musicplayer.PlayerService"),
                    connection, QtAndroid::BindFlag::AutoCreate))
    {
        qDebug() << "~~~  binding success";
    }


    // Initializing felgo application
    QApplication app(argc, argv);
    FelgoApplication felgo;
    felgo.setPreservePlatformFonts(true);

    auto imageProvider = new MusicImageProvider;
    dataModel.setMusicImageProvider(imageProvider);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataModel", &dataModel);
    engine.addImageProvider("musicImage", imageProvider);
    felgo.initialize(&engine);

    felgo.setLicenseKey(PRODUCT_LICENSE_KEY);
    felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    engine.load(QUrl(felgo.mainQmlFileName()));

    return app.exec();
}
