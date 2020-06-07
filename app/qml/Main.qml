import Felgo 3.0
import QtQuick 2.0

import "pages"

App {
    onInitTheme: {
        Theme.appButton.minimumHeight = 50
        Theme.appButton.flatMinimumWidth = width / 5
        Theme.appButton.horizontalMargin = 0
        Theme.appButton.verticalMargin = 0
        Theme.navigationAppDrawer.textColor = "#404040"
        Theme.colors.secondaryTextColor = "#909090"
        Theme.listItem.selectedBackgroundColor = Qt.lighter(Qt.lighter(Theme.tintColor))
    }

    Logic {
        id: logic

        onPlay: dataModel.play()
        onNext: dataModel.next()
        onPrevious: dataModel.previous()
        onSeek: dataModel.seek(position)
        onShuffle: dataModel.shuffle()
        onRepeat: dataModel.repeat()
        onMusicChanged: dataModel.changeMusic(index)

        onPlaylistSelected: dataModel.selectPlaylist(index)
        onPlaylistAdded: dataModel.addPlaylist(name)
        onPlaylistEdited: dataModel.editPlaylist(index, name)
        onPlaylistDeleted: dataModel.deletePlaylist(index)

        onDirToggled: dataModel.toggleDir(index)
        onDirToPlaylistAdded: dataModel.addDirToPlaylist(dirIndex, playlistIndex)
        onMusicToPlaylistAdded: dataModel.addMusicToPlaylist(musicIndex, playlistIndex)
        onMusicFromMemoryDeleted: dataModel.addMusicToPlaylist(index)

        onMusicRootDirAdded: dataModel.addMusicRootDir(path)
        onMusicRootDirDeleted: dataModel.deleteMusicRootDir(index)

        onTablesDropped: dataModel.dropTables()
    }

    Navigation {
        id: navigation

        NavigationItem {
            title: qsTr("Player")
            icon: IconType.play

            NavigationStack {
                PlayerPage {
                }
            }
        }

        NavigationItem {
            title: qsTr("Playlists")
            icon: IconType.thlist

            NavigationStack {
                PlaylistsPage {
                }
            }
        }

        NavigationItem {
            id: navItemFile
            title: qsTr("Files")
            icon: IconType.file

            onSelected: dataModel.refreshDirs()

            NavigationStack {
                transitionDelegate: transitionDelegateiOS

                FilePage {
                }
            }
        }

        NavigationItem {
            title: qsTr("Dev tools")
            icon: IconType.codepen

            NavigationStack {
                Page {
                    title: qsTr("Dev tools")

                    Column {
                        anchors.centerIn: parent

                        AppButton {
                            text: qsTr("Drop all tables")

                            onClicked: logic.tablesDropped()
                        }
                    }
                }
            }
        }
    }
}
