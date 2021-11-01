//****************************************************************************
//                      Implements a screen widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: frameScreen

    ///@detalis Frame widget.
    property alias dapFrame: frameScreen
    ///@detalis Screen components.
    property Item dapContenetItemScreen

    anchors.fill: parent
    color: currTheme.backgroundMainScreen
    width: 678 * pt

    anchors
    {
        top: parent.top
        topMargin: 24 * pt
        right: parent.right
        rightMargin: 44 * pt
        left: parent.left
        leftMargin: 24 * pt
        bottom: parent.bottom
        bottomMargin: 20 * pt

    }

    // Install screen components
    Item 
    {
        id: contenetItemScreen
        data: dapContenetItemScreen
        anchors.fill: parent
    }
}
