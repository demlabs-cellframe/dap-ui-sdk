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
        id: serialModel
        ListElement {
            key: "GTSF-WWRM-KLMWQ-OWWM"
        }
        ListElement {
            key: "GTSF-WWRM-KLMWQ-OWWM"
        }
        ListElement {
            key: "GTSF-WWRM-KLMWQ-OWWM"
        }
        ListElement {
            key: "GTSF-WWRM-KLMWQ-OWWM"
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
        objectName: "purchaseHistoryListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width // root.width - 72
        height: root.height - y
        clip: true

        model: serialModel

        delegate: DapQmlButton {
            buttonStyle: DapQmlButton.IconMainSub
            mainText: model.key
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
