import QtQuick 2.0
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0

Item {
    id: root

    signal close()

    /* model */
    ListModel {
        id: purchaseListModel
        ListElement {
            price: "$4.98"
            main: "1-month plan"
            sub: ""
        }
        ListElement {
            price: "$26.88"
            main: "6-month plan"
            sub: "$4.48 per month"
        }
        ListElement {
            price: "$35.88"
            main: "1-year plan"
            sub: "$2.99 per month"
        }
    }

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Get a new license key"
        qss: "dialog-title"
    }

    /* button resizer */
    DapQmlLabel {
        id: resizer
        visible: false
        qss: "purchase-btn-resizer"
    }

    /* list */
    ListView {
        id: purchaseListView
        y: title.y + title.height * 2
        height: root.height - y
        clip: true

        property string qss: "purchase-listview"
        DapQmlStyle { qss: purchaseListView.qss; item: purchaseListView }

        model: purchaseListModel

        delegate: DapQmlButton {
            width: resizer.width // purchaseListView.width
            height: resizer.height // 130
            buttonStyle: DapQmlButton.Style.LeftTopMainBottomSub
            frame: true
            leftText: model.price
            mainText: model.main
            subText: model.sub
        }
    }

//    DapQmlButton {
//        x: 19
//        y: 140
//        width: 390
//        height: 130
//        leftText: "$4.98"
//        mainText: "1-month plan"
//        subText: ""
//    }

//    DapQmlButton {
//        x: 19
//        y: 290
//        width: 390
//        height: 130
//        leftText: "$26.88"
//        mainText: "6-month plan"
//        subText: "$4.48 per month"
//    }

//    DapQmlButton {
//        x: 19
//        y: 440
//        width: 390
//        height: 130
//        leftText: "$35.88"
//        mainText: "1-year plan"
//        subText: "$2.99 per month"
//    }
}
