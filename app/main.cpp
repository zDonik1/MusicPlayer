#include <QApplication>
#include <FelgoApplication>

#include <QQmlApplicationEngine>

#include <QDebug>
#include <QColor>
#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidParcel>
#include <QAndroidServiceConnection>
#include <QAndroidJniObject>
#include "appbinder.h"

class AppConnection : public QAndroidServiceConnection
{
public:
    void onServiceConnected(const QString &name, const QAndroidBinder &serviceBinder) override
    {
        qDebug() << "~~~  onServiceConnected" << name;

        {
            // test binder
            QAndroidParcel sendData, replyData;
            sendData.writeBinder(m_binder);
            serviceBinder.transact(1, sendData, &replyData);
            qDebug() << replyData.readVariant();
        }

        {
            // test QVariant
            QAndroidParcel sendData, replyData;
            QVariantHash testData;
            QVariantMap testMap;
            testMap["1"] = QVariant("one");
            testMap["2"] = QVariant("two");
            testData["0"] = testMap;
            testData["QString"] = QString("QString");
            testData["QColor"] = QColor(Qt::red);
            sendData.writeVariant(testData);
            serviceBinder.transact(3, sendData, &replyData);
            qDebug() << replyData.readVariant();
        }

        {
            // test any data
            QAndroidParcel sendData, replyData;
            sendData.writeData("Here goes any data we like");
            serviceBinder.transact(4, sendData, &replyData);
            qDebug() << replyData.readVariant();
        }
    }

    void onServiceDisconnected(const QString &name) override
    {
        qDebug() << "~~~  onServiceDisconnected" << name;
    }

private:
    AppBinder m_binder;
};

int main(int argc, char *argv[])
{
    qDebug() << "~~~ calling startService";
    QAndroidJniObject::callStaticMethod<void>(
                "com/zdonik/musicplayer/PlayerService",
                "startMyService",
                "(Landroid/content/Context;)V",
                QtAndroid::androidActivity().object());
    qDebug() << "~~~ ending call startService";

    AppConnection connection;
    qDebug() << "~~~  try to bind service";
    if (QtAndroid::bindService(
                    QAndroidIntent(QtAndroid::androidActivity(),
                                   "com.zdonik.musicplayer.PlayerService"),
                    connection, QtAndroid::BindFlag::AutoCreate))
    {
        qDebug() << "~~~  binding success";
    }

    QApplication app(argc, argv);
    FelgoApplication felgo;

    // Use platform-specific fonts instead of Felgo's default font
    felgo.setPreservePlatformFonts(true);

    QQmlApplicationEngine engine;
    felgo.initialize(&engine);

    felgo.setLicenseKey(PRODUCT_LICENSE_KEY);
    felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    engine.load(QUrl(felgo.mainQmlFileName()));

    return app.exec();
}
