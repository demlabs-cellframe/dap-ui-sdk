import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQml 2.12

import "qrc:/widgets"

Item
{
    id: root
    width: 17
    height: 18

    property string popupText: ""

    signal copyClicked()

    DapImageRender
    {
        id:networkAddressCopyButtonImage
        width: parent.width
        height: parent.height
        source: mouseArea.containsMouse ? "qrc:/Resources/" + pathTheme + "/icons/other/copy_hover.svg":
                                          "qrc:/Resources/" + pathTheme + "/icons/other/copy.svg"
//        source: mouseArea.containsMouse ? "qrc:/resources/icons/other/copy_hover.svg":
//                                          "qrc:/resources/icons/other/copy.svg"
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked:
        {
            print("DapCopyButton onClicked")
            dapMainWindow.infoItem.showInfo(
                        0,0,
                        dapMainWindow.width*0.5,
                        8,
                        popupText,
                        "qrc:/Resources/" + pathTheme + "/icons/other/Verified.svg")

            copyClicked()
        }
    }
}
