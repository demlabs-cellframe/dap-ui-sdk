import QtQuick 2.4

Rectangle
{
    property string dapStatusText
    property var dapStatusTextFont
    property var dapStatusTextColor

    property int dapStatusIndicatorWidth
    property int dapStatusIndicatorHeight
    property int dapStatusIndicatorRadius
    property var dapStatusIndicatorColor

    property int dapStatusSpacing
    property var dapStatusBackgroundColor


    id: statusRectangle
    color: dapStatusBackgroundColor


    Row
    {
        id: statusRow
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.fill: parent
        spacing: dapStatusSpacing

        Text
        {
            id: statusText
            anchors.verticalCenter: parent.verticalCenter
           // width: dapStatusTextWidth
           // height: dapStatusTextHeight
            font: dapStatusTextFont
            text: dapStatusText
            color: dapStatusTextColor
        }

        Rectangle
        {
            id: statusIndicatorRectangle
            anchors.verticalCenter: parent.verticalCenter
            width: dapStatusIndicatorWidth
            height: dapStatusIndicatorHeight
            radius: dapStatusIndicatorRadius
            color: dapStatusIndicatorColor
        }
    }

}
