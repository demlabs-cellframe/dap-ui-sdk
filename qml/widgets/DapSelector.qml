import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Rectangle
{
    id: selectorItem

    property int itemBorder: 10
    property int viewerBorder: 4
    property int currentIndex: viewerItem.currentIndex
    property alias selectorModel: viewerItem.model

    signal itemSelected()

/*    ListModel {
        id: selectorModel
        ListElement {
            name: "1m"
        }
        ListElement {
            name: "5m"
        }
        ListElement {
            name: "15m"
        }
        ListElement {
            name: "30m"
        }
        ListElement {
            name: "1h"
        }
        ListElement {
            name: "4h"
        }
        ListElement {
            name: "12h"
        }
        ListElement {
            name: "24h"
        }
        ListElement {
            name: "7D"
        }
        ListElement {
            name: "14D"
        }
        ListElement {
            name: "1M"
        }
        ListElement {
            name: "3M"
        }
        ListElement {
            name: "1Y"
        }
    }*/
/*    ListModel {
        id: selectorModel
        ListElement {
            name: "1 Day"
        }
        ListElement {
            name: "1 Week"
        }
        ListElement {
            name: "1 Month"
        }
        ListElement {
            name: "3 Month"
        }
        ListElement {
            name: "All"
        }
    }*/

    width: viewerItem.width

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

        highlight:
            Rectangle
            {
//                color: "lightsteelblue"
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
                                color:  currTheme.buttonColorNormalPosition1
                            }
                        }
                }
            }

//        model: selectorModel

        delegate:
            Rectangle
            {
                id: frameItem
//                width: 40
                width: childrenRect.width + itemBorder * 2
                height: childrenRect.height + itemBorder * 2

                color: "transparent"
//                border.color: "dark gray"
//                radius: height * 0.5

                Text
                {
                    id: textItem
//                    anchors.centerIn: parent
//                    x: itemBorder
//                    y: itemBorder
//                    height: selectorItem.height
//                            - itemBorder * 2 - viewerBorder * 2
                    x: itemBorder
                    y: itemBorder
                    height: selectorItem.height
                            - itemBorder * 2 - viewerBorder * 2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: currTheme.textColor
                    font: mainFont.dapFont.medium14
                    text: name

                    MouseArea {
//                        anchors.fill: parent
                        x: -itemBorder
                        y: -itemBorder
                        width: frameItem.width
                        height: frameItem.height

                        onClicked: {
                            viewerItem.currentIndex = index
                            itemSelected()
                        }
                    }
                }
            }

    }

}
