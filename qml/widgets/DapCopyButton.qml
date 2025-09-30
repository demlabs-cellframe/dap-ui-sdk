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

    signal copyClicked()

    Image
    {
        id:networkAddressCopyButtonImage
        width: parent.width
        height: parent.height
        mipmap: true
        source: mouseArea.containsMouse ? pathResources + pathTheme + "/icons/other/copy_hover_small.svg":
                                          pathResources + pathTheme + "/icons/other/copy_small.svg"
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked:
        {
            print("DapCopyButton onClicked")
            showInfoNotification(popupText, "check_icon.png")
            copyClicked()
        }
    }
}
