import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    function timeToText(time) {
        let seconds = time / 1000
        let minutes = Math.floor(seconds / 60)
        seconds = Math.floor(seconds % 60)
        return minutes + ":" + (seconds < 10 ? "0" : "") + seconds
    }

    id: root
    title: dataModel.currentPlaylistName === ""
           ? qsTr("Music Player")
           : dataModel.currentPlaylistName

    Component.onCompleted: logic.playerPageLoaded()

    AppListView {
        property real delegateHeight: dp(68)
        property int currentMusic: dataModel.musicModel.currentMusic

        id: listView
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
            bottom: controllerColumn.top
        }
        backgroundColor: Theme.backgroundColor
        model: dataModel.musicModel
        highlight: componentHighlight
        highlightFollowsCurrentItem: false

        delegate: MusicDelegate {
            height: listView.delegateHeight
            backgroundColor: "transparent"
            title: r_title
            duration: timeToText(r_duration)
            image: "image://musicImage/" + index
            isDefaultImage: r_isDefaultImage

            onClicked: {
                listView.currentIndex = index
                logic.musicChanged(index)
            }
        }
    }

    Component {
        id: componentHighlight

        Rectangle {
            y: listView.itemAtIndex(listView.currentMusic).y
            width: listView.width
            height: listView.delegateHeight
            color: listView.currentMusic === -1
                   ? "transparent"
                   : Theme.listItem.selectedBackgroundColor
        }
    }

    Connections {
        target: dataModel

        onImagesReady: {
            let iMax = listView.count
            for (let i = 0; i < iMax; ++i) {
                listView.itemAtIndex(i).reloadImage()
            }
        }
    }

    AppButton {
        // '0' - no playlist, '1' - no music, '-1' - no intent
        property int intent: dataModel.musicModel.currentPlaylist === -1
                             ? 0 : (listView.count === 0 ? 1 : -1)

        anchors.centerIn: parent
        text: intent === 0
              ? qsTr("Select a playlist")
              : (intent === 1
                 ? qsTr("Add music")
                 : "")
        flat: true
        visible: intent !== ""
        enabled: visible

        onClicked: if (intent === 0)
                       navigation.currentIndex = 1
                   else
                       navigation.currentIndex = 2
    }

    Column {
        id: controllerColumn
        anchors {
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }

        AppSlider {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.9
            from: 0
            to: dataModel.musicDuration
            value: dataModel.musicPosition

            onMoved: logic.seek(position)
        }

        Row {
            AppButton {
                icon: IconType.random
                flat: true
                minimumWidth: root.width / 5
                textColor: dataModel.isShuffle ? Theme.tintColor
                                               : Theme.disabledColor

                onClicked: logic.shuffle()
            }

            AppButton {
                icon: IconType.stepbackward
                flat: true
                minimumWidth: root.width / 5

                onClicked: logic.previous()
            }

            AppButton {
                icon: dataModel.isPlaying ? IconType.pause : IconType.play
                flat: true
                minimumWidth: root.width / 5

                onClicked: logic.play()
            }

            AppButton {
                icon: IconType.stepforward
                flat: true
                minimumWidth: root.width / 5

                onClicked: logic.next()
            }

            AppButton {
                icon: IconType.undo
                flat: true
                minimumWidth: root.width / 5
                textColor: dataModel.isRepeat ? Theme.tintColor
                                              : Theme.disabledColor

                onClicked: logic.repeat()
            }
        }
    }
}
