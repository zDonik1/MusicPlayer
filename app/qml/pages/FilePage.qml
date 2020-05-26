import QtQuick 2.0
import QtQuick.Controls 2.12
import Felgo 3.0

import "../components"

Page {
    id: rootPage
    title: qsTr("Files")

    rightBarItem: NavigationBarRow {
        IconButtonBarItem {
            icon: IconType.minus

            onClicked: navigationStack.push(rootDirsPage)
        }

        IconButtonBarItem {
            icon: IconType.plus

            onClicked: navigationStack.push(filesystemPage)
        }
    }

    AppListView {
        id: listFile
        scrollIndicatorVisible: true
        model: dataModel.dirModel

        delegate: FileBaseDelegate {
            icon: r_isDir
                  ? (open ? IconType.folderopen : IconType.folder)
                  : IconType.fileaudioo
            text: r_name
            height: r_isDir ? dp(55) : (open ? dp(55) : 0)
            visible: r_isDir ? true : height !== 0
            backgroundColor: r_isDir ? Qt.lighter(Qt.lighter(Theme.tintColor))
                                     : Theme.backgroundColor
            open: r_isOpen
            optionsButtonIcon: IconType.ellipsisv

            onClicked: if (r_isDir)
                           logic.dirToggled(index)
                       else
                           optionsClicked()

            onOptionsClicked: if (r_isDir)
                                  menuDir.open()
                              else
                                  menuFile.open()

            DropdownMenu {
                id: menuDir
                width: dp(200)
                x: rootPage.width

                Action {
                    text: qsTr("Add to playlist")

                    onTriggered: {
                        menuDir.close()
                        playlistPopupAdd.fileIndex = index
                        playlistPopupAdd.isAddingDir = true
                        playlistPopupAdd.open()
                    }
                }
            }

            DropdownMenu {
                id: menuFile
                width: dp(200)
                x: rootPage.width

                Action {
                    text: qsTr("Add to playlist")

                    onTriggered: {
                        menuDir.close()
                        playlistPopupAdd.fileIndex = index
                        playlistPopupAdd.isAddingDir = false
                        playlistPopupAdd.open()
                    }
                }

                Action {
                    text: qsTr("Delete from memory")

                    onTriggered: {
                        menuFile.close()
                        logic.deleteMusicFromMemory(index)
                    }
                }
            }
        }
    }

    PlaylistPopup {
        property int fileIndex: -1
        property bool isAddingDir: false

        id: playlistPopupAdd

        onPlaylistClicked: {
            if (isAddingDir)
                logic.addDirToPlaylist(fileIndex, index)
            else
                logic.addMusicToPlaylist(fileIndex, index)
            close()
        }
    }

    Component {
        id: rootDirsPage

        RootDirsPage {
        }
    }

    Component {
        id: filesystemPage

        FilesystemPage {
        }
    }
}
