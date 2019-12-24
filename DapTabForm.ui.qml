//**************************************************************************** 
//                  Implements a general view of the tab.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Page 
{
    // Top panel widget
    property DapTopPanelForm dapTopPanelForm
    // Screen widget
    property DapScreenForm dapScreenForm
    // Separator widget
    property alias dapSeparator: separator
    // Right pane widget
    property DapRightPanelForm dapRightPanelForm

    anchors.fill: parent
    // Install the top panel widget
    header: dapTopPanelForm
    // Install the screen widget and the right panel
    contentItem:
        Rectangle 
        {
            // Screen widget
            Item
            {
                id: screenForm
                data: dapScreenForm
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
                anchors.right: rightPanelForm.left
            }
            // Right pane widget
            Item
            {
                id: rightPanelForm
                data: dapRightPanelForm
                height: parent.height
                anchors.right: parent.right
            }
        }
}
