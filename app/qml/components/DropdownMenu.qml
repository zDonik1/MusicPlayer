import QtQuick 2.0
import QtQuick.Controls 2.12
import Felgo 3.0

Menu {
    id: root
    transformOrigin: Popup.TopRight
    topPadding: dp(5)
    bottomPadding: dp(5)

    background: AppPaper {
        height: implicitContentHeight + root.topPadding + root.bottomPadding
        implicitWidth: dp(120)
    }

    delegate: MenuItem {
        id: menuItem
        implicitHeight: dp(45)
        padding: dp(20)

        contentItem: AppText {
            text: menuItem.text
        }

        background: Item {}

        RippleMouseArea {
            anchors.fill: parent
            circularBackground: false
            propagateComposedEvents: true

            onClicked: menuItem.action.triggered()
        }
    }

    enter: Transition {
        PropertyAction {
            property: "opacity"
            value: 1.0
        }

        NumberAnimation {
            property: "height"
            from: implicitHeight * 0.3
            to: implicitHeight
            duration: 200
            easing.type: Easing.OutCirc
        }

        NumberAnimation {
            property: "width"
            from: implicitWidth * 0.3
            to: implicitWidth
            duration: 200
            easing.type: Easing.OutCirc
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: 200
        }
    }
}
