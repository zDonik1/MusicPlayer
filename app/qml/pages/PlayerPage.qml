import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    id: root
    title: qsTr("Music Player")

    AppListView {
        property int selected: -1

        id: listView
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
            bottom: controllerColumn.top
        }
        model: dataModel.musicModel

        delegate: MusicDelegate {
            backgroundColor: index === listView.selected
                             ? Theme.listItem.selectedBackgroundColor
                             : Theme.backgroundColor
            title: r_title
            duration: ""
            image: ""

            onClicked: {
                listView.selected = index
                logic.musicChanged(index)
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

            onMoved: logic.seek(position)
        }

        Row {
            AppButton {
                icon: IconType.random
                flat: true
                minimumWidth: root.width / 5
                textColor: Theme.disabledColor

                onClicked: logic.shuffle()
            }

            AppButton {
                icon: IconType.stepbackward
                flat: true
                minimumWidth: root.width / 5

                onClicked: logic.previous()
            }

            AppButton {
                icon: IconType.play
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
                textColor: Theme.disabledColor

                onClicked: logic.repeat()
            }
        }
    }
}
