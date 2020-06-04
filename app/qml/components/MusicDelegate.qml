import QtQuick 2.0
import QtGraphicalEffects 1.0
import Felgo 3.0

Item {
    function reloadImage() {
        let temp = image
        image = ""
        image = temp
    }

    signal clicked()

    property alias backgroundColor: background.color
    property alias title: textTitle.text
    property alias duration: textDuration.text
    property bool isDefaultImage: true
    property string image: ""
    property alias clickable: mouseArea.enabled

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
                id: imageMusic

                anchors.centerIn: parent
                height: isDefaultImage ? dp(30) : parent.height
                width: height
                fillMode: Image.PreserveAspectCrop
                source: isDefaultImage ? "../../assets/audiotrack.png" : image
                sourceSize: Qt.size(width, height)
                visible: false
                cache: false
            }

            ColorOverlay {
                anchors.fill: imageMusic
                color: isDefaultImage ? "white" : "transparent"
                source: imageMusic
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
            id: textDuration
            anchors.horizontalCenter: parent.horizontalCenter
            color: Theme.secondaryTextColor
            y: dp(13)
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
        id: mouseArea
        anchors.fill: parent
        circularBackground: false

        onClicked: root.clicked()
    }
}
