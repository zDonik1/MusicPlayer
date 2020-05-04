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

        property int currentSectionIndex: 0

        id: listFile
        scrollIndicatorVisible: true
//        model: fileModel
        model: dataModel.dirModel

        delegate: FileDelegate {
            property string dir: rDir

            text: rFilename
            animDuration: 200

            onOptionsClicked: console.log(index + " options clicked")
        }

        section {
            property: "rDir"
            delegate: DirDelegate {
                text: section

                onOpenChanged: {
                    for (var i = 0; i < listFile.count; ++i)
                        if (listFile.itemAtIndex(i).dir === section)
                            listFile.itemAtIndex(i).open = open
                }
            }
        }
    }
}
