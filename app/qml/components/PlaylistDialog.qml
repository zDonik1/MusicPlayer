import QtQuick 2.0
import Felgo 3.0

Dialog {
    property alias contentText: textField.text

    id: root

    onCanceled: {
        contentText = ""
        close()
    }

    AppTextField {
        id: textField
        anchors.centerIn: parent
        width: parent.width * 0.8

        onActiveFocusChanged: {
            if (activeFocus)
                root.y = root.y - dp(150)
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
}
