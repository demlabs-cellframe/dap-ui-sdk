import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "ChooseServer"

    /* signals */
    signal sigSelect(int index, string name);

    /* functions */
    function setCurrentIndex(a_index) {
        csListView.currentIndex = a_index;
    }

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Choose server"
        qss: "dialog-title"
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
        objectName: "chooseServerListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: root.width - 72
        height: root.height - y
        clip: true

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
