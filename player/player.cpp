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

void Player::setPlaylist(const QList<QMediaContent> &musicUrls)
{
    m_mediaPlayer.stop();
    m_playlist.clear();
    addMusicToPlaylist(musicUrls);
}

void Player::addMusicToPlaylist(const QList<QMediaContent> &musicUrls)
{
    m_playlist.addMedia(musicUrls);
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
    if (!m_shuffle)
        m_playlist.next();
    else
        setRandomIndex(true);
}

void Player::previous()
{
    if (!m_shuffle)
        m_playlist.previous();
    else
        setRandomIndex(true);
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

void Player::setMusicIndex(int index)
{
    if (m_shuffle)
        m_generateRandomIndex = false;
    m_playlist.setCurrentIndex(index);
}

void Player::setServerBinder(const QAndroidBinder &serverBinder)
{
    m_serverBinder = serverBinder;
}

void Player::onCurrentIndexChanged(int /*index*/)
{
    if (!m_shuffle || m_playlist.mediaCount() == 1) {
        sendChangedIndex();
        return;
    }

    if (!m_generateRandomIndex) {
        sendChangedIndex();
        m_generateRandomIndex = true;
        return;
    }

    setRandomIndex(false);
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

void Player::setRandomIndex(bool forced)
{
    int lastMusicPlayedIndex;
    if (forced)
        lastMusicPlayedIndex = m_playlist.currentIndex();
    else
        lastMusicPlayedIndex = m_playlist.previousIndex();

    int generatedIndex = lastMusicPlayedIndex;
    while (generatedIndex == lastMusicPlayedIndex) {
        generatedIndex = QRandomGenerator::global()->generate()
                % m_playlist.mediaCount();
    }
    m_generateRandomIndex = false;
    m_playlist.setCurrentIndex(generatedIndex);
    m_mediaPlayer.play();
}

void Player::sendChangedIndex()
{
    QAndroidParcel indexData;
    indexData.writeVariant(m_playlist.currentIndex());
    m_serverBinder.transact(MessageType::MUSIC_CHANGED, indexData);
}
