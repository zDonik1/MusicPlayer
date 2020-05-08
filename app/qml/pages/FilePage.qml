import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    title: qsTr("Files")

    AppListView {
        id: listFile
        scrollIndicatorVisible: true
        model: dataModel.dirModel

        delegate: FileBaseDelegate {
            icon: r_isDir
                  ? (open ? IconType.folderopen : IconType.folder)
                  : IconType.fileaudioo
            text: r_name
            height: r_isDir ? dp(55) : (open ? dp(55) : 0)
            visible: r_isDir ? true : height !== 0
            backgroundColor: r_isDir ? Qt.lighter(Qt.lighter(Theme.tintColor))
                                     : Theme.backgroundColor
            open: r_isOpen

            onClicked: if (r_isDir)
                           logic.dirToggled(index)
        }
    }
}
