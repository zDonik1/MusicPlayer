#include <QDebug>
#include <QMediaPlayer>
#include <QAndroidParcel>

#include <jni.h>

#include <messagetype.h>
#include "playerservice.h"
#include "player.h"

static Player *playerPtr = nullptr;

extern "C" {

JNIEXPORT void JNICALL Java_com_zdonik_musicplayer_NativeControls_previous(
        JNIEnv */*env*/, jobject /*obj*/)
{
    playerPtr->previous();
}

JNIEXPORT void JNICALL Java_com_zdonik_musicplayer_NativeControls_playPause(
        JNIEnv */*env*/, jobject /*obj*/)
{
    bool play = !playerPtr->getPlay();
    playerPtr->setPlay(play);
    QAndroidParcel data;
    data.writeVariant(play);
    playerPtr->getServerBinder().transact(MessageType::PLAY, data);
}

JNIEXPORT void JNICALL Java_com_zdonik_musicplayer_NativeControls_next(
        JNIEnv */*env*/, jobject /*obj*/)
{
    playerPtr->next();
}

JNIEXPORT void JNICALL Java_com_zdonik_musicplayer_NativeControls_close(
        JNIEnv */*env*/, jobject /*obj*/)
{
    playerPtr->setPlay(false);
    QAndroidParcel data;
    data.writeVariant(false);
    playerPtr->getServerBinder().transact(MessageType::PLAY, data);
    playerPtr->save();
    playerPtr->getServerBinder().transact(MessageType::QUIT, QAndroidParcel());
}

}

int main(int argc, char *argv[])
{
    Player player;
    playerPtr = &player;
    PlayerService app(argc, argv, player);
    return app.exec();
}
