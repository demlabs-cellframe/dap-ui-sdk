import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

RadioButton {
    id: root

    /* VARS */

    text: ""
    checked: false
    property string textQss: "radio-label"
    property string qss: ""
    property int iconSize: 60
    property bool separator: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

    indicator: DapQmlLabel {
        qss: root.checked ? "radio-on" : "radio-off"
        width: root.iconSize
        height: root.iconSize
        onClicked: { root.toggle(); root.clicked(); }
    }

    contentItem: DapQmlLabel {
        leftPadding: root.indicator.width + root.spacing
        horizontalAlign: Text.AlignLeft
        verticalAlign: Text.AlignVCenter
        text: root.text
        qss: root.textQss
        clip: false
        onClicked: { root.toggle(); root.clicked(); }
    }

    /* bottom separator */
    DapQmlSeparator {
        x: (root.width - width) / 2
        y: root.height - height
        width: root.width - 32
        visible: root.separator
    }
}
