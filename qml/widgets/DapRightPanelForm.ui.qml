import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Page
{
    id: root

    header: root.header


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
        rightMargin: 20 * pt
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
