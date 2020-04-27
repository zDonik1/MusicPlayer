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
        id: listFile
//        model: dataModel.dirModel
        model: fileModel

        delegate: FileDelegate {
            text: filename
        }

        section {
            property: "dir"
            delegate: DirDelegate {
                text: section
            }
        }
    }
}
