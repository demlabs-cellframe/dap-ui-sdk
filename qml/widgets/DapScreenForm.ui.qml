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
    radius: 16 
    width: 678 

//    anchors
//    {
//        top: parent.top
//        topMargin: 24 
//        right: parent.right
//        rightMargin: 44 
//        left: parent.left
//        leftMargin: 24 
//        bottom: parent.bottom
//        bottomMargin: 20 

//    }

    // Install screen components
    Item 
    {
        id: contenetItemScreen
        data: dapContenetItemScreen
        anchors.fill: parent
    }
}
