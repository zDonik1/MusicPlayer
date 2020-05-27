import QtQuick 2.0

Item {
    signal play()
    signal next()
    signal previous()
    signal seek(real position)
    signal shuffle()
    signal repeat()
    signal musicChanged(int index)

    signal playlistSelected(int index)
    signal playlistAdded(string name)
    signal playlistEdited(int index, string name)
    signal playlistDeleted(int index)

    signal dirToggled(int index)
    signal addDirToPlaylist(int dirIndex, int playlistIndex)
    signal addMusicToPlaylist(int musicIndex, int playlistIndex)
    signal deleteMusicFromMemory(int index)

    signal musicRootDirAdded(url path)
    signal musicRootDirDeleted(int index)
}
