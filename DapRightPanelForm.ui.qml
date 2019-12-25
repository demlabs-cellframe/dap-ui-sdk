//****************************************************************************
//                     Implements the right panel widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: frameRightPanel
    ///@detalis Right panel widget.
    property alias dapFrame: frameRightPanel
    ///@detalis Right pane title widget.
    property alias dapHeader: header
    ///@detalis Right pane header data.
    property Item dapHeaderData
    ///@detalis Stack of right panels owned by current.
    property alias dapContentItemPanel: contentItemPanel
    ///@detalis Content of the current right panel.
    property  Item dapContentItemData

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
    Item
    {
        id: contentItemPanel
        data: dapContentItemData
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
