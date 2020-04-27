import QtQuick 2.0
import Felgo 3.0

Item {
    signal clicked()

    property alias icon: icon.icon
    property alias text: textName.text
    property alias backgroundColor: background.color

    id: root
    width: parent.width
    height: dp(55)

    Rectangle {
        id: background
        anchors.fill: parent
    }

    Item {
        id: imageContainer
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: height

        Icon {
            id: icon
            anchors.centerIn: parent
            color: Qt.darker(Theme.tintColor)
        }
    }

    AppText {
        id: textName
        anchors {
            left: imageContainer.right
            leftMargin: dp(5)
            right: menuButtonContainer.left
            verticalCenter: parent.verticalCenter
        }
        lineHeight: 1.2
        fontSize: Theme.listItem.fontSizeText
        elide: Text.ElideRight
    }

    Item {
        id: menuButtonContainer
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        width: height

        Icon {
            anchors.centerIn: parent
            icon: IconType.ellipsisv
            color: Theme.tintColor
        }

        RippleMouseArea {
            anchors.fill: parent
            radius: dp(30)
            touchPoint: Qt.point(width / 2, height / 2)

            onClicked: root.clicked()
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Theme.dividerColor
    }
}
