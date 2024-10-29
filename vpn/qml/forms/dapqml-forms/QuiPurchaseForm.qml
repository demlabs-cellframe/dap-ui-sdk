/* INCLUDES */

import QtQuick 2.0
import QtQml.Models 2.1
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Purchase Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * - Model
 * - Resizer
 * - ListView with DapQmlButton as delegate
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum Type
    {
        T_1_MONTH   = 0,
        T_6_MONTHS  = 1,
        T_1_YEAR    = 2
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "Purchase"

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief 1 month plan clicked
    signal sig1month();

    /// @brief 6 months plan clicked
    signal sig6months();

    /// @brief 1 year plan clicked
    signal sig1year();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function btnClicked(itemId) {
        switch(itemId) {
        case QuiPurchaseForm.Type.T_1_MONTH:    root.sig1month(); break;
        case QuiPurchaseForm.Type.T_6_MONTHS:   root.sig6months(); break;
        case QuiPurchaseForm.Type.T_1_YEAR:     root.sig1year(); break;
        }
    }

    /// @}
    /****************************************//**
     * Model
     ********************************************/

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
            main: "6-months plan"
            sub: "$4.48 per month"
        }
        ListElement {
            itemId: 2
            price: "$35.88"
            main: "1-year plan"
            sub: "$2.99 per month"
        }
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Get a new license key") + lang.notifier
        qss: "dialog-title"
    }

    /****************************************//**
     * Resizers
     ********************************************/

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

    /****************************************//**
     * Listview
     ********************************************/

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

/*-----------------------------------------*/
