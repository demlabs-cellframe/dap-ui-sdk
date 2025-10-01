import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml

import "qrc:/widgets"

Item
{
    id: root
    width: 16
    height: 16

    property string popupText: ""
    property alias mouseArea: mouseArea
    property bool hovered: mouseArea.containsMouse
    property bool setHovered: false

    property string hoverIcon: pathResources + pathTheme + "/icons/other/copy_hover_small.svg"
    property string normalIcon: pathResources + pathTheme + "/icons/other/copy_small.svg"


    signal copyClicked()

    Image
    {
        id:networkAddressCopyButtonImage
        width: parent.width
        height: parent.height
        mipmap: true
        source: mouseArea.containsMouse || setHovered ? hoverIcon:
                                                        normalIcon
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked:
        {
            clickFunc()
        }
    }

    function clickFunc()
    {
        print("DapCopyButton onClicked")
        showInfoNotification(popupText, "check_icon.png")
        copyClicked()
    }
}
