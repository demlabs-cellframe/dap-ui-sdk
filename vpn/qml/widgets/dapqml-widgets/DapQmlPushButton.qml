import QtQuick 2.12
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.5
import DapQmlStyle 1.0

Button {
    id: root
    width: 374
    height: 90

    property string active: "qrc:/light/btn_bg_hover_active.png"
    property string inactive: "qrc:/light/btn_bg.png"
    property int fontSize: 14
    property string qss

    function isStill()
    {
        return !root.hovered && !root.down && !root.checked;
    }

    /* content */
    contentItem: Text {
        anchors.centerIn: root

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: "#FFFFFF"
        text: root.text
        font.family: "Roboto"
        font.pixelSize: root.fontSize
        font.weight: Font.Bold
    }

    background: Image {
        anchors.fill: parent
        mipmap: true
        source: !isStill() ? root.active : root.inactive
    }

    DapQmlStyle { id: style; qss: root.qss; item: root }
}
