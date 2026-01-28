import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "qrc:/widgets"

Item {
    id: popup

    property int opacityDelay: 100
    property int showDelay: 1500
    property int hideDelay: 500
    property string iconPath: ""
    property string infoText: ""
    property int borderWidth: 12 * scaleFactor
    property int spacing: 10 * scaleFactor
    property int commonHeight: 20 * scaleFactor
    property alias textComponent: textItem

    property double scaleFactor: 1.0

    property real startX: mainWindow.width
    property real stopX: mainWindow.width - (popup.width + 24 * scaleFactor)

    width: rectItem.width
    height: rectItem.height

    y: 84 * scaleFactor

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
        width: 168 * scaleFactor
        height: 48 * scaleFactor
        radius: 16 * scaleFactor
        color: currTheme.secondaryBackground
    }

    // MultiEffect {

    //     anchors.fill: rectItem
    //     source: rectItem
    //     shadowBlur: 1.0
    //     shadowEnabled: true
    //     shadowColor: currTheme.reflection
    //     shadowVerticalOffset: -1
    //     shadowHorizontalOffset: -1
    // }

    MultiEffect {

        anchors.fill: rectItem
        source: rectItem
        shadowBlur: 1.0
        shadowEnabled: true
        shadowColor: currTheme.shadowColor
        shadowVerticalOffset: 5
        shadowHorizontalOffset: 5
    }

    RowLayout
    {
        anchors.fill: rectItem
        anchors.leftMargin: 16 * scaleFactor
        anchors.rightMargin: 16 * scaleFactor
        spacing: 10 * scaleFactor

        DapImageLoader
        {
            Layout.alignment: Qt.AlignLeft
            id: imageItem
            Layout.minimumHeight: commonHeight
            Layout.maximumHeight: commonHeight
            Layout.minimumWidth: commonHeight
            Layout.maximumWidth: commonHeight
            innerWidth: commonHeight
            innerHeight: commonHeight
            source: iconPath
        }

        Text
        {
            id: textItem
            height: commonHeight
            font: mainFont.dapFont.medium14
            color: currTheme.white
            horizontalAlignment: Qt.AlignLeft
            verticalAlignment: Qt.AlignVCenter
            text: infoText
            elide: Text.ElideRight
        }
    }

    function showInfo(width, height, x_pos, y_pos, text, image)
    {
        if(width)
        {
            rectItem.width = width
        }
        else
        {
            // Reset to default width when width=0 is passed
            rectItem.width = 168 * scaleFactor
        }
        stopX = mainWindow.width - (popup.width + 24 * scaleFactor)

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

