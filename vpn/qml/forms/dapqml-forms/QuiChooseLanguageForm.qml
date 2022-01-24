import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root

    signal close()

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Language"
        qss: "dialog-title"
    }

    /* model */
    ListModel {
        id: pseudoListModel
        ListElement {
            name: "English"
        }
        ListElement {
            name: "Russian"
        }
        ListElement {
            name: "Українська"
        }
        ListElement {
            name: "中文"
        }
    }

    /* resizer */
    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "chooselanguage-btn-resizer"
    }

    /* listview */
    ListView {
        id: csListView

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: root.width - 72
        height: root.height - y
        clip: true

        model: pseudoListModel

        delegate: DapQmlRadioButton {
            text: model.name
            separator: true
            iconSize: resizer.height
            width: resizer.width
            height: resizer.height
        }
    }
}
