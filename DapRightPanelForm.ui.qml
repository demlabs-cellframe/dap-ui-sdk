import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    id: dapRightPanel
    property alias rightPanel: dapRightPanel
//    property alias header: dapHeader
//    property Rectangle headerData: undefined
//    property alias childRightPanels: dapChildRightPanels
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: 350

    Rectangle
    {
        id: dapHeader
//        data: Rectangle
//        {
//            anchors.fill: parent

//        }
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
//        anchors.fill: parent
        height: 30
        color: "blue"
    }

//    StackView
//    {
//        id: dapChildRightPanels
//        initialItem: parent
//        anchors.top: dapHeader.bottom
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//    }
}
