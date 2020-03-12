import QtQuick 2.2
import QtQuick.Controls 2.0

BusyIndicator
{
    ///@detalis dapBusy Identificator of dap busy indicator
    property alias dapBusy: control
    ///@detalis busyPointNum Number of rectangle points at busy indicator
    property int busyPointNum
    ///@detalis busyPointRounding Rounding of rectangle points at busy indicator
    property int busyPointRounding
    ///@detalis busyPointWidth Width of rectangle points at busy indicator
    property int busyPointWidth
    ///@detalis busyPointHeight Height of rectangle points at busy indicator
    property int busyPointHeight
    ///@detalis busyPointMinScale Minimum scale of point
    property real busyPointMinScale
    ///@detalis busyPointMaxScale Maximum scale of point
    property real busyPointMaxScale
    ///@detalis busyIndicatorWidth Width of rectangle busy indicator
    property int busyIndicatorWidth
    ///@detalis busyIndicatorHeight Height of rectangle busy indicator
    property int busyIndicatorHeight
    ///@detalis busyIndicatorDelay Delay between busy indicator states
    property int busyIndicatorDelay
    ///@detalis busyIndicatorDarkColor Color of lightest point of busy indicator
    property string busyIndicatorDarkColor
    ///@detalis busyIndicatorLightColor Color of darkest point of busy indicator
    property string busyIndicatorLightColor



    id: control
    visible: running

}

