import QtQuick 2.4
import QtQml
import QtQuick.Controls 2.4
import Qt5Compat.GraphicalEffects
import "qrc:/widgets"

Text
{
    property string foundLink: ""

    signal linkClicked()

    id: textItem
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignTop
    wrapMode: Text.WordWrap
    font: mainFont.dapFont.regular13
    color: currTheme.white
    textFormat: Text.RichText

    onLinkActivated:
    {
        foundLink = link
        linkClicked()
    }

    MouseArea {
        property string linkAtPosition: ""

        id: area
        hoverEnabled: true
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: textItem.linkActivated(linkAtPosition)
        onPositionChanged:
        {
            var pos = area.mapToItem(textItem, mouseX, mouseY);
            linkAtPosition = textItem.linkAt(pos.x, pos.y);
            if (linkAtPosition !== "") {
                area.cursorShape = Qt.PointingHandCursor;
            } else {
                area.cursorShape = Qt.ArrowCursor;
            }
        }
    }
}
