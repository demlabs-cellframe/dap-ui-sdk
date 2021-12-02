import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

Page
{
    id: control
    height: 55
    background: Rectangle {
        color: "transparent"
    }

    Rectangle {
        id: beckgrndRect
        anchors.fill: parent
        radius: 20
        color: currTheme.backgroundPanel

        height: control.height - (currTheme.radiusRectangle-5*pt)
        anchors.top: control.top
        anchors.left: control.left
        radius: 1
    }

    DropShadow {
        anchors.fill: beckgrndRect
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: beckgrndRect
    }

    InnerShadow {
        anchors.fill: beckgrndRect
        radius: 0
        samples: 16
        horizontalOffset: 1
        color: "#4C4B5A"
        source: beckgrndRect
    }

    InnerShadow {
        anchors.fill: leftRect
        radius: 0
        samples: 16
        horizontalOffset: 1
        color: "#4C4B5A"
        source: leftRect
    }
}

