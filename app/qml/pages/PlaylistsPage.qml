import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    property int editedPlaylistIndex

    title: qsTr("Playlists")

    rightBarItem: IconButtonBarItem {
        icon: IconType.plus

        onClicked: {
            dialogAdd.y = -22;
            dialogAdd.open()
        }
    }

    AppListView {
        anchors.fill: parent
        model: dataModel.playlistModel
        emptyText.text: qsTr("No playlists added")

        delegate: PlaylistDelegate {
            name: r_name
            description: "Songs: " + r_musicCount

            onClicked: {
                logic.playlistSelected(index)
                navigation.currentIndex = 0
            }

            onEditClicked: {
                editedPlaylistIndex = index
                dialogEdit.contentText = name
                dialogEdit.y = -22;
                dialogEdit.open()
            }

            onDeleteClicked: logic.playlistDeleted(index)
        }
    }

    PlaylistDialog {
        id: dialogAdd
        title: qsTr("New Playlist")

        onAccepted: {
            close()
            logic.playlistAdded(contentText)
            clearText()
        }
    }

    PlaylistDialog {
        id: dialogEdit
        title: qsTr("Edit Playlist")

        onAccepted: {
            close()
            logic.playlistEdited(editedPlaylistIndex, contentText)
            clearText()
        }
    }
}
