import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Rectangle
{
    id: selectorItem

    property int itemHorisontalBorder: 10 * guiApp.scaleFactor
    property int itemVerticalBorder: -2 * guiApp.scaleFactor
    property int viewerBorder: 4 * guiApp.scaleFactor
    property int currentIndex: viewerItem.currentIndex
    property int defaultIndex: 0
    property font textFont: mainFont.dapFont.medium14
    property color textColor: currTheme.white
    property alias selectorModel: viewerItem.model
    property alias selectorListView: viewerItem

    property bool itemWidthEnabled: false

    property int itemWidth
    property int itemRadius

    signal itemSelected()

    implicitWidth: itemWidthEnabled ? 0 : viewerItem.width

    border.color: currTheme.input
    color: currTheme.mainBackground
    radius: height * 0.5

    ListView
    {
        id: viewerItem
        x: viewerBorder
        y: viewerBorder
        height: selectorItem.height
        width: contentWidth + viewerBorder * 2
        orientation: ListView.Horizontal
        interactive: false

        currentIndex: defaultIndex

        highlight:
        Rectangle
        {
            radius: itemRadius ? itemRadius : height * 0.5
            gradient: Gradient
            {
                orientation: Gradient.Horizontal
                GradientStop
                {
                    position: 0.0
                    color:
                    {
                        if (viewerItem.currentIndex < 0
                            || viewerItem.currentIndex >= viewerItem.count)
                            return currTheme.mainButtonColorNormal0

                        var c = viewerItem.model.get(viewerItem.currentIndex).color
                        return c !== undefined ? c : currTheme.mainButtonColorNormal0
                    }
                }
                GradientStop
                {
                    position: 1.0
                    color:
                    {
                        if (viewerItem.currentIndex < 0
                            || viewerItem.currentIndex >= viewerItem.count)
                            return currTheme.mainButtonColorNormal1

                        var c = viewerItem.model.get(viewerItem.currentIndex).color
                        return c !== undefined ? c : currTheme.mainButtonColorNormal1
                    }
                }
            }
        }

        model: selectorModel

        delegate:
            Rectangle
            {
                id: frameItem
                width: itemWidthEnabled ? itemWidth + itemHorisontalBorder * 2
                                        : textItem.width + itemHorisontalBorder * 2
                height: selectorItem.height - viewerBorder * 2

                color: "transparent"

                Text
                {
                    id: textItem
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: frameItem.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    bottomPadding: CURRENT_OS === "win" ? 4 * guiApp.scaleFactor : 0
                    color: textColor
                    font: textFont
                    text: name
                }

                MouseArea
                {
                    anchors.fill: parent

                    onClicked:
                    {
                        if(viewerItem.currentIndex !== index)
                        {
                            viewerItem.currentIndex = index
                            itemSelected()
                        }
                    }
                }
            }

    }

}
