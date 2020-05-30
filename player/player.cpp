#include "player.h"

#include <QAndroidParcel>

#include <messagetype.h>

Player::Player()
{
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &Player::onMediaStatusChanged);

    m_mediaPlayer.setPlaylist(&m_playlist);
}

void Player::setPlaylist(const QVariantList &list)
{
    m_mediaPlayer.stop();
    m_playlist.clear();
    for (auto &variant : list)
        m_playlist.addMedia(QUrl::fromLocalFile(variant.toUrl().toString()));
}

void Player::setPlay(bool play)
{
    if (play)
        m_mediaPlayer.play();
    else
        m_mediaPlayer.pause();
}

void Player::next()
{
    m_playlist.next();
}

void Player::previous()
{
    m_playlist.previous();
}

void Player::setShuffle(bool shuffle)
{
    m_shuffle = shuffle;
    checkShuffleRepeat();
}

void Player::setRepeat(bool repeat)
{
    m_repeat = repeat;
    checkShuffleRepeat();
}

void Player::changeMusic(int index)
{
    m_playlist.setCurrentIndex(index);
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

void Player::checkShuffleRepeat()
{
    if (m_repeat) {
        m_playlist.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        return;
    }

    if (!m_shuffle)
        m_playlist.setPlaybackMode(QMediaPlaylist::Loop);
    else
        m_playlist.setPlaybackMode(QMediaPlaylist::Random);
}
