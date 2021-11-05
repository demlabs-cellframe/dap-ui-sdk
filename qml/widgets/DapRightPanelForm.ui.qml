import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Page
{
    id: root

    header: root.header


    //Header height
    dapHeader.height: 36 * pt
    dapRightPanelWidth: visible? 350 * pt : 0 * pt
    width: dapRightPanelWidth
    anchors {
        top: parent.top
        topMargin: 24 * pt
        right: parent.right
        rightMargin: 20 * pt
//        left: parent.left
//        leftMargin: 24 * pt
        bottom: parent.bottom
        bottomMargin: 20 * pt
    }

    background: Rectangle {
        id: backGrndRect
        color: currTheme.backgroundMainScreen
    }

    Rectangle
    {
        id: frame
        anchors.fill: parent
        anchors.margins: 24 * pt
        color: currTheme.backgroundElements
        radius: 16 * pt

        InnerShadow {
            id: topLeftSadow
            anchors.fill: frame
            cached: true
            horizontalOffset: 5
            verticalOffset: 5
            radius: 4
            samples: 32
            color: "#2A2C33"
            smooth: true
            source: frame
        }
        InnerShadow {
            anchors.fill: frame
            cached: true
            horizontalOffset: -1
            verticalOffset: -1
            radius: 1
            samples: 32
            color: "#4C4B5A"
            source: topLeftSadow
        }
    }

    //Close button for right pannel
    property DapButton dapButtonClose:
        DapButton
    {
        id: buttonClose
        height: 20 * pt
        width: 20 * pt
        heightImageButton: 10 * pt
        widthImageButton: 10 * pt
        activeFrame: false
        normalImageButton: "qrc:/resources/icons/"+pathTheme+"/close_icon.png"
        hoverImageButton:  "qrc:/resources/icons/"+pathTheme+"/close_icon_hover.png"
    }
}
