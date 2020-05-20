import QtQuick 2.0
import QtQuick.Controls 2.12
import Felgo 3.0

Item {
    signal clicked()
    signal editClicked()
    signal deleteClicked()

    property alias name: textName.text
    property alias description: textDescription.text

    id: root
    height: dp(60)
    width: parent.width

    Item {
        anchors {
            left: parent.left
            leftMargin: dp(15)
            right: buttonContainer.left
            verticalCenter: parent.verticalCenter
        }
        height: textName.height + textDescription.height

        AppText {
            id: textName
        }

        AppText {
            id: textDescription
            anchors {
                top: textName.bottom
                topMargin: dp(3)
            }
            color: Theme.listItem.detailTextColor
            fontSize: Theme.listItem.fontSizeDetailText
        }
    }

    RippleMouseArea {
        anchors.fill: parent
        circularBackground: false

        onClicked: root.clicked()
    }

    Item {
        id: buttonContainer
        anchors {
            right: parent.right
        }
        height: parent.height
        width: height

        IconButton {
            anchors.centerIn: parent
            icon: IconType.ellipsisv
            color: Theme.tintColor

            onClicked: menu.open()
        }

        DropdownMenu {
            id: menu

            Action {
                text: qsTr("Edit")

                onTriggered: root.editClicked()
            }
            Action {
                text: qsTr("Delete")

                onTriggered: root.deleteClicked()
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        color: Theme.dividerColor
        width: parent.width
        height: 1
    }
}
