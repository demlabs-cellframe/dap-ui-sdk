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
    ///@detalis Main menu bar widget.
    property alias dapMenuTab: menuTab

    color: "#070023"
    focus: true

    // Install the widget of the main menu panel
    ListView
    {
        id: menuTab
        anchors.fill: parent
        delegate: itemMenuTabDelegate
        spacing: 3 * pt
        clip: true
        interactive: false
    }
}
