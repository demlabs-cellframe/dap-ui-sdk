import QtQuick 2.1
import DapQmlStyle 1.0

Rectangle {
    id: root
    property string qss
    property string text: ""
    property int horizontalAlignment: Text.AlignHCenter
    property int verticalAlignment: Text.AlignVCenter

    Text {
        anchors.fill: root

        horizontalAlignment: root.horizontalAlignment
        verticalAlignment: root.verticalAlignment

        text: root.text

        font {
            family: "Lato"
            pixelSize: 20
        }
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
