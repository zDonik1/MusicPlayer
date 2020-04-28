import QtQuick 2.0
import Felgo 3.0

FileBaseDelegate {
    property alias animDuration: anim.duration

    id: root
    icon: IconType.fileaudioo

    states: [
        State {
            name: "opened"
            PropertyChanges {
                target: root
                height: dp(55)
            }
        },
        State {
            name: "closed"
            PropertyChanges {
                target: root
                height: 0
            }
        }
    ]

    onClicked: root.optionsClicked()

    Behavior on height {
        NumberAnimation {
            id: anim
            easing.type: Easing.InOutQuad
        }
    }
}
