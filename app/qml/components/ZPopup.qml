import QtQuick 2.0
import Felgo 3.0

Item {
    function open() { openAnim.start() }
    function close() { closeAnim.start() }

    default property alias contentData: itemContent.data
    property alias contentItem: itemContent
    property alias title: textTitle.text
    property int openHeight: dp(450)
    property int closedHeight: openHeight / 2
    property int openWidth: dp(280)
    property int closedWidth: openWidth / 2

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
        height: closedHeight
        width: closedWidth
        radius: dp(8)
        color: Theme.backgroundColor
        opacity: 0
        visible: opacity > 0

        // catches any unwanted taps in the popup
        MouseArea {
            anchors.fill: parent
        }

        Rectangle {
            id: navBar
            anchors.top: parent.top
            width: parent.width
            height: dp(60)
            radius: popup.radius
            color: Qt.lighter(Qt.lighter(Theme.tintColor))

            AppText {
                id: textTitle
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: dp(15)
                }
                color: Theme.tintColor
                fontSize: 19
                font.bold: true
            }
        }

        Item {
            id: itemContent
            anchors {
                top: navBar.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
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
            value: closedHeight
        }
        PropertyAction {
            target: popup
            property: "width"
            value: closedWidth
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
            to: openHeight
            duration: 300
            easing.type: Easing.OutBack
        }
        NumberAnimation {
            target: popup
            property: "width"
            to: openWidth
            duration: 300
            easing.type: Easing.OutBack
        }
    }
}
