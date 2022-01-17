import QtQuick 2.4
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root

    signal close()

    /* title */
    DapQmlDialogTitle {
        text: "Bug report"
        qss: "dialog-title"
    }

    /* text edit */
    Item {
        id: input
        property string qss: "bugrep-input"

        Image {
            anchors.fill: input
            source: "qrc:/light/report_bg.png"
        }

        DapQmlStyle { id: style; qss: input.qss; item: input }
    }

    /* letter counter */
    DapQmlLabel {
        qss: "bugrep-letter-counter"
        text: "0/200"
        color: "#A4A3C0"
        horizontalAlign: Text.AlignRight
    }

    /* attach */
    DapQmlLabel {
        id: attach
        qss: "bugrep-attach-btn"
        text: "Click here to attach a screenshot"
        color: "#DA0B82"

        MouseArea {
            id: mouseArea
            anchors.fill: attach
            cursorShape: Qt.PointingHandCursor
            enabled: false
        }
    }

    /* send button */
    DapQmlPushButton {
        qss: "bugrep-send-btn"
        text: "SEND REPORT"
    }
}
