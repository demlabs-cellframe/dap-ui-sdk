import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "ChooseServer"
    property var items: new Array

    /* signals */
    signal sigSelect(int index, string name);

    /* functions */
    function setCurrentIndex(a_index) {
        csListView.currentIndex = a_index;

        var count           = csListView.count
        var current         = csListView.currentIndex;
        for(var i = 0; i < count; i++) {
            var entry       = csListView.itemAtIndex(i);
            entry.checked   = (i === current)
        }
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
        objectName: "chooseServerListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

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
                Component.onCompleted: { items.push(this); }
            }
        }
    }
}
