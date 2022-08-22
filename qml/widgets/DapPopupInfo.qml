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

    width: rectItem.width
    height: rectItem.height

    z: 100

    opacity: 0
    visible: false

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


    Rectangle
    {
        id: rectItem
        width: borderWidth*2+5 + spacing + textItem.width + imageItem.width
        height: borderWidth*2+commonHeight
        border.width: 1
        border.color: currTheme.lineSeparatorColor
        radius: 16
        color: currTheme.backgroundElements


        Text
        {
            id: textItem
            x: borderWidth+5
            y: borderWidth-2
            height: commonHeight
            font: mainFont.dapFont.medium14
            color: currTheme.textColor
            verticalAlignment: Qt.AlignVCenter

            text: infoText
        }

        DapImageLoader
        {
            id: imageItem
            x: borderWidth+5+spacing+textItem.width
            y: borderWidth
            innerWidth: commonHeight
            innerHeight: commonHeight
            source: iconPath
        }
    }

    DropShadow
    {
        id: shadow
        anchors.fill: rectItem
        horizontalOffset: 2
        verticalOffset: 2
        radius: 8
        samples: 10
        cached: true
        color: currTheme.shadowColor
        source: rectItem
    }

    function showInfo(x_pos, y_pos, text, image)
    {
        showTimer.stop()
        hideTimer.stop()
        opacityAnim.stop()

        x = x_pos
        y = y_pos

        infoText = text
        iconPath = image

        opacityAnim.duration = opacityDelay

        opacity = 1
        visible = true

        showTimer.start()
        hideTimer.start()
    }
}

