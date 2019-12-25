//**************************************************************************** 
//                  Implements a general view of the tab.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle
{
    id: frameTab

    ///@detalis Top panel widget.
    property Item dapTopPanel
    ///@detalis Screen widget.
    property Item dapScreen
    ///@detalis Separator widget.
    property alias dapSeparator: separator
    ///@detalis Right pane widget.
    property Item dapRightPanel
    ///@detalis Tab frame.
    property alias dapFrame: frameTab

    anchors.fill: parent

    // Install the top panel widget
    Item
    {
        id: topPanel
        data: dapTopPanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    // Install the screen widget and the right panel
    Rectangle
    {
        anchors.top: topPanel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Row
        {
            anchors.fill: parent

            // Screen widget
            Item
            {
                id: screen
                data: dapScreen
                height: parent.height
            }
            // Separator widget
            Rectangle
            {
                id: separator
                height: parent.height
            }
            // Right pane widget
            Item
            {
                id: rightPanel
                data: dapRightPanel
                height: parent.height
            }
        }
    }
}
