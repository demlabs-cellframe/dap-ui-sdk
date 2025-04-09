import QtQuick
import QtQml

import "qrc:/widgets"

Text
{
    property string fullText: "-"
    property bool showToolTip: true
    property bool alwaysHoverShow: false
    property string textColor: currTheme.white

    id: textItem

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

