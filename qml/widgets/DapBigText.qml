import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

import "qrc:/widgets"

Item
{
    id: bigNumber

    property string fullText: "-"
    property bool showToolTip: true
    property bool alwaysHoverShow: false

    property alias horizontalAlign: textItem.horizontalAlignment
    property alias verticalAlign: textItem.verticalAlignment

    property font textFont: mainFont.dapFont.medium12
    property string textColor: currTheme.white

    property alias textElement: textItem

    Text
    {
        id: textItem
        anchors.fill: parent
        font: textFont
        color: textColor
        text: fullText
        verticalAlignment: Qt.AlignVCenter
        elide: Text.ElideMiddle

        MouseArea
        {
            id: area
            anchors.fill: parent
            visible: showToolTip
            hoverEnabled: true

            DapCustomToolTip{
                id: tooltip
                parent: area
                globalOFF: alwaysHoverShow && area.containsMouse ?
                    true :
                    area.containsMouse ?
                        textItem.implicitWidth > textItem.width ?
                            true :
                            false : false
                contentText: fullText
            }
        }
    }
}
