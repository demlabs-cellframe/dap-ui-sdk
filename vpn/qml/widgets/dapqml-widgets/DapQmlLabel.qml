import QtQuick 2.12
import DapQmlStyle 1.0
import Brand 1.0

Item {
    id: root
    clip: true

    /* signals */
    signal clicked();

    /* VARS */
    property string qss
    property string text: ""
    property color color
    property string scaledPixmap: ""
    property var fontFamiliy: Brand.fontName()
    property int fontSize: 12
    property int fontWeight: Font.Normal
    property int horizontalAlign: Text.AlignHCenter
    property int verticalAlign: Text.AlignVCenter
    property int wrapMode: Text.NoWrap
    property int textFormat: Text.AutoText
    property bool mipmap: true

    property real bottomPadding
    property real leftPadding
    property real padding
    property real rightPadding
    property real topPadding

    Image {
        id: icon
        anchors.fill: root
        source: root.scaledPixmap
        mipmap: root.mipmap
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
        textFormat: root.textFormat

        font {
            family: root.fontFamiliy
            pixelSize: root.fontSize
            weight: root.fontWeight
        }
    }

    MouseArea {
        anchors.fill: root
        onClicked: root.clicked()
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
