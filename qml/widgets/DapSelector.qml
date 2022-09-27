import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Rectangle
{
    id: selectorItem

    property int itemHorisontalBorder: 10
    property int itemVerticalBorder: -2
    property int viewerBorder: 4
    property int currentIndex: viewerItem.currentIndex
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

        highlight:
            Rectangle
            {
                radius: height * 0.5

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
                                position: 0;
                                color: currTheme.buttonColorNormalPosition0
                            }
                            GradientStop
                            {
                                id: grad2
                                position: 1;
                                color: currTheme.buttonColorNormalPosition1
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
                    color: currTheme.textColor
                    font: mainFont.dapFont.medium14
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
