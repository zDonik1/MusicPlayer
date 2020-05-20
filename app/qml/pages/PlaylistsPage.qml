import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    property int editedPlaylistIndex

    title: qsTr("Playlists")

    rightBarItem: IconButtonBarItem {
        icon: IconType.plus

        onClicked: dialogAdd.open()
    }

    AppListView {
        anchors.fill: parent
        model: dataModel.playlistModel

        delegate: PlaylistDelegate {
            name: r_name
            description: "Songs: " + r_songCount

            onEditClicked: {
                editedPlaylistIndex = index
                dialogEdit.contentText = name
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
            contentText = ""
        }
    }

    PlaylistDialog {
        id: dialogEdit
        title: qsTr("Edit Playlist")

        onAccepted: {
            close()
            logic.playlistEdited(editedPlaylistIndex, contentText)
            contentText = ""
        }
    }
}
