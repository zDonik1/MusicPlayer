import QtQuick 2.0
import Felgo 3.0

Item {
    signal clicked()
    signal optionsClicked()

    property alias icon: icon.icon
    property alias text: textName.text
    property alias backgroundColor: background.color
    property alias iconItem: icon
    property alias textItem: textName
    property bool open: true
    property alias optionsButtonIcon: optionsButtonIcon.icon
    property alias optionButtonColor: optionsButtonIcon.color
    property alias hasOptionsButton: menuButtonContainer.visible

    id: root
    width: parent.width
    height: dp(55)
    clip: true

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
        wrapMode: Text.NoWrap
        fontSize: Theme.listItem.fontSizeText
        elide: Text.ElideRight
    }

    RippleMouseArea {
        anchors.fill: parent
        circularBackground: false

        onClicked: root.clicked()
    }

    Item {
        id: menuButtonContainer
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        width: height
        enabled: visible

        Icon {
            id: optionsButtonIcon
            anchors.centerIn: parent
            icon: IconType.cc
            color: Theme.tintColor
        }

        RippleMouseArea {
            anchors.fill: parent
            radius: dp(30)
            touchPoint: Qt.point(width / 2, height / 2)

            onClicked: root.optionsClicked()
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Theme.dividerColor
    }
}
