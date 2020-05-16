import QtQuick 2.4
import QtQuick.Controls 2.0

DapComboBoxForm
{
    Component.onCompleted:
    {
        if(model.count > 0)
        {
            if(isDefaultNeedToAppend && model.get(0, comboBoxTextRole[0]) !== mainLineText)
            {
                if(comboBoxTextRole[0] === "name")
                    model.insert(0, {name: mainLineText});
                else if(comboBoxTextRole[0] === "text")
                    model.insert(0, {text: mainLineText});
                currentIndex = 0;
            }
        }
    }

    delegate:
        ItemDelegate
        {
            width: parent.width

            //Adjusting the height of the line, taking into account that the second element from the end may be the last
            height:
                {
                    if(index != currentIndex)
                    {
                        if(index == (count - 2))
                        {
                            if(index+1 == currentIndex)
                                return heightListElement + bottomIntervalListElement
                            else
                                return heightListElement + intervalListElement
                        }
                        if (index == count - 1)
                            return heightListElement + bottomIntervalListElement
                        return heightListElement + intervalListElement
                    }
                    else return 0
                }

            //Text item
            contentItem:
                Rectangle
                {
                    id: rectangleTextComboBox
                    anchors.fill: parent
                    anchors.topMargin: paddingTopItemDelegate
                    anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
                    anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
                    color: "transparent"

                    property int comboBoxIndex: index
                    property int comboBoxCurrentIndex: currentIndex

                    FontMetrics
                    {
                        id: comboBoxFontMetric
                    }

                    Row
                    {
                        id: textRow
                        width: rectangleTextComboBox.width - ((index != currentIndex) ?
                                                                  endRowPadding :
                                                                  (indicatorWidth + indicatorLeftInterval)
                                                              )
                        height: rectangleTextComboBox.height
                        spacing: roleInterval
                        property var elTextArray: []

                        Repeater
                        {
                            id: textRepeater
                            model: comboBoxTextRole.length

                            DapText
                            {
                                id: textComboBoxDelegate
                                width: (textRow.width - roleInterval * (comboBoxTextRole.length - 1)) / comboBoxTextRole.length
                                enabled: false
                                fontDapText: (fontComboBox.length > index) ?
                                                 fontComboBox[index] :
                                                 fontComboBox[0];
                                textColor: hovered ? colorTextComboBox[index][1] : colorTextComboBox[index][0]
                                fullText: getModelData(rectangleTextComboBox.comboBoxIndex, comboBoxTextRole[index])
                                textElide: (elideTextComboBox.length > index) ?
                                               elideTextComboBox[index] :
                                               elideTextComboBox[0];
                                horizontalAlignment: (alignTextComboBox.length > index) ?
                                               alignTextComboBox[index] :
                                               alignTextComboBox[0];
                                onElTextChanged: textRow.elTextArray[index] = elText

                                Component.onCompleted:
                                {
                                    if(rectangleTextComboBox.comboBoxIndex == rectangleTextComboBox.comboBoxCurrentIndex)
                                    {

                                        var tmp = mainRow;
                                        tmp[index] = elText;
                                        mainRow = tmp;

                                        if(index == 0)
                                        {
                                            comboBoxFontMetric.font = (fontComboBox.length > index) ?
                                                      fontComboBox[index] :
                                                      fontComboBox[0];
                                            mainLineText = comboBoxFontMetric.elidedText(fullText, Text.ElideRight, rectangleTextComboBox.width, Qt.TextShowMnemonic);
                                        }
                                    }
                                }

                            }

                        }

                    }
                    Component.onCompleted:
                    {
                        if(rectangleTextComboBox.comboBoxCurrentIndex !== -1)
                            updateMainRow(comboBoxFontMetric, rectangleTextComboBox.comboBoxIndex, rectangleTextComboBox.comboBoxCurrentIndex, textRow.elTextArray, (widthPopupComboBoxNormal - indicatorWidth - indicatorLeftInterval));

                    }
                    onComboBoxCurrentIndexChanged:
                    {
                        if(rectangleTextComboBox.comboBoxCurrentIndex !== -1)
                            updateMainRow(comboBoxFontMetric, rectangleTextComboBox.comboBoxIndex, rectangleTextComboBox.comboBoxCurrentIndex, textRow.elTextArray, rectangleTextComboBox.width);

                    }
                }



            //Indent from the bottom edge or the next line that will not stand out when you hover over the mouse
            background:
                Rectangle
                {
                    anchors.fill: parent
                    anchors.bottomMargin:
                    {
                        if(index == count - 2)
                        {
                            if(index+1 == currentIndex)
                                return bottomIntervalListElement
                            else
                                return intervalListElement
                        }
                        if (index == count - 1)
                            return bottomIntervalListElement
                        return intervalListElement
                    }
                    color: hovered ? hilightColor : normalColor
                }
            highlighted: parent.highlightedIndex === index
        }


    function getModelData(rowIndex, modelRole)
    {
        return model.get(rowIndex)[modelRole];
    }


    function updateMainRow(fm, cbIndex, cbCurrentIndex, elTextArray, width)
    {
        if(cbIndex === cbCurrentIndex)
        {
            for(var i = 0; i < comboBoxTextRole.length; i++)
            {
                fm.font = (fontComboBox.length > i) ?
                            fontComboBox[i] :
                            fontComboBox[0];

                var tmp = mainRow;
                tmp[i] = elTextArray[i];
                mainRow = tmp;

                if(i == 0)
                    mainLineText = fm.elidedText(getModelData(cbCurrentIndex, comboBoxTextRole[0]), Text.ElideRight, width, Qt.TextShowMnemonic);
            }
        }
    }

}
