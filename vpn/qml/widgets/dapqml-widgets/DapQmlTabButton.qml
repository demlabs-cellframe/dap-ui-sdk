import QtQuick 2.12
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.5
import DapQmlStyle 1.0

TabButton {
    id: root
    width: 130
    height: 130
    background: Rectangle { color: "transparent" }

    property string active
    property string inactive
    property string qss

    DapQmlStyle { item: root; qss: root.qss; }

    function isStill()
    {
        return !root.checked;//!root.hovered && !root.down;
    }

    /* content */
    Image {
        anchors.fill: parent
        mipmap: true
        source: !isStill() ? root.active : root.inactive
    }
}
