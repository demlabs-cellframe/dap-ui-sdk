//****************************************************************************
//                     Implements the right panel widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

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
    ///@detalis Right screen widget.
    ///@detalis Right pane header data.
    property Item dapHeaderData
    ///@detalis Stack of right panels owned by current.
    property alias dapContentPanel: contentItemRightPanel
    ///@detalis Content of the current right panel.
    property  Item dapContentItemData
    ///@detalis dapRightPanelWidth External property for possibility to hiding RightPanel
    property int dapRightPanelWidth

    width: dapRightPanelWidth
    anchors.top: parent.top
    anchors.topMargin: 24 * pt
    anchors.right: parent.right
    anchors.rightMargin: 20 * pt
    anchors.left: parent.left
    anchors.leftMargin: 24 * pt
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 24 * pt


    InnerShadow {
            id: topLeftSadow
            anchors.fill: frameRightPanel
            cached: true
            horizontalOffset: 5
            verticalOffset: 5
            radius: 4
            samples: 32
            color: "#2A2C33"
            smooth: true
            source: frameRightPanel
        }
    InnerShadow {
            anchors.fill: frameRightPanel
            cached: true
            horizontalOffset: -1
            verticalOffset: -1
            radius: 1
            samples: 32
            color: "#4C4B5A"
//            smooth: true
            source: topLeftSadow
        }

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
