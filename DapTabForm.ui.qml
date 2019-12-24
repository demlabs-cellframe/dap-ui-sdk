//**************************************************************************** 
//                  Implements a general view of the tab.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Page 
{
    ///@detalis Top panel widget.
    property DapTopPanel dapTopPanel
    ///@detalis Screen widget.
    property DapScreen dapScreen
    ///@detalis Separator widget.
    property alias dapSeparator: separator
    ///@detalis Right pane widget.
    property DapRightPanel dapRightPanel

    anchors.fill: parent
    // Install the top panel widget
    header: dapTopPanel
    // Install the screen widget and the right panel
    contentItem:
        Rectangle 
        {
            // Screen widget
            Item
            {
                id: screen
                data: dapScreen
                height: parent.height
                anchors.left: parent.left
                anchors.right: separator.left
            }
            // Separator widget
            Rectangle 
            {
                id: separator
                height: parent.height
                width: 3 * pt
                color: "green"
                anchors.right: rightPanel.left
            }
            // Right pane widget
            Item
            {
                id: rightPanel
                data: dapRightPanel
                height: parent.height
                anchors.right: parent.right
            }
        }
}
