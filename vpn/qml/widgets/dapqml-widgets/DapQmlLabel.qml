import QtQuick 2.12
import DapQmlStyle 1.0

Item {
    id: root
    clip: true
//    width: (label.paintedWidth > icon.paintedWidth) ? label.paintedWidth : icon.paintedWidth
//    height: (label.paintedHeight > icon.paintedHeight) ? label.paintedHeight : icon.paintedHeight
    property string qss
    property string text: ""
    property color color
    property string scaledPixmap: ""
    property string fontFamiliy: "Lato"
    property int fontSize: 12
    property int fontWeight: Font.Normal
    property int horizontalAlign: Text.AlignHCenter
    property int verticalAlign: Text.AlignVCenter

    Image {
        id: icon
        anchors.fill: root
        source: root.scaledPixmap
        mipmap: true
    }

    Text {
        id: label
        anchors.fill: root

        horizontalAlignment: root.horizontalAlign
        verticalAlignment: root.verticalAlign

        text: root.text
        color: root.color

        font {
            family: root.fontFamiliy
            pixelSize: root.fontSize
            weight: root.fontWeight
        }
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
