import QtQuick 2.0
import Felgo 3.0

import "../components"

Page {
    title: qsTr("Files")

    ListModel {
        id: fileModel
        ListElement {
            dir: "somedir"
            filename: "my first music"
        }
        ListElement {
            dir: "somedir"
            filename: "my second music"
        }
        ListElement {
            dir: "somedir"
            filename: "my third music"
        }
        ListElement {
            dir: "otherdir"
            filename: "favorite one"
        }
        ListElement {
            dir: "otherdir"
            filename: "favorite two"
        }
    }
    AppListView {
        property string dirToggled: ""
        property int closeDuration: 200

        onDirToggledChanged: resetTimer.start()

        id: listFile
        model: dataModel.dirModel
//        model: fileModel

        delegate: FileDelegate {
            text: filename
            visible: height !== 0
            animDuration: listFile.closeDuration

            onOptionsClicked: console.log("options opened")

            Connections {
                target: listFile

                onDirToggledChanged:
                    if (listFile.dirToggled === dir)
                        state = (state === "closed") ? "opened" : "closed"
            }
        }

        section {
            property: "dir"
            delegate: DirDelegate {
                text: section

                onOpenChanged: listFile.dirToggled = section
            }
        }
    }

    Timer {
        id: resetTimer
        interval: listFile.closeDuration

        onTriggered: listFile.dirToggled = ""
    }
}
