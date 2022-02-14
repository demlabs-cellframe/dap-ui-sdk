import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "PurchaseHistory"

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "History"
        qss: "dialog-title"
    }

    /* model */
    ListModel {
        id: reportModel
        ListElement {
            name: "GTSF-WWRM-KLMWQ-OWWM"
        }
        ListElement {
            name: "GTSF-WWRM-KLMWQ-OWWM"
        }
        ListElement {
            name: "GTSF-WWRM-KLMWQ-OWWM"
        }
        ListElement {
            name: "GTSF-WWRM-KLMWQ-OWWM"
        }
    }

    /* resizer */
    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "ph-btn-resizer"
    }

    DapQmlRectangle {
        id: icnResizer
        visible: false
        qss: "ph-icn-resizer"
    }

    /* listview */
    ListView {
        id: phListView

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width // root.width - 72
        height: root.height - y
        clip: true

        model: reportModel

        delegate: DapQmlButton {
            buttonStyle: DapQmlButton.IconMainSub
            mainText: model.name
            subText: ""
            mainQss: "ph-btn-label-main"
            subQss: "ic_copy"
            icon: "ic_key-item"
            separator: true
            iconSize: icnResizer.height
            width: resizer.width
            height: resizer.height
        }
    }
}
