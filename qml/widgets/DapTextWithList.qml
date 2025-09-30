import QtQuick 2.4
import QtQml
import QtQuick.Controls 2.4
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts 1.3

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

    property font textAndMenuFont: mainFont.dapFont.medium12
    property string textColor: currTheme.white
    property string textHoverColor: currTheme.white

    property int heightTooltip: 80
    property int widthTooltip: 167

    property int minWidthTooltip: 167
    property int maxWidthTooltip: 300
    property int heightStrTooltip: 20
    property int heightDiffStrTooltip: 2

    property int marginTextTooltip: 12
    property string colorHeadStrTooltip: currTheme.gray
    property string colorStrTooltip: currTheme.white
    property alias listView: list
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
    }

    function updateToolPos()
    {
        tooltip.x = textItem.width - widthTooltip - offsetTooltipRight
        tooltip.y = textItem.y - heightTooltip
    }

    Text
    {
        id: textItem
        anchors.fill: parent
        font: textAndMenuFont
        color: textColor
        text: fullText
        verticalAlignment: Qt.AlignVCenter
        elide: Text.ElideMiddle

        FontMetrics {
            id: metrics
            font: textAndMenuFont
        }

        MouseArea
        {
            id: area
            anchors.fill: parent
            visible: showToolTip
            hoverEnabled: true

            DapCustomToolTip{

                function getWidthTootip()
                {
                    var maxStr = ""
                    var maxCount = 0
                    for(var i = 0; i < list.model.count; i++)
                    {
                        var tmpItem = list.model.get(i)
                        var tmpStr = tmpItem.name + " " + tmpItem.number + " " + tmpItem.token_name
//                        console.log(tmpStr)
                        if(maxCount < tmpStr.length)
                        {
                            maxCount = tmpStr.length
                            maxStr = tmpStr
                        }
                    }

                    var widthStr = metrics.boundingRect(maxStr).width
                    widthStr = widthStr + 2 * marginTextTooltip

                    if(maxWidthTooltip < widthStr)
                    {
                        widthStr = maxWidthTooltip
                    }
                    if(minWidthTooltip > widthStr)
                    {
                        widthStr = minWidthTooltip
                    }

                    widthTooltip = widthStr
                    return widthTooltip
                }

                function getHeightTooltip()
                {
                    var listCount = list.model.count;
                    var strHeight = metrics.boundingRect("A").height
                    var result = 2 * marginTextTooltip + (heightStrTooltip - heightDiffStrTooltip) * listCount
                    heightTooltip = result
                    return result;
                }

                onWidthChanged:
                {
                    updateToolPos()
                }

                onHeightChanged:
                {
                    updateToolPos()
                }

                id: tooltip
                bottomRect.visible: false

                parent: area
                width: getWidthTootip()
                height: getHeightTooltip()
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
                            height: heightStrTooltip
                            RowLayout {
                                spacing: 3
                                Text {
                                    id: nameRow
                                    text: name
                                    font: textAndMenuFont
                                    color: colorHeadStrTooltip
                                    horizontalAlignment: Text.AlignLeft
                                }

                                DapBigNumberText
                                {
                                    function getMaxSimbols()
                                    {
                                        var widthName = metrics.boundingRect(name).width
                                        var widthToken = metrics.boundingRect(token_name).width
                                        var maxWidth = widthTooltip - (widthName + marginTextTooltip * 2 + widthToken);
                                        var numSym = number.length
                                        var widthNumber = 0;
                                        for(var c = 0; c < number.length; c++)
                                        {
                                            widthNumber += metrics.advanceWidth(number[c])
                                            if(widthNumber > maxWidth)
                                            {
                                                numSym = c - 3
                                                break
                                            }
                                        }
                                        return numSym
                                    }

                                    id: numberRow
                                    Layout.fillWidth: true
                                    copyButtonVisible: false
                                    textFont: textAndMenuFont
                                    textColor: colorStrTooltip
                                    height: nameRow.height
                                    outSymbols: getMaxSimbols()
                                    fullNumber: number
                                    tokenName: token_name
                                    width: widthTooltip - offsetTooltipRight * 2
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

                onVisibleChanged:
                {
                    updateToolPos()
                }
            }

            onEntered:
            {
                parent.color = textHoverColor
            }

            onExited:
            {
                parent.color = textColor
            }
        }
    }
}
