import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Rectangle
{
    id: selectorItem

    property int itemHorisontalBorder: 10
    property int itemVerticalBorder: -2
    property int viewerBorder: 4
    property int currentIndex: viewerItem.currentIndex
    property int defaultIndex: 0
    property font textFont: mainFont.dapFont.medium14
    property color textColor: currTheme.textColor
    property alias selectorModel: viewerItem.model
    property alias selectorListView: viewerItem

    signal itemSelected()

    implicitWidth: viewerItem.width

    border.color: currTheme.borderColor
    color: currTheme.backgroundMainScreen
    radius: height * 0.5

    ListView
    {
        id: viewerItem
        x: viewerBorder
        y: viewerBorder
        width: contentItem.width + viewerBorder * 2
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
            Rectangle
            {
                id: hl
                radius: height * 0.5

                property var gradColor:
                    viewerItem.model.get(viewerItem.currentIndex).color

//                color: gradColor

//                onWidthChanged:
//                {
//                    print("onColorChanged", gradColor, color)
//                }


//                color: (model.get(currentIndex).color === undefined ?
//                           currTheme.buttonColorNormalPosition0 :
//                           model.get(currentIndex).color)

                LinearGradient
                {
                    anchors.fill: parent
                    source: parent
                    start: Qt.point(0,parent.height/2)
                    end: Qt.point(parent.width,parent.height/2)
                    gradient:
                        Gradient {
                            GradientStop
                            {
                                id: grad1
                                position: 0
                                color: gradColor === undefined ?
                                           currTheme.buttonColorNormalPosition0 :
                                           gradColor
                            }
                            GradientStop
                            {
                                id: grad2
                                position: 1
                                color: gradColor === undefined ?
                                           currTheme.buttonColorNormalPosition1 :
                                           gradColor
                            }
                        }
                }
            }

        model: selectorModel

        delegate:
            Rectangle
            {
                id: frameItem
                width: textItem.width + itemHorisontalBorder * 2
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
                    bottomPadding: OS_WIN_FLAG ? 4 : 0
                    color: textColor
                    font: textFont
                    text: name

                }

                MouseArea
                {
                    anchors.fill: parent

                    onClicked: {
                        viewerItem.currentIndex = index
                        itemSelected()
                    }
                }
            }

    }

}
