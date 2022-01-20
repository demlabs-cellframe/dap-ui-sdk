import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

RadioButton {
    id: root

    signal clicked();

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
    }

    contentItem: DapQmlLabel {
        leftPadding: root.indicator.width + root.spacing
        horizontalAlign: Text.AlignLeft
        verticalAlign: Text.AlignVCenter
        text: root.text
        qss: root.textQss
        clip: false
    }

    /* bottom separator */
    DapQmlSeparator {
        y: root.height - height
        width: root.width
        visible: root.separator
    }

//    /* content */
//    GridLayout {
//        anchors.fill: parent;
//        columns: 1

//        /* btn & label */
//        GridLayout {
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            columns: 2

//            /* icon */
//            DapQmlLabel {
//                Layout.preferredWidth: root.iconSize
//                Layout.preferredHeight: root.iconSize

//                qss: root.checked ? "radio-on" : "radio-off"
//                width: root.iconSize
//                height: root.iconSize
//            }

//            /* text */
//            DapQmlLabel {
//                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
//                Layout.fillWidth: true
//                Layout.fillHeight: true

//                horizontalAlign: Text.AlignLeft
//                verticalAlign: Text.AlignVCenter
//                text: root.text
//                qss: root.textQss
//                clip: false
//            }
//        }

//        /* bottom separator */
//        DapQmlSeparator {
//            Layout.fillWidth: true
//            width: root.width
//            visible: root.separator
//        }
//    }
}
