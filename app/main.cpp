#include <QApplication>
#include <FelgoApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidJniObject>

#include "appconnection.h"
#include "databasemanager.h"
#include "appstate.h"
#include "datamodel.h"
#include "musicimageprovider.h"

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

    // Initializing random objects
    DatabaseManager databaseManager;
    AppState appState(*databaseManager.getSettingsDAO());
    DataModel dataModel(databaseManager, appState);
    QApplication app(argc, argv);

    // Connecting to service
    AppConnection connection(app, dataModel);
    qDebug() << "~~~  try to bind service";
    if (QtAndroid::bindService(
                    QAndroidIntent(QtAndroid::androidActivity(),
                                   "com.zdonik.musicplayer.PlayerService"),
                    connection, QtAndroid::BindFlag::AutoCreate))
    {
        qDebug() << "~~~  binding success";
    }

    // Initializing felgo application
    FelgoApplication felgo;
    felgo.setPreservePlatformFonts(true);

    auto imageProvider = new MusicImageProvider;
    dataModel.setMusicImageProvider(imageProvider);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataModel", &dataModel);
    engine.rootContext()->setContextProperty("appState", &appState);
    engine.addImageProvider("musicImage", imageProvider);
    felgo.initialize(&engine);

    felgo.setLicenseKey(PRODUCT_LICENSE_KEY);
    felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    engine.load(QUrl(felgo.mainQmlFileName()));

    return app.exec();
}
