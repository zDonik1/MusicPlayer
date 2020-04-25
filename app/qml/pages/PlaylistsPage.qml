import QtQuick 2.0
import Felgo 3.0

Page {
    title: qsTr("Playlists")

    ListModel {
        id: playlistModel

        ListElement {
            playlistTitle: "Favorite"
            songCount: "12"
        }
        ListElement {
            playlistTitle: "NEFFEX"
            songCount: "15"
        }
        ListElement {
            playlistTitle: "TheFatRat"
            songCount: "23"
        }
    }

    AppListView {
        anchors.fill: parent
        model: playlistModel

        delegate: SimpleRow {
            text: playlistTitle
            detailText: "Songs: " + songCount
        }
    }
}
