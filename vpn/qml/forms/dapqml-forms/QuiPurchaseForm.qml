import QtQuick 2.0
import DapQmlStyle 1.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "Purchase"

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
        Component.onCompleted: StyleDebugTree.describe (
           "Purchase Resizer",
            ["x", "y", "width", "height"],
           this);
    }

    /* list */
    ListView {
        id: purchaseListView
        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        property string qss: "purchase-listview"
        DapQmlStyle { qss: purchaseListView.qss; item: purchaseListView }

        model: purchaseListModel

        Component.onCompleted: StyleDebugTree.describe (
           "Purchase Listview",
            ["x", "y", "width", "height"],
           this);

        delegate: DapQmlButton {
            width: resizer.width // purchaseListView.width
            height: resizer.height // 130
            buttonStyle: DapQmlButton.Style.LeftTopMainBottomSub
            frame: true
            leftText: model.price
            mainText: model.main
            subText: model.sub
            mainQss: "purchase-btn-label-main"
            subQss: "purchase-btn-label-sub"
            leftQss: "purchase-btn-label-left"
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
