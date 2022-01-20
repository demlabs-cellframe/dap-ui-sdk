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
    property int wrapMode: Text.NoWrap

    property real bottomPadding
    property real leftPadding
    property real padding
    property real rightPadding
    property real topPadding

    Image {
        id: icon
        anchors.fill: root
        source: root.scaledPixmap
        mipmap: true
    }

    Text {
        id: label
        anchors.fill: root

        bottomPadding: root.bottomPadding
        leftPadding: root.leftPadding
        padding: root.padding
        rightPadding: root.rightPadding
        topPadding: root.topPadding

        horizontalAlignment: root.horizontalAlign
        verticalAlignment: root.verticalAlign

        text: root.text
        color: root.color
        wrapMode: root.wrapMode

        font {
            family: root.fontFamiliy
            pixelSize: root.fontSize
            weight: root.fontWeight
        }
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
