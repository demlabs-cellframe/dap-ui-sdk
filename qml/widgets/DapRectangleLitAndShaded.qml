import QtQuick
import Qt5Compat.GraphicalEffects

Rectangle
{
    id: mainRect

    property color shadowColor: "#000000"
    property color lightColor: "#ffffff"
    property Item contentData

//    color: "gray"
//    radius: 0

    Rectangle
    {
        id: firstRect
        anchors.fill: parent
        color: mainRect.color
        radius: mainRect.radius

        Rectangle
        {
            id: secondRect
            anchors.fill: parent
            color: mainRect.color
            radius: mainRect.radius

            layer.enabled: true
            layer.effect: OpacityMask
            {
                maskSource: Rectangle
                {
                    width: mainRect.width
                    height: mainRect.height
                    radius: mainRect.radius
                }
            }

            data: contentData
        }
    }

    InnerShadow
    {
        id: shadow
        anchors.fill: firstRect
        horizontalOffset: 2
        verticalOffset: 2
        radius: 10
        samples: 10
//        fast: true
        cached: true
        color: shadowColor
        source: firstRect
        visible: firstRect.visible
    }
    InnerShadow {
        id: light
        anchors.fill: firstRect
        horizontalOffset: -1
        verticalOffset: -1
        radius: 0
        samples: 10
//        fast: true
        cached: true
        color: lightColor
        source: shadow
        visible: firstRect.visible
    }

}
