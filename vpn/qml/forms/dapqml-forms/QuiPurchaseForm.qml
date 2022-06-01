import QtQuick 2.0
import QtQml.Models 2.1
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "Purchase"

    /* defs */
    enum Type
    {
        T_1_MONTH   = 0,
        T_6_MONTHS  = 1,
        T_1_YEAR    = 2
    }

    /* signals */
    signal sig1month();
    signal sig6months();
    signal sig1year();

    /* functions */
    function btnClicked(itemId) {
        switch(itemId) {
        case QuiPurchaseForm.Type.T_1_MONTH:    root.sig1month(); break;
        case QuiPurchaseForm.Type.T_6_MONTHS:   root.sig6months(); break;
        case QuiPurchaseForm.Type.T_1_YEAR:     root.sig1year(); break;
        }
    }

    /* model */
    ListModel {
        id: purchaseListModel
        ListElement {
            itemId: 0
            price: "$4.98"
            main: "1-month plan"
            sub: ""
        }
        ListElement {
            itemId: 1
            price: "$26.88"
            main: "6-month plan"
            sub: "$4.48 per month"
        }
        ListElement {
            itemId: 2
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

    /* spacing resizer */
    DapQmlLabel {
        id: resizerSpacing
        visible: false
        qss: "purchase-btn-spacing"
    }

    /* listview position */
    DapQmlLabel {
        id: resizerListView
        visible: false
        qss: "purchase-listview"
    }

    /* list */
    ListView {
        id: purchaseListView
        x: (root.width - width) / 2
        y: resizerListView.y // title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true
        spacing: resizerSpacing.height

        DapQmlStyle { qss: "purchase-listview"; item: purchaseListView }

        model: purchaseListModel

        delegate: DapQmlButton {
            property int itemId: model.itemId

            width: resizer.width
            height: resizer.height

            buttonStyle: DapQmlButton.Style.LeftTopMainBottomSub
            frame: true

            leftText: model.price
            mainText: model.main
            subText: model.sub

            mainQss: "purchase-btn-label-main"
            subQss: "purchase-btn-label-sub"
            leftQss: "purchase-btn-label-left"

            onClicked: btnClicked(model.itemId);
        }
    }
}
