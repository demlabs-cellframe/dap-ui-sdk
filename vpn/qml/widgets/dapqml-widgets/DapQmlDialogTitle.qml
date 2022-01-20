import QtQuick 2.0
import DapQmlStyle 1.0

Item {
    id: root
    property string text: "Untitled"
    property string qss

    /* close button */
    DapQmlPushButton {
        inactive: "qrc:/light/ic_close.png"
        active: "qrc:/light/ic_close_hover.png"
        qss: "form-title-close-btn"
    }

    /* label */
    DapQmlLabel {
        qss: "form-title-label"
        text: root.text
        clip: false
        fontWeight: Font.Bold
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
