//****************************************************************************
//                  Implements a general view of the tab.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

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

    property alias rightPanel: rightPanel

    anchors.fill: parent

    // Install the top panel widget
    Item
    {
        id: topPanel
        data: dapTopPanel
        height: dapTopPanel.height
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    // Install the screen widget and the right panel
    Row
    {
        anchors.top: topPanel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // Screen widget
        Item
        {
            id: screen
            data: dapScreen
            height: parent.height
            width: parent.width - separator.width - rightPanel.width
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
            width: dapRightPanel.width
        }
    }

    DropShadow {
        anchors.fill: topPanel
        horizontalOffset: currTheme.hOffset
        verticalOffset: currTheme.vOffset
        radius: currTheme.radiusShadow
        color: currTheme.shadowColor
        source: topPanel
        spread: 0.1
    }
}
