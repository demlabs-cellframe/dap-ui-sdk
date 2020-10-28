import QtQuick 2.0
import QtQuick.Controls 2.0

Button
{
    id: root

    property alias backgroundSubcontroll: background
    property alias iconSubcontroll: iconSubcontroll
    property alias textSubcontroll: textSubcontroll
    property string iconSource
    property string hoverIconSource


    property bool isHovered: false

    background: Item {
    }

    contentItem:
        Rectangle
        {
            id: background
            anchors.fill: parent

            Text
            {
                id: textSubcontroll
                anchors.fill: parent
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
                text: qsTr(root.text)
            }

            Image
            {
                id: iconSubcontroll
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: root.hovered ? hoverIconSource : iconSource
            }
        }
}
