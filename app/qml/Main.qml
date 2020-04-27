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
        onMusicChanged: dataModel.musicChanged(index)
    }

    Navigation {
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
            title: qsTr("Files")
            icon: IconType.file

            NavigationStack {
                FilePage {
                }
            }
        }
    }
}
