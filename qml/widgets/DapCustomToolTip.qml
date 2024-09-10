import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

ToolTip
{
    id: root
    signal updatePos()

    property alias bottomRect: bottomRect
    property font textFont: mainFont.dapFont.regular12
    property string textColor: currTheme.white
    property string contentText
    property alias backgroundToolTip: backgroundToolTip

    property bool globalOFF: false

    property int offset: 8
    property int widthLimit: 250
    property bool isUnderDirection: false
    property var foundMouseArea: findMouseAreaRecursive(parent)
    property bool mouseOver: globalOFF || foundMouseArea === undefined ? false : foundMouseArea.containsMouse
    property int timerInterval: 0

    y: isUnderDirection == true ? parent.height + offset + bottomRect.height/2 + 3 : -(backgroundToolTip.height + 14)

    onMouseOverChanged:
    {
        if(mouseOver == true)
        {
            tooltipTimer.start()
        }
        else
        {
            tooltipTimer.stop()
            root.visible = false
        }
    }

    function findMouseAreaRecursive(element)
    {
        var queue = [element]
        while (queue.length > 0)
        {
            var currentItem = queue.shift()

            if (currentItem instanceof MouseArea)
            {
                currentItem.hoverEnabled = true
                return currentItem
            }

            for (var i = 0; i < currentItem.children.length; ++i)
            {
                var child = currentItem.children[i];
                queue.push(child);
            }
        }
        console.log("DapCustomToolTip", textElement.text, "MouseArea not found. Need MouseArea")
        return undefined
    }

    onVisibleChanged: updatePos()

    Timer
    {
        id: tooltipTimer
        interval: timerInterval
        repeat: false
        running: false
        onTriggered:
        {
            root.visible = true
        }
    }


    contentItem:
        Item
    {
        implicitWidth: textElement.width + 8
        Text
        {
            id: textElement
            width: contentWidth
            //anchors.horizontalCenter: parent.horizontalCenter
            anchors.centerIn: parent
            color: textColor
            text: contentText
            font: textFont
            wrapMode: Text.NoWrap
            y: -5

            onContentWidthChanged:
            {
                if(contentWidth > widthLimit)
                {
                    width = widthLimit
                    wrapMode = Text.WordWrap
                }
                else
                {
                    width = contentWidth
                }
            }
        }
    }

    background:
        Item
    {
        Rectangle
        {
            id: backgroundToolTip
            anchors.fill: parent
            radius: 4
            color: currTheme.mainBackground
        }
        DropShadow
        {
            anchors.fill: backgroundToolTip
            source: backgroundToolTip
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
            anchors.fill: backgroundToolTip
            source: backgroundToolTip
            color: currTheme.shadowColor
            horizontalOffset: 2
            verticalOffset: 2
            radius: 10
            samples: 20
            opacity: 1
        }

        Rectangle
        {
            id: bottomRect
            anchors.horizontalCenter: backgroundToolTip.horizontalCenter
            color: backgroundToolTip.color
            width: 10
            height: 10
            rotation: 45

            Connections
            {
                target: root

                function onUpdatePos ()
                {
                    if(root.y < 0)
                    {
                        bottomRect.anchors.top = backgroundToolTip.bottom
                        bottomRect.anchors.topMargin = -(bottomRect.width/2)

                        bottomLineHide.anchors.bottom = backgroundToolTip.bottom
                        shadow1.visible = true
                        shadow2.visible = false
                    }
                    else
                    {
                        bottomRect.anchors.bottom = backgroundToolTip.top
                        bottomRect.anchors.bottomMargin = -(bottomRect.width/2)

                        bottomLineHide.anchors.top = backgroundToolTip.top
                        shadow1.visible = false
                        shadow2.visible = true
                    }
                }
            }
        }

        DropShadow
        {
            id: shadow1
            rotation: 45
            anchors.fill: bottomRect
            source: bottomRect
            color: currTheme.shadowColor
            horizontalOffset: 2
            verticalOffset: 2
            radius: 10
            samples: 20
            opacity: 1
            visible: bottomRect.visible
        }
        DropShadow
        {
            id: shadow2
            rotation: 45
            anchors.fill: bottomRect
            source: bottomRect
            color: currTheme.reflection
            horizontalOffset: -1
            verticalOffset: -1
            radius: 0
            samples: 0
            opacity: 1
            fast: true
            cached: true
            visible: bottomRect.visible
        }

        Rectangle
        {
            id: bottomLineHide
            anchors.left: backgroundToolTip.left
            anchors.right: backgroundToolTip.right
            height: bottomRect.height/2
            color: backgroundToolTip.color
            visible: bottomRect.visible
        }
    }
}
