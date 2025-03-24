import QtQuick 2.4
import Qt5Compat.GraphicalEffects

Rectangle
{
    id:control
    property alias source: loader.source
    property alias sourceComponent: loader.sourceComponent

    x: 0 
    anchors.left: parent.left
//    radius: 8 
    height: 60 
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
        height: control.height - (currTheme.frameRadius-5*pt)
        anchors.top: control.top
        anchors.left: control.left
        radius: 1
    }

    //left ligt line
//    Rectangle{
//        width: 2
//        color: currTheme.reflection
//        height: control.height - (currTheme.frameRadius-5*pt)
//        anchors.top: control.top
//        anchors.left: control.left

//    }
    Loader
    {
        anchors.fill: parent
        id: loader
    }
}

