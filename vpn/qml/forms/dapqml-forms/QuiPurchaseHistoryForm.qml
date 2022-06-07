/* INCLUDES */

import QtQuick 2.0
import DapQmlModelSerialHistory 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Purchase History Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * - 4 resizers
 * - ListView with DapQmlButton as delegate
 * - Notice at the bottom of the form
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "PurchaseHistory"

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: "History"
        qss: "dialog-title"
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "ph-btn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    DapQmlRectangle {
        id: icnResizer
        visible: false
        qss: "ph-icn-resizer"
    }

    DapQmlRectangle {
        id: noticeResizer
        visible: false
        qss: "ph-notice-resizer"
    }

    DapQmlRectangle {
        id: noticeSpacer
        visible: false
        qss: "ph-notice-spacer"
    }

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: phListView
        objectName: "purchaseHistoryListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y - noticeResizer.height - noticeSpacer.height
        clip: true

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height

            DapQmlButton {
                buttonStyle: DapQmlButton.IconMainSubIcon
                mainText: model.key
                subText: " "
                mainQss: "ph-btn-label-main"
                subQss: ""
                icon: "ic_key-item"
                iconRight: "ic_copy"
                separator: true
                iconSize: icnResizer.height
                iconRightSize: icnResizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
            }
        }
    }

    /****************************************//**
     * Empty history annotaion
     ********************************************/

    DapQmlLabel {
        id: empotyHistory
        visible: DapQmlModelSerialHistory.length() === 0
        text: "The license key usage history on this device is empty."
        wrapMode: Text.WordWrap
        qss: "ph-label-empty-history"
    }

    /****************************************//**
     * Notice
     ********************************************/

    DapQmlLabel {
        id: notice
        x: (root.width - noticeResizer.width) / 2
        y: root.height - noticeResizer.height - noticeSpacer.height
        width: noticeResizer.width
        height: noticeResizer.height
        text: "The license key usage history is stored locally on this device. After reinstalling the system the key history will be unavailable."
        wrapMode: Text.WordWrap
        qss: "ph-label-notice"
    }
}

/*-----------------------------------------*/
