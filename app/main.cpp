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
    // Starting service
    qDebug() << "~~~ calling startService";
    QAndroidJniObject::callStaticMethod<void>(
                "com/zdonik/musicplayer/PlayerService",
                "startMyService",
                "(Landroid/content/Context;)V",
                QtAndroid::androidActivity().object());
    qDebug() << "~~~ ending call startService";

    // Connecting to service
    AppConnection connection;
    qDebug() << "~~~  try to bind service";
    if (QtAndroid::bindService(
                    QAndroidIntent(QtAndroid::androidActivity(),
                                   "com.zdonik.musicplayer.PlayerService"),
                    connection, QtAndroid::BindFlag::AutoCreate))
    {
        qDebug() << "~~~  binding success";
    }

    // Initializing DataModel
    DatabaseManager databaseManager;
    DataModel dataModel(databaseManager, connection.getServerBinder());

    // Initializing felgo application
    QApplication app(argc, argv);
    FelgoApplication felgo;

    felgo.setPreservePlatformFonts(true);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataModel", &dataModel);
    felgo.initialize(&engine);

    felgo.setLicenseKey(PRODUCT_LICENSE_KEY);
    felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    engine.load(QUrl(felgo.mainQmlFileName()));

    return app.exec();
}
