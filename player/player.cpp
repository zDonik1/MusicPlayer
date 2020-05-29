#include "player.h"

#include <QAndroidParcel>

#include <messagetype.h>

Player::Player()
{
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &Player::onMediaStatusChanged);
}

void Player::play()
{
    m_mediaPlayer.play();
}

void Player::changeMusic(const QUrl &url)
{
    m_mediaPlayer.setMedia(url);
}

void Player::setServerBinder(const QAndroidBinder &serverBinder)
{
    m_serverBinder = serverBinder;
}

void Player::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    QAndroidParcel data;
    data.writeVariant(QString("media status changed to "
                              + QString::number(status)));
    m_serverBinder.transact(MessageType::DEBUG, data);
}
