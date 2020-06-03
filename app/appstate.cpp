#include "appstate.h"

#include <QDebug>

#include "settingsdao.h"

AppState::AppState(SettingsDAO &settings)
    : m_settings(settings)
{
}

void AppState::load()
{
    QVariant currentPlaylistVariant = m_settings.getValue("current_playlist");
    if (currentPlaylistVariant.isValid()) {
        m_currentPlaylistIndex = currentPlaylistVariant.toInt();
        m_currentPlaylistName = m_settings
                .getValue("current_playlist_name").toString();

        QVariant currentMusicVariant = m_settings
                .getValue("current_music_index");
        if (currentMusicVariant.isValid())
            m_currentMusicIndex = currentMusicVariant.toInt();

        m_currentMusicDuration = m_settings
                .getValue("current_music_duration").toLongLong();
        m_currentMusicPosition = m_settings
                .getValue("current_music_position").toLongLong();
    }

    m_shuffle = m_settings.getValue("shuffle").toBool();
    m_repeat = m_settings.getValue("repeat").toBool();
}

void AppState::save()
{
    m_settings.storeValue("current_playlist", m_currentPlaylistIndex);
    m_settings.storeValue("current_playlist_name", m_currentPlaylistName);
    m_settings.storeValue("current_music_index", m_currentMusicIndex);
    m_settings.storeValue("current_music_duration", m_currentMusicDuration);
    m_settings.storeValue("current_music_position", m_currentMusicPosition);
    m_settings.storeValue("shuffle", m_shuffle);
    m_settings.storeValue("repeat", m_repeat);
}

int AppState::getCurrentPlaylistIndex() const
{
    return m_currentPlaylistIndex;
}

const QString &AppState::getCurrentPlaylistName() const
{
    return m_currentPlaylistName;
}

int AppState::getCurrentMusicIndex() const
{
    return m_currentMusicIndex;
}

qint64 AppState::getCurrentMusicDuration() const
{
    return m_currentMusicDuration;
}

qint64 AppState::getCurrentMusicPosition() const
{
    return m_currentMusicPosition;
}

bool AppState::isPlaying() const
{
    return m_isPlaying;
}

bool AppState::getShuffle() const
{
    return m_shuffle;
}

bool AppState::getRepeat() const
{
    return m_repeat;
}

void AppState::setCurrentPlaylistIndex(int index)
{
    m_currentPlaylistIndex = index;
    emit currentPlaylistIndexChanged();
}

void AppState::setCurrentPlaylistName(const QString &name)
{
    m_currentPlaylistName = name;
    emit currentPlaylistNameChanged();
}

void AppState::setCurrentMusicIndex(int index)
{
    m_currentMusicIndex = index;
    emit currentMusicIndexChanged();
}

void AppState::setCurrentMusicDuration(int64_t duration)
{
    m_currentMusicDuration = duration;
    emit currentMusicDurationChanged();
}

void AppState::setCurrentMusicPosition(int64_t position)
{
    m_currentMusicPosition = position;
    emit currentMusicPositionChanged();
}

void AppState::setIsPlaying(bool playing)
{
    m_isPlaying = playing;
    emit isPlayingChanged();
}

void AppState::setShuffle(bool shuffle)
{
    m_shuffle = shuffle;
    emit shuffleChanged();
}

void AppState::setRepeat(bool repeat)
{
    m_repeat = repeat;
    emit repeatChanged();
}
