import QtQuick 2.4

Rectangle
{
    radius: 8 * pt

    height: 60 * pt
    width: parent.width
    color: "#070023"

    Rectangle {
        color: parent.color
        height: parent.height
        width: parent.radius
        x: parent.width - width
    }
}

