//****************************************************************************
//                     Implements the right panel widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: frameRightPanel

    ///@detalis Signal initiating switching to the previous displayed form.
    signal nextActivated (var parametrsRightPanel)
    ///@detalis Signal initiating switching to the previous displayed form.
    signal previousActivated (var parametrsRightPanel)
    ///@detalis Next display right panel.
    property string dapNextRightPanel
    ///@detalis Previous display right panel.
    property string dapPreviousRightPanel
    ///@detalis Right panel widget.
    property alias dapFrame: frameRightPanel
    ///@detalis Right pane title widget.
    property alias dapHeader: headerRightPanel
    ///@detalis Right pane header data.
    property Item dapHeaderData
    ///@detalis Stack of right panels owned by current.
    property alias dapContentPanel: contentItemRightPanel
    ///@detalis Content of the current right panel.
    property  Item dapContentItemData
    ///@detalis dapRightPanelWidth External property for possibility to hiding RightPanel
    property int dapRightPanelWidth

    width: dapRightPanelWidth

    // Install right panel title
    Item
    {
        id: headerRightPanel
        data: dapHeaderData
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    // Install right panel content
    Item
    {
        id: contentItemRightPanel
        data: dapContentItemData
        anchors.top: headerRightPanel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
