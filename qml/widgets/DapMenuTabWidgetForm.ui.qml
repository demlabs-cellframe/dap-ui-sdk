//****************************************************************************
//                Implements the widget of the main menu panel.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle
{
    id: frameMenuTab

    ///@detalis Main menu panel widget frame.
    property alias dapFrameMenuTab: frameMenuTab
//    property alias dapFrameMenuTabColor: color
    ///@detalis Main menu bar widget.
    property ListView dapMenuWidget
    ///@detalis Path to the selected tab.
    property string pathScreen

    focus: true

    // Install the widget of the main menu panel
    Item
    {
        id: menuTab
        data: dapMenuWidget
        anchors.fill: parent
    }
}
