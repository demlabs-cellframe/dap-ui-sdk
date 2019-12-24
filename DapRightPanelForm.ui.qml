//****************************************************************************
//                     Implements the right panel widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: rightPanel
    
    ///@detalis Right panel widget.
    property alias dapRightPanel: rightPanel
    ///@detalis Right pane title widget.
    property alias dapHeader: header
    ///@detalis Right pane header data.
    property Item dapHeaderData: Item { }
    ///@detalis Stack of right panels owned by current.
    property alias dapChildRightPanels: childRightPanels
    ///@detalis Content of the current right panel.
    property  Item dapContentItemPanel: Item { }
    
    dapRightPanel.anchors.top: parent.top
    dapRightPanel.anchors.bottom: parent.bottom
    dapRightPanel.anchors.right: parent.right

    // Install right panel title
    Item
    {
        id: header
        data: dapHeaderData
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    // Install right panel content
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
