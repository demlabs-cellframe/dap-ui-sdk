import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root

    /* signals */
    signal sigSelect(int index, string name);

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Choose server"
        qss: "dialog-title"
    }

    /* model */
    ListModel {
        id: pseudoListModel
        ListElement {
            name: "auto"
        }
        ListElement {
            name: "USA"
        }
        ListElement {
            name: "Finland"
        }
        ListElement {
            name: "Germany"
        }
    }

    /* resizer */
    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "chooseserver-btn-resizer"
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
            onClicked: root.sigSelect (model.index, model.name)
        }
    }
}
