import QtQuick 2.2
import QtQuick.Controls 2.0

DapBusyIndicatorForm
{

    contentItem:
        Item
        {
            id: busyItem
            width: busyIndicatorWidth
            height: busyIndicatorHeight

            //To set number of indicator point
            Repeater
            {
                id: busyRepeater
                model: busyPointNum

                Rectangle
                {
                    x: busyItem.width / 2 - width / 2
                    y: busyItem.height / 2 - height / 2
                    width: busyPointWidth
                    height: busyPointHeight
                    radius: busyPointRounding
                    opacity: 1
                    //Add possibility of color animation
                    Behavior on color
                    {
                        ColorAnimation
                        {
                            from: busyIndicatorDarkColor
                            duration: busyIndicatorDelay * busyPointNum
                        }
                    }
                    //Add possibility of scale animation
                    Behavior on scale
                    {
                        ScaleAnimator
                        {
                            from: busyPointMaxScale
                            duration: busyIndicatorDelay * busyPointNum
                        }
                    }
                    //Set next indicator point as transformation of previous
                    transform:
                        Rotation
                        {
                            origin.x: busyItem.width / 2 - width
                            origin.y: busyItem.height / 2 - height
                            angle: index * (360 / busyPointNum)
                        }
                    //Timer of every point transformation
                    Timer
                    {
                        id: reset
                        interval: busyIndicatorDelay * (index)
                        onTriggered:
                        {
                            parent.opacity = 1
                            parent.color = busyIndicatorDarkColor
                            parent.scale = busyPointMaxScale
                            reset2.start()
                        }
                    }
                    //Timer of point transformation at one position
                    Timer
                    {
                        id: reset2
                        interval: busyIndicatorDelay
                        onTriggered:
                        {
                            parent.opacity = 1
                            parent.color = busyIndicatorLightColor
                            parent.scale = busyPointMinScale
                        }
                    }
                    //Timer of all busy indicator transformation
                    Timer
                    {
                        id: globalTimer
                        interval: busyIndicatorDelay * busyPointNum
                        onTriggered:
                        {
                            reset.start()
                        }
                        triggeredOnStart: true
                        repeat: true
                    }
                    Component.onCompleted:
                    {
                        globalTimer.start()
                    }
                    onVisibleChanged:
                    {
                        if(!visible)
                        {
                            globalTimer.stop();
                            reset.stop();
                            reset2.stop();

                        }
                    }
                }
            }
        }

}
