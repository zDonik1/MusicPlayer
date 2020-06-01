#include "player.h"

#include <QAndroidParcel>
#include <QRandomGenerator>

#include <messagetype.h>

Player::Player()
{
    connect(&m_playlist, &QMediaPlaylist::currentIndexChanged,
            this, &Player::onCurrentIndexChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::stateChanged,
            this, &Player::onPlayerStateChanged);
    m_positionTimer.callOnTimeout(this, &Player::onPositionTimerTimeout);

    m_mediaPlayer.setPlaylist(&m_playlist);
    m_positionTimer.setInterval(500);
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

int Player::next()
{
    m_playlist.next();
    return m_playlist.currentIndex();
}

int Player::previous()
{
    m_playlist.previous();
    return m_playlist.currentIndex();
}

void Player::seek(int64_t position)
{
    m_mediaPlayer.setPosition(position);
}

void Player::setShuffle(bool shuffle)
{
    m_shuffle = shuffle;
}

void Player::setRepeat(bool repeat)
{
    m_repeat = repeat;
    if (m_repeat)
        m_playlist.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    else
        m_playlist.setPlaybackMode(QMediaPlaylist::Loop);
}

void Player::changeMusic(int index)
{
    m_playlist.setCurrentIndex(index);
}

void Player::setServerBinder(const QAndroidBinder &serverBinder)
{
    m_serverBinder = serverBinder;
}

void Player::onCurrentIndexChanged(int /*index*/)
{
    if (!m_shuffle)
        return;

    if (m_doneChangingIndex)
        return;

    int generatedIndex = m_playlist.previousIndex();
    while (generatedIndex == m_playlist.previousIndex()) {
        generatedIndex = QRandomGenerator::global()->generate()
                % m_playlist.mediaCount();
    }
    m_doneChangingIndex = true;
    m_playlist.setCurrentIndex(generatedIndex);
    m_doneChangingIndex = false;
}

void Player::onPlayerStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState)
        m_positionTimer.start();
    else
        m_positionTimer.stop();
}

void Player::onPositionTimerTimeout()
{
    QAndroidParcel data;
    data.writeVariant(m_mediaPlayer.position());
    m_serverBinder.transact(MessageType::POSITION_CHANGED, data);
}

void Player::debug(const QString &message)
{
    QAndroidParcel data;
    data.writeVariant(message);
    m_serverBinder.transact(MessageType::DEBUG, data);
}
