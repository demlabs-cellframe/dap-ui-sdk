import QtQuick 2.0
import DapQmlModelSerialHistory 1.0
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

    /* resizer */
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

    /* listview */
    ListView {
        id: phListView
        objectName: "purchaseHistoryListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y - notice.height
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

    /* empty history annotation */
    DapQmlLabel {
        id: empotyHistory
        visible: DapQmlModelSerialHistory.length() === 0
        text: "The license key usage history on this device is empty."
        wrapMode: Text.WordWrap
        qss: "ph-label-empty-history"
    }

    /* notice */
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
