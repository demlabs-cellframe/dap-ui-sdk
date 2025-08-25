/* INCLUDES */

import QtQuick 2.0
import DapQmlModelSerialHistory 1.0
import Brand 1.0
//import StyleDebugTree 1.0
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
    clip: true

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
        text: titleText
        qss: "dialog-title"
        property string titleText: qsTr("Serial key history") + lang.notifier
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
        id: icnCpyResizer
        visible: false
        qss: "ph-icn-cpy-resizer"
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

    DapQmlDummy {
        id: contentRect
        qss: "content-mid"
    }

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: phListView
        objectName: "purchaseHistoryListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: listviewSizer.width ? listviewSizer.width : resizer.width // root.width - 72
        //width: resizer.width
        height: root.height - y - noticeResizer.height - noticeSpacer.height
        clip: true

        DapQmlDummy {
            id: listviewSizer
            qss: "ph-content";
        }

        delegate: Item {
            width: phListView.width
            height: resizer.height + spacer.height

            DapQmlButton {
                x: (parent.width - width) / 2
                y: spacer.height / 2
                z: 50
                width: contentRect.width // resizer.width
                height: resizer.height
                buttonStyle: DapQmlButton.IconMainSubIcon
                mainText: model.key
                subText: " "
                mainQss: "ph-btn-label-main"
                subQss: "ph-btn-label-main"
                icon: Brand.isOldStyle() ? "ic_key-item" : "null-size null-pos"
                iconRight: "ic_copy"
                iconSize: icnResizer.height
                iconRightSize: icnCpyResizer.height

                signal sigStartAnimation();

                property QtObject animationCtl: QtObject {
                    property var target: parent
                    property Timer timer: Timer {
                        interval: 500
                        running: false
                        repeat: false
                        onTriggered: parent.finish()
                    }

                    function start() {
                        target.iconRight    = "ic_copy_pressed";
                        timer.start();
                    }

                    function finish() {
                        target.iconRight    = "ic_copy";
                    }
                }

                onRightClicked: {
                    clipboard.setText (model.key);
                    // animationCtl.start();
                    sigStartAnimation();
                }

                Component.onCompleted: phListView.model.attachAnimation(this)
            }

            DapQmlSeparator {
                anchors.bottom: parent.bottom
                z: 40
                width: parent.width
            }

        }
    }

    /****************************************//**
     * Empty history annotaion
     ********************************************/

    DapQmlLabel {
        id: empotyHistory
        visible: DapQmlModelSerialHistory.length() === 0
        text: qsTr("The license key usage history on this device is empty.") + lang.notifier
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
        text: qsTr("The license key usage history is stored locally on this device. After reinstalling the system the key history will be unavailable.") + lang.notifier
        wrapMode: Text.WordWrap
        qss: "ph-label-notice"

//        Component.onCompleted: {
//           StyleDebugTree.describe (
//               "ph notice",
//                ["x", "y", "width", "height", "contentWidth", "contentHeight"],
//               this);
//           StyleDebugTree.describe (
//               "ph notice label",
//                ["x", "y", "width", "height", "contentWidth", "contentHeight"],
//               notice.label);
//        }
    }
}

/*-----------------------------------------*/
