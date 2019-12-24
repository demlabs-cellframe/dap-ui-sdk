//****************************************************************************
//                      Implements a screen widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: frame

    ///@detalis Frame widget.
    property alias dapFrame: frame
    ///@detalis Screen components.
    property Item dapContenetItemScreen

    anchors.fill: parent

    // Install screen components
    Item 
    {
        id: contenetItemScreen
        data: dapContenetItemScreen
    }
}
