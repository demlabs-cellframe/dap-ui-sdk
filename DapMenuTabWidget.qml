import QtQuick 2.4

DapMenuTabWidgetForm {
    Rectangle {
        id: test
        width: viewMenuTab.width
        height: 60 * pt
        color: "transparent"

        Image
        {
            id: iconItem
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 24 * pt
            height: 18 * pt
            width: 18 * pt
            source: normalIcon
        }

        Text {
            id: textItem
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: iconItem.right
            anchors.leftMargin: 18 * pt
            font.family: "Roboto"
            font.pixelSize: 16 * pt
            color: "#FFFFFF"
            text: name
        }
    }
}
