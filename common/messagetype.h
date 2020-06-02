#pragma once

enum MessageType
{
    BINDER = 1,
    DEBUG,

    LOAD_PLAYLIST,
    LOAD_MUSIC_INDEX,
    POSITION_CHANGED,
    MUSIC_ADDED,

    PLAY,
    NEXT,
    PREVIOUS,
    SEEK,
    SHUFFLE,
    REPEAT,
    MUSIC_CHANGED,
};
