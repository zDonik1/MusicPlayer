import QtQuick 2.0
import Felgo 3.0

FileBaseDelegate {
    property int animDuration

    id: root
    icon: IconType.fileaudioo
    visible: open
    state: open ? "opened" : "closed"

    states: [
        State {
            name: "opened"
            PropertyChanges {
                target: root
                height: dp(55)
            }
            PropertyChanges {
                target: iconItem
                size: dp(20)
            }
            PropertyChanges {
                target: textItem
                fontSize: Theme.listItem.fontSizeText
            }
        },
        State {
            name: "closed"
            PropertyChanges {
                target: root
                height: 0
            }
            PropertyChanges {
                target: iconItem
                size: 0
            }
            PropertyChanges {
                target: textItem
                fontSize: 0
            }
        }
    ]

    onClicked: root.optionsClicked()

    Behavior on height {
        NumberAnimation {
            duration: animDuration
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on iconItem.size {
        NumberAnimation {
            duration: animDuration
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on textItem.fontSize {
        NumberAnimation {
            duration: animDuration
            easing.type: Easing.InOutQuad
        }
    }
}
