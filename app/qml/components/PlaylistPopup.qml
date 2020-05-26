import QtQuick 2.0
//import QtQuick.Controls 2.12
import Felgo 3.0

Item {
    signal playlistClicked(int index)

    function open() { openAnim.start() }
    function close() { closeAnim.start() }

    id: root
    anchors.fill: parent
    visible: popup.opacity > 0

    Rectangle {
        anchors.fill: parent
        color: "#55000000"

        MouseArea {
            anchors.fill: parent

            onClicked: root.close()
        }
    }

    Rectangle {
        id: popup
        anchors.centerIn: parent
        height: dp(200)
        width: dp(160)
        radius: dp(8)
        color: Theme.backgroundColor
        opacity: 0
        visible: opacity > 0

        Rectangle {
            id: navBar
            anchors.top: parent.top
            width: parent.width
            height: dp(60)
            radius: popup.radius
            color: Qt.lighter(Qt.lighter(Theme.tintColor))

            AppText {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: dp(15)
                }
                text: qsTr("Select playlist")
                color: Theme.tintColor
                fontSize: 19
                font.bold: true
            }
        }

        AppListView {
            id: listView
            anchors {
                top: navBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            model: dataModel.playlistModel

            delegate: PlaylistDelegate {
                name: r_name
                description: "Songs: " + r_musicCount
                hasOptionsButton: false

                onClicked: playlistClicked(index)
            }
        }
    }

    SequentialAnimation {
        id: closeAnim

        NumberAnimation {
            target: popup
            property: "opacity"
            to: 0
            duration: 200
            easing.type: Easing.InOutQuad
        }
        PropertyAction {
            target: popup
            property: "height"
            value: dp(200)
        }
        PropertyAction {
            target: popup
            property: "width"
            value: dp(160)
        }
    }

    ParallelAnimation {
        id: openAnim

        PropertyAction {
            target: popup
            property: "opacity"
            value: 1
        }
        NumberAnimation {
            target: popup
            property: "height"
            to: dp(450)
            duration: 300
            easing.type: Easing.OutBack
        }
        NumberAnimation {
            target: popup
            property: "width"
            to: dp(280)
            duration: 300
            easing.type: Easing.OutBack
        }
    }
}
