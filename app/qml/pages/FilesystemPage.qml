import QtQuick 2.0
import QtQuick.Controls 2.12
import Felgo 3.0
import Qt.labs.folderlistmodel 2.12
import Qt.labs.platform 1.1

import "../components"

Page {
    id: root
    title: qsTr("Add music root folder")
    backNavigationEnabled: true

    rightBarItem: IconButtonBarItem {
        icon: IconType.levelup

        onClicked: folderModel.folder = folderModel.parentFolder
    }

    Rectangle {
        id: pathContainer
        anchors.top: parent.top
        height: dp(40)
        width: parent.width
        color: Qt.lighter(Qt.lighter(Theme.tintColor))

        AppText {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            leftPadding: dp(15)
            rightPadding: dp(15)
            text: folderModel.folder.toString().slice(7)
            elide: Text.ElideMiddle
            wrapMode: Text.NoWrap
        }

        Rectangle {
            anchors.bottom: parent.bottom
            height: 1
            width: parent.width
            color: Theme.tintColor
        }
    }

    AppListView {
        id: listView
        anchors {
            top: pathContainer.bottom
            bottom: parent.bottom
        }
        model: folderModel

        delegate: FileBaseDelegate {
            icon: IconType.folder
            text: fileName
            optionsButtonIcon: IconType.plus

            onClicked: {
                folderModel.folder = fileURL
            }
            onOptionsClicked: {
                logic.musicRootDirAdded(fileURL)
                navigationStack.pop()
            }
        }

        FolderListModel {
            id: folderModel
            showFiles: false
            // 11 - generic data location
            folder: StandardPaths.standardLocations(11)[0]
        }
    }
}
