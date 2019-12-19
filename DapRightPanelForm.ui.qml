import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    id: dapRightPanel
    
    property alias rightPanel: dapRightPanel
    property alias header: dapHeader
    property Item headerData: Item { }
    property alias childRightPanels: dapChildRightPanels
    property  Item contentItemPanel: Item { }
    
    rightPanel.anchors.top: parent.top
    rightPanel.anchors.bottom: parent.bottom
    rightPanel.anchors.right: parent.right

    Item
    {
        id: dapHeader
        data: headerData
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    StackView
    {
        id: dapChildRightPanels
        initialItem: contentItemPanel
        anchors.top: dapHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
