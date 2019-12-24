//****************************************************************************
//                     Implements the right panel widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: rightPanel
    
    // Right panel widget
    property alias dapRightPanel: rightPanel
    // Right pane title widget
    property alias dapHeader: header
    // Right pane header data
    property Item dapHeaderData: Item { }
    // 
    property alias dapChildRightPanels: childRightPanels
    // 
    property  Item dapContentItemPanel: Item { }
    
    dapRightPanel.anchors.top: parent.top
    dapRightPanel.anchors.bottom: parent.bottom
    dapRightPanel.anchors.right: parent.right

    Item
    {
        id: header
        data: dapHeaderData
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    StackView
    {
        id: childRightPanels
        initialItem: dapContentItemPanel
        anchors.top: dapHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
