import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects

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

    signal itemSelected()

    implicitWidth: viewerItem.width

    border.color: currTheme.input
    color: currTheme.mainBackground
    radius: height * 0.5

    ListView
    {
        id: viewerItem
        x: viewerBorder
        y: viewerBorder
        width: contentWidth + viewerBorder * 2
        height: selectorItem.height
//        clip: true
        orientation: ListView.Horizontal
        interactive: false

        currentIndex: defaultIndex

//        onCurrentItemChanged:
//        {
//            print("onCurrentItemChanged", model.get(currentIndex).color)
//            gradColor = model.get(currentIndex).color
////            hl.color = model.get(currentIndex).color
//        }

        highlight:
        Item {
            property var gradColor:
                viewerItem.model.get(viewerItem.currentIndex).color

            /* mask source */
            Rectangle {
                id: contenMask
                anchors.fill: parent
                radius: height * 0.5
                visible: false
            }
            /* mask */
            OpacityMask {
                anchors.fill: content
                source: content
                maskSource: contenMask
            }
            Canvas{
                id: content
                anchors.fill: parent
                opacity: 0
                onPaint: {
                    var ctx = getContext("2d")

                    var gradient = ctx.createLinearGradient(0,parent.height/2,parent.width,parent.height/2)
                    gradient.addColorStop(0, gradColor === undefined ?
                                              currTheme.mainButtonColorNormal0 :
                                              gradColor)
                    gradient.addColorStop(1, gradColor === undefined ?
                                              currTheme.mainButtonColorNormal1 :
                                              gradColor)
                    ctx.fillStyle = gradient
                    ctx.fillRect(0,0,parent.width,parent.height)
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
//                    x: itemHorisontalBorder
//                    y: itemVerticalBorder
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

                    onClicked: {
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
