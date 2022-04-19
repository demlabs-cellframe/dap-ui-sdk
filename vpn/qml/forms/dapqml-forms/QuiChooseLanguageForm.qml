import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "ChooseLanguage"

    /* signals */
    signal sigSelect(int index, string name);

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
        qss: "radiobtn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }


    /* listview */
    ListView {
        id: csListView

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        model: pseudoListModel

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height
            property bool checked: false

            DapQmlRadioButton {
                text: model.name
                checked: parent.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
                onClicked: root.sigSelect (model.index, model.name)
            }
        }
    }
}
