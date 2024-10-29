import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.4
import QtGraphicalEffects 1.0
import "qrc:/widgets"

Item {
    id: popup

    property int opacityDelay: 100
    property int showDelay: 1500
    property int hideDelay: 500
    property string iconPath: ""
    property string infoText: ""
    property int borderWidth: 12
    property int spacing: 10
    property int commonHeight: 20
    property alias textComponent: textItem

    property real startX: dapMainWindow.width
    property real stopX: dapMainWindow.width - (popup.width + 24)

    width: rectItem.width
    height: rectItem.height

    y: 84

    z: 100

    opacity: 0
    visible: false

    NumberAnimation{
        id:showAnim
        target: popup
        property: "x"
        from: startX
        to: stopX
        duration: 200
        running: false
    }

    Behavior on opacity {
        NumberAnimation {
            id: opacityAnim
            duration: opacityDelay
        }
    }

    Timer {
        id: showTimer
        interval: showDelay
        running: false
        repeat: false
        onTriggered:
        {
            opacityAnim.duration = hideDelay
            popup.opacity = 0.0
        }
    }

    Timer {
        id: hideTimer
        interval: showDelay+hideDelay
        running: false
        repeat: false
        onTriggered:
        {
            visible = false
        }
    }


    //Background
    Rectangle
    {
        id: rectItem
        width: 168
        height: 48
        radius: 16
        color: currTheme.secondaryBackground
    }

    DropShadow {
        anchors.fill: rectItem
        source: rectItem
        color: currTheme.reflection
        horizontalOffset: -1
        verticalOffset: -1
        radius: 0
        samples: 0
        opacity: 1
        fast: true
        cached: true
    }

    DropShadow
    {
        id: shadow
        anchors.fill: rectItem
        horizontalOffset: 5
        verticalOffset: 5
        radius: 8
        samples: 10
        cached: true
        color: currTheme.shadowColor
        source: rectItem
    }

    RowLayout
    {
        anchors.fill: rectItem
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 10

        DapImageLoader
        {
            Layout.alignment: Qt.AlignLeft
            id: imageItem
            innerWidth: commonHeight
            innerHeight: commonHeight
            source: iconPath
        }

        Text
        {
//            Layout.alignment: Qt.AlignRight
            id: textItem
            height: commonHeight
            font: mainFont.dapFont.medium14
            color: currTheme.white
            verticalAlignment: Qt.AlignVCenter
            text: infoText
        }
    }

    function showInfo(width, height, x_pos, y_pos, text, image)
    {
        if(width)
        {
            rectItem.width = width
            stopX = dapMainWindow.width - (popup.width + 24)
        }
        if(height)
            rectItem.height = height

        showTimer.stop()
        hideTimer.stop()

        showAnim.start()

        infoText = text
        iconPath = image

        opacityAnim.duration = opacityDelay

        opacity = 1
        visible = true

        showTimer.start()
        hideTimer.start()
    }
}

