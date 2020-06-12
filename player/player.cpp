#include "player.h"

#include <QAndroidParcel>
#include <QAndroidJniObject>
#include <QRandomGenerator>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>

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

Player::~Player()
{
    save();
    qDebug() << "^^^^ player destructor called: saved playlist";
}

void Player::debug(const QString &message)
{
    QAndroidParcel data;
    data.writeVariant(message);
    m_serverBinder.transact(MessageType::DEBUG, data);
}

QList<QMediaContent> Player::varListToMediaContentList(const QVariantList &list)
{
    QList<QMediaContent> musicUrls;
    for (auto &variant : list)
        musicUrls.push_back(QUrl::fromLocalFile(variant.toUrl().toString()));
    return musicUrls;
}

void Player::initServiceObject()
{
    m_javaService = QAndroidJniObject::callStaticObjectMethod
            ("com/zdonik/musicplayer/PlayerService", "instance",
             "()Lcom/zdonik/musicplayer/PlayerService;");
}

bool Player::load()
{
    auto appDataLocation = QStandardPaths::writableLocation(
                QStandardPaths::AppDataLocation);
    QFile settingsFile(appDataLocation.append("/").append(settingsFilename));
    if (!settingsFile.exists())
        return false;

    if (!settingsFile.open(QIODevice::ReadOnly))
        return false;

    QDataStream input;
    int musicIndex;
    quint64 musicDuration, musicPosition;
    input >> musicIndex >> musicDuration >> musicPosition;

    if (musicIndex < 0)
        return false;

    m_playlist.setCurrentIndex(musicIndex);
    m_mediaPlayer.setPosition(musicPosition);
    return true;
}

void Player::save()
{
    auto appDataLocation = QStandardPaths::writableLocation(
                QStandardPaths::AppDataLocation);

    QFile settingsFile(appDataLocation.append("/").append(settingsFilename));
    settingsFile.open(QIODevice::WriteOnly);
    QDataStream output;
    output << m_playlist.currentIndex() << m_mediaPlayer.duration()
           << m_mediaPlayer.position();
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

void Player::clearPlaylist()
{
    m_playlist.clear();
}

void Player::sendPlaybackState()
{
    QAndroidParcel musicIndexData, musicPositionData, playData;
    musicIndexData.writeVariant(m_playlist.currentIndex());
    m_serverBinder.transact(MessageType::LOAD_MUSIC_INDEX, musicIndexData);
    musicPositionData.writeVariant(m_mediaPlayer.position());
    m_serverBinder.transact(MessageType::POSITION_CHANGED, musicPositionData);
    playData.writeVariant(m_mediaPlayer.state() == QMediaPlayer::PlayingState);
    m_serverBinder.transact(MessageType::PLAY, playData);
}

const QAndroidBinder &Player::getServerBinder() const
{
    return m_serverBinder;
}

bool Player::getPlay() const
{
    return m_mediaPlayer.state() == QMediaPlayer::PlayingState;
}

int Player::musicCount() const
{
    return m_playlist.mediaCount();
}

bool Player::isStopped() const
{
    return m_mediaPlayer.state() == QMediaPlayer::StoppedState;
}

void Player::setPlay(bool play)
{
    if (play)
        m_mediaPlayer.play();
    else
        m_mediaPlayer.pause();

   m_javaService.callMethod<void>("setPlay", "(Z)V", play);
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

void Player::setMusicIndex(int index, bool update)
{
    if (m_shuffle)
        m_generateRandomIndex = false;
    m_updateMusicIndexOnce = update;
    m_playlist.setCurrentIndex(index);
}

void Player::setServerBinder(const QAndroidBinder &serverBinder)
{
    m_serverBinder = serverBinder;
}

void Player::onCurrentIndexChanged(int /*index*/)
{
    if (!m_shuffle || m_playlist.mediaCount() == 1) {
        if (m_updateMusicIndexOnce)
            sendChangedIndex();
        else
            m_updateMusicIndexOnce = true;
        return;
    }

    if (!m_generateRandomIndex) {
        if (m_updateMusicIndexOnce)
            sendChangedIndex();
        else
            m_updateMusicIndexOnce = true;
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
