import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    title: qsTr("Music Player")

    ListModel {
        id: musicModel

        ListElement {
            musicTitle: "Tommee Profitt - Enemy (feat. Beacon Light & Sam Tinnesz)"
            musicImage: ""
            musicDuration: "3:30"
        }
        ListElement {
            musicTitle: "Generdyn - Destiny (feat. Krigare)"
            musicImage: ""
            musicDuration: "4:34"
        }
        ListElement {
            musicTitle: "KLOUD - Humans (Far Out Remix)"
            musicImage: ""
            musicDuration: "3:41"
        }
        ListElement {
            musicTitle: "BlackGummy - Superhuman (feat. Colleen D'Agostino) (BlackStation Remix)"
            musicImage: ""
            musicDuration: "3:20"
        }
    }

    AppListView {
        property int selected: -1

        id: listView
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
            bottom: controllerColumn.top
        }
        model: musicModel

        delegate: MusicDelegate {
            backgroundColor: index === listView.selected
                             ? Theme.listItem.selectedBackgroundColor
                             : Theme.backgroundColor

            onClicked: {
                listView.selected = index
                logic.musicChanged(index)
            }
        }
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
                textColor: Theme.disabledColor

                onClicked: logic.shuffle()
            }

            AppButton {
                icon: IconType.stepbackward
                flat: true

                onClicked: logic.previous()
            }

            AppButton {
                icon: IconType.play
                flat: true

                onClicked: logic.play()
            }

            AppButton {
                icon: IconType.stepforward
                flat: true

                onClicked: logic.next()
            }

            AppButton {
                icon: IconType.undo
                flat: true
                textColor: Theme.disabledColor

                onClicked: logic.repeat()
            }
        }
    }
}
