import QtQuick 2.4
import QtGraphicalEffects 1.0

Rectangle
{
    id:control
    property alias source: loader.source
    property alias sourceComponent: loader.sourceComponent

    x: 0 * pt
    anchors.left: parent.left
//    radius: 8 * pt
    height: 60 * pt
    width: parent.width
//    color: "#070023"

    Rectangle {
        color: parent.color
        height: parent.height
        width: parent.radius
        x: parent.width - width
    }

    Rectangle{
        id:blockRadius
        width: parent.radius
        color: currTheme.backgroundPanel
        height: control.height - (currTheme.radiusRectangle-5*pt)
        anchors.top: control.top
        anchors.left: control.left
        radius: 1
    }
    InnerShadow {
            anchors.fill: control
            cached: true
            horizontalOffset: 1
            verticalOffset: 0
            radius: 1
            samples: 1
            color: "#4C4B5A"
//            smooth: true
            source: control
        }
    InnerShadow {
            anchors.fill: blockRadius
            cached: true
            horizontalOffset: 1
            verticalOffset: 0
            radius: 1
            samples: 10
            color: "#4C4B5A"
//            smooth: true
            source: blockRadius
        }

    //left ligt line
//    Rectangle{
//        width: 2
//        color: currTheme.reflection
//        height: control.height - (currTheme.radiusRectangle-5*pt)
//        anchors.top: control.top
//        anchors.left: control.left

//    }
    Loader
    {
        anchors.fill: parent
        id: loader
    }
}

