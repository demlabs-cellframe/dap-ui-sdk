import QtQuick 2.4
import QtQml
import QtQuick.Controls 2.4
import Qt5Compat.GraphicalEffects

import "qrc:/widgets"

Item
{
    id: bigNumber

    property string fullText: "-"
    property bool showToolTip: true
    property bool alwaysHoverShow: false
    property alias tooltip: tooltip
    
    // Hover state property and signal
    property bool isHover: false
    signal hoverChanged(bool isHovered)
    signal clickedItem()

    property alias horizontalAlign: textItem.horizontalAlignment
    property alias verticalAlign: textItem.verticalAlignment

    property font textFont: mainFont.dapFont.medium12
    property string textColor: currTheme.white

    property alias textElement: textItem
    property alias hovered: area.containsMouse

    Text
    {
        id: textItem
        anchors.fill: parent
        font: textFont
        color: textColor
        text: fullText
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignLeft
        elide: Text.ElideMiddle

        MouseArea
        {
            id: area
            anchors.fill: parent
            visible: showToolTip
            hoverEnabled: true

            // Handle hover state changes
            onEntered:
            {
                isHover = true
                hoverChanged(true)
            }
            onExited:
            {
                isHover = false
                hoverChanged(false)
            }

            onClicked:
            {
                clickedItem()
            }

            DapCustomToolTip{
                id: tooltip
                parent: area
                visible: alwaysHoverShow && area.containsMouse ?
                    true :
                    area.containsMouse ?
                        textItem.implicitWidth > textItem.width ?
                            true :
                            false : false
                contentText: fullText
                onVisibleChanged: /*console.log(text.y, bigNumber.y, y)*/
                {
                    updatePos()
                }

            }
        }
    }
}
