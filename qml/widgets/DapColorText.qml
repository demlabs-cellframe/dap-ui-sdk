import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

import "qrc:/widgets"

Item
{
    id: textColorChanger

    property string fullText: "-"
    property bool showToolTip: true
    property bool alwaysHoverShow: false

    property int offsetTooltipRight: 0

    property alias horizontalAlign: textItem.horizontalAlignment
    property alias verticalAlign: textItem.verticalAlignment

    property font textFont: mainFont.dapFont.medium12
    property string textColor: currTheme.white
    property string textHoverColor: currTheme.white

    property int heightTooltip: 80
    property int widthTooltip: 167
    property int marginTextTooltip: 12
    property string colorHeadStrTooltip: currTheme.gray
    property string colorStrTooltip: currTheme.white
    property ListModel listModel: ListModel {

        ListElement {
            name: "Fee: "
            number: "0.1 tCELL"
        }
        ListElement {
            name: "Validator fee: "
            number: "0.05 tCELL"
        }
        ListElement {
            name: "Deposited: "
            number: "+ 0.02 mtCELL"
        }

    property alias textElement: textItem


    }

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
                bottomRect.height: 0

                parent: area
                width: widthTooltip
                height: heightTooltip
                contentItem: Item{
                    anchors.fill: parent

                    ListView {

                        id:list
                        anchors.fill: parent
                        model: listModel
                        anchors.leftMargin: marginTextTooltip
                        anchors.topMargin: marginTextTooltip
                        anchors.rightMargin: marginTextTooltip
                        anchors.bottomMargin: marginTextTooltip
                        delegate: Item{
                            width: parent.width
                            height: 20
                            Row {
                                spacing: 3
                                Text {
                                    text: name
                                    font: textFont
                                    color: colorHeadStrTooltip
                                    horizontalAlignment: Text.AlignLeft
                                }
                                Text {
                                    text: number
                                    font: textFont
                                    color: colorStrTooltip
                                    horizontalAlignment: Text.AlignLeft
                                }
                            }
                        }
                    }
                }

                visible: alwaysHoverShow && area.containsMouse ?
                                 true :
                                 area.containsMouse ?
                                     textItem.implicitWidth > textItem.width ?
                                         true :
                                         false : false

                onVisibleChanged: {
                    tooltip.x = textItem.width - widthTooltip - offsetTooltipRight
                    tooltip.y = textItem.y - heightTooltip
                }
            }

            onEntered: {
                parent.color = textHoverColor
            }

            onExited: {
                parent.color = textColor
            }
        }
    }
}
