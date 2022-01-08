import QtQuick 2.0
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {
    id: root

    function centerHor(item) {
        return root.width / 2 - item.width / 2;
    }

    /* title */
    DapQmlLabel {
        x: centerHor(this)
        y: 62
        width: 340
        height: 28
        text: "VPN Connected"
        qss: "dashboard-title"
    }

    /* status */
    RowLayout {
        x: centerHor(this)
        y: 107
        width: 108
        height: 22

        DapQmlLabel {
            Layout.fillWidth: false
            Layout.preferredWidth: 5
            Layout.preferredHeight: 5
            id: indicator
            qss: "ic_online"
        }

        DapQmlLabel {
            Layout.fillWidth: false
            Layout.preferredWidth: 86
            Layout.preferredHeight: 22
            id: statusTime
            text: "56:30:10"
            qss: "dashboard-status"
        }
    }
}
