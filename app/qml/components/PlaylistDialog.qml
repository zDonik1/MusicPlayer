import QtQuick 2.0
import Felgo 3.0

ZPopup {
    function clearText() { textField.clear() }

    signal accepted()
    signal canceled()

    property alias contentText: textField.text

    id: root
    openHeight: dp(200)

    onCanceled: {
        clearText()
        close()
    }

    AppTextField {
        id: textField
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -dp(15)
        width: parent.width * 0.8

        onActiveFocusChanged: {
            if (activeFocus)
                root.y = root.y - dp(150)
        }
    }

    Row {
        anchors {
            right: parent.right
            bottom: parent.bottom
        }

        AppButton {
            text: qsTr("Cancel")
            flat: true

            onClicked: root.canceled()
        }

        AppButton {
            text: qsTr("Done")
            flat: true

            onClicked: root.accepted()
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
}
