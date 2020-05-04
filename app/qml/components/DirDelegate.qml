import QtQuick 2.0
import Felgo 3.0

FileBaseDelegate {
    icon: open ? IconType.folderopeno : IconType.foldero
    backgroundColor: Qt.lighter(Qt.lighter(Theme.tintColor))

    onClicked: open = !open
}
