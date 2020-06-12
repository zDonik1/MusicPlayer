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

void startService(AppConnection &connection)
{
    // Starting service
    qDebug() << "~~~ calling startService";
    QAndroidJniObject::callStaticMethod<void>(
                "com/zdonik/musicplayer/PlayerService",
                "startMyService",
                "(Landroid/content/Context;)V",
                QtAndroid::androidActivity().object());
    qDebug() << "~~~ ending call startService";

    // Connecting to service
    qDebug() << "~~~  try to bind service";
    if (QtAndroid::bindService(
                    QAndroidIntent(QtAndroid::androidActivity(),
                                   "com.zdonik.musicplayer.PlayerService"),
                    connection, QtAndroid::BindFlag::AutoCreate))
    {
        qDebug() << "~~~  binding success";
    }
}

int main(int argc, char *argv[])
{
    qmlRegisterType<DirModel>("Models", 1, 0, "DirModel");
    qmlRegisterType<PlaylistModel>("Models", 1, 0, "PlaylistModel");

    // Initializing random objects
    DatabaseManager databaseManager;
    AppState appState(*databaseManager.getSettingsDAO());
    DataModel dataModel(databaseManager, appState);
    AppConnection connection(dataModel);
    startService(connection);

    // Initializing felgo application
    QApplication app(argc, argv);
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
