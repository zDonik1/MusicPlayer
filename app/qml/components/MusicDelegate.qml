import QtQuick 2.0
import QtGraphicalEffects 1.0
import Felgo 3.0

Item {
    signal clicked()

    property alias backgroundColor: background.color

    id: root
    width: parent.width
    height: dp(68)

    Rectangle {
        id: background
        anchors.fill: parent
        color: Theme.backgroundColor
    }

    Item {
        id: imageContainer
        anchors.left: parent.left
        height: parent.height
        width: height

        Rectangle {
            anchors.centerIn: parent
            height: parent.height - dp(15)
            width: height
            radius: dp(3)
            color: Qt.lighter(Theme.tintColor)
            clip: true

            Image {
                id: image

                anchors.centerIn: parent
                height: musicImage !== ""
                        ? parent.height : dp(30)
                width: height
                fillMode: Image.PreserveAspectCrop
                source: musicImage !== ""
                        ? musicImage : "../../assets/audiotrack.png"
                visible: false
            }

            ColorOverlay {
                anchors.fill: image
                color: "white"
                source: image
            }
        }
    }

    AppText {
        id: textTitle
        anchors {
            left: imageContainer.right
            leftMargin: dp(5)
            right: durationContainer.left
            verticalCenter: parent.verticalCenter
        }
        lineHeight: 1.2
        fontSize: Theme.listItem.fontSizeText
        maximumLineCount: 2
        elide: Text.ElideRight
        wrapMode: Text.Wrap
        text: musicTitle
    }

    Item {
        id: durationContainer
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        width: dp(40)

        AppText {
            anchors.horizontalCenter: parent.horizontalCenter
            color: Theme.secondaryTextColor
            y: dp(13)
            text: musicDuration
            fontSize: 14
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        color: Theme.dividerColor
        width: parent.width
        height: 1
    }

    RippleMouseArea {
        anchors.fill: parent
        circularBackground: false

        onClicked: root.clicked()
    }
}
