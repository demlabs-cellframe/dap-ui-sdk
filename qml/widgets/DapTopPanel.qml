import QtQuick 2.4

Rectangle
{
    property alias source: loader.source
    property alias sourceComponent: loader.sourceComponent

    x: 3 * pt
    anchors.left: parent.left
//    radius: 8 * pt
    height: 60 * pt
    width: parent.width
    color: "#070023"

    Rectangle {
        color: parent.color
        height: parent.height
        width: parent.radius
        x: parent.width - width
    }
    Loader
    {
        anchors.fill: parent
        id: loader
    }
}

