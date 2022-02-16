//****************************************************************************
//                     Implements the right panel widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

Page
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

    //Close button for right pannel
    property DapButton dapButtonClose:
    DapButton
    {
        id: buttonClose
        height: 20 * pt
        width: 20 * pt
        heightImageButton: 10 * pt
        widthImageButton: 10 * pt
        activeFrame: false
        normalImageButton: "qrc:/resources/icons/"+pathTheme+"/close_icon.png"
        hoverImageButton:  "qrc:/resources/icons/"+pathTheme+"/close_icon_hover.png"
    }

    //Header height
    dapHeader.height: 36 * pt
    dapRightPanelWidth: visible? 350 * pt : 0 * pt
    width: dapRightPanelWidth
    anchors {
        top: parent.top
        topMargin: 24 * pt
        right: parent.right
        rightMargin: 24 * pt
//        left: parent.left
//        leftMargin: 24 * pt
        bottom: parent.bottom
        bottomMargin: 20 * pt
    }

    background:
    Item {}

    DapRectangleLitAndShaded
    {
        anchors.fill: parent
        color: currTheme.backgroundElements
        radius: currTheme.radiusRectangle
        shadowColor: currTheme.shadowColor
        lightColor: currTheme.reflectionLight

        contentData:
            Item
            {
                anchors.fill: parent

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
    }


}
