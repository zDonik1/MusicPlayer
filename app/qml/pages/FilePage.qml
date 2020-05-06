import QtQuick 2.0
import Felgo 3.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "../components"

Page {
    title: qsTr("Files")

    TreeView {
        property int currentSectionIndex: 0

        id: treeFile
        anchors.fill: parent
        model: dataModel.dirModel

        itemDelegate: FileBaseDelegate {
            property bool isFolder: styleData.depth === 0
            icon: isFolder
                  ? (styleData.isExpanded
                     ? IconType.folderopen
                     : IconType.folder)
                  : IconType.fileaudioo
            text: styleData.value

            onClicked: {
                if (isFolder) {
                    if (styleData.isExpanded)
                        treeFile.collapse(styleData.index)
                    else
                        treeFile.expand(styleData.index)
                }
            }
        }

        rowDelegate: Item {
            height: dp(55)
        }

        headerDelegate: Item {
            height: 0
            visible: false
        }

        TableViewColumn {
            title: "Filename"
            role: "rDisplay"
            width: parent.width
        }

        TreeViewStyle {
            control: parent

            branchDelegate: Item {
                visible: false
            }
        }
    }
}
