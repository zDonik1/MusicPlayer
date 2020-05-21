import QtQuick 2.0
import Felgo 3.0

import "../components"

ListPage {
    title: qsTr("Delete music root folder")

    model: dataModel.rootDirModel

    delegate: FileBaseDelegate {
        icon: IconType.folder
        text: r_path
        optionsButtonIcon: IconType.minus
        textItem.elide: Text.ElideMiddle

        onClicked: {
            logic.musicRootDirDeleted(index)
            navigationStack.pop()
        }

        onOptionsClicked: clicked()
    }
}
