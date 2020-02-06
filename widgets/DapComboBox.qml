import QtQuick 2.4
import QtQuick.Controls 2.0

DapComboBoxForm
{

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
                    id: rectTextComboBox

                    anchors.fill: parent
                    anchors.topMargin: paddingTopItemDelegate
                    anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
                    anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
                    color: "transparent"
                    width: (index !== currentIndex) ?
                               widthPopupComboBoxNormal - 2*sidePaddingActive - endRowPadding :
                               widthPopupComboBoxNormal - indicatorWidth - indicatorLeftInterval - 2 * sidePaddingActive

                    FontMetrics
                    {
                        id: comboBoxFontMetric
                        font: fontComboBox
                    }
                    property int comboBoxIndex: index
                    property int comboBoxCurrentIndex: currentIndex
                    Row
                    {
                        spacing: roleInterval
                        Repeater
                        {
                            model: comboBoxTextRole.length
                            Text
                            {
                                id: textDelegateComboBox
                                font: fontComboBox
                                color: hovered ? hilightColorText : normalColorText
                                horizontalAlignment:
                                {
                                    if(index === 0)
                                        return Text.AlignLeft;
                                    if(index === comboBoxTextRole.length - 1)
                                        return Text.AlignRight;
                                    return Text.AlignHCenter;
                                }
                                width: (rectTextComboBox.width - roleInterval * (comboBoxTextRole.length - 1)) / comboBoxTextRole.length
                                /*{
                                        if(index === 0)
                                            setModelDataWidth(rectTextComboBox.comboBoxIndex, comboBoxFontMetric, rectTextComboBox.width);
                                         console.log("comboBoxIndex", rectTextComboBox.comboBoxIndex, "index", index, "comboBoxTextRole", comboBoxTextRole[index])
                                         return comboBoxTextRoleWidth[index];
                                }*/
                                TextMetrics
                                {
                                    id: comboBoxTextMetric
                                    font: fontComboBox
                                    elide: Text.ElideRight
                                    text: getModelData(rectTextComboBox.comboBoxIndex, comboBoxTextRole[index])
                                    elideWidth: textDelegateComboBox.width
                                }
                                /*Component.onCompleted:
                                {
                                    setModelDataWidth(rectTextComboBox.comboBoxIndex, comboBoxFontMetric, rectTextComboBox.width);
                                    textDelegateComboBox.width = comboBoxTextRoleWidth[index];
                                }*/
                                text:
                                {
                                    if(rectTextComboBox.comboBoxIndex != rectTextComboBox.comboBoxCurrentIndex)
                                    {
                                        if(comboBoxTextMetric.elideWidth < comboBoxFontMetric.tightBoundingRect(comboBoxTextMetric.text).width)
                                            return checkElidedText(comboBoxFontMetric, comboBoxTextMetric);
                                        else if(comboBoxTextMetric.elideWidth === comboBoxFontMetric.tightBoundingRect(comboBoxTextMetric.text).width)
                                            return comboBoxTextMetric.text;
                                        else
                                            return comboBoxTextMetric.elidedText.replace('…', '..');
                                    }
                                    else
                                    {
                                        if(popup.visible)
                                        {
                                            if(comboBoxTextMetric.elideWidth < comboBoxFontMetric.tightBoundingRect(comboBoxTextMetric.text).width)
                                                mainLineText[index] = checkElidedText(comboBoxFontMetric, comboBoxTextMetric);
                                            else
                                            {
                                                if(comboBoxTextMetric.elideWidth === comboBoxFontMetric.tightBoundingRect(comboBoxTextMetric.text).width)
                                                    mainLineText[index] = comboBoxTextMetric.text;
                                                else
                                                    mainLineText[index] = comboBoxTextMetric.elidedText.replace('…', '..');
                                            }

                                        }
                                        else
                                        {
                                            if(index === 0)
                                                mainLineText[index] = comboBoxFontMetric.elidedText(getModelData(rectTextComboBox.comboBoxIndex, comboBoxTextRole[index]),
                                                                                                                        Text.ElideRight,
                                                                                                                        (widthPopupComboBoxNormal - indicatorWidth - indicatorLeftInterval),
                                                                                                                         Qt.TextShowMnemonic).replace('…', '..');
                                        }
                                        console.log("mainLineText[index]", "index", mainLineText[index], index)
                                            return "";

                                    }
                                }
                            }

                        }
                        Rectangle
                        {
                            id: endRowRectangle
                            height: parent.height
                            width: endRowPadding
                            color: "transparent"
                        }
                    }
                }



            //Indent from the bottom edge or the next line that will not stand out when you hover over the mouse
            background:
                Rectangle
                {
                    anchors.fill: parent
                    anchors.bottomMargin: {
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

    function checkElidedText(fontMetric, textMetric)
    {
        return textMetric.elidedText.substring(0, textMetric.elidedText.length-1) +
                ((fontMetric.tightBoundingRect(textMetric.elidedText.substring(0, textMetric.elidedText.length-1)).width +
                  fontMetric.tightBoundingRect(textMetric.text.charAt(textMetric.elidedText.length-1) + '..').width) < textMetric.elideWidth ?
                     (textMetric.text.charAt(textMetric.elidedText.length-1) + '..')
                   : '..');
    }

    function getModelData(rowIndex, modelRole)
    {
        return model.get(rowIndex)[modelRole];
    }

    function setModelDataWidth(rowIndex, fontMetric, rowWidth)
    {
        comboBoxTextRoleWidth = [0];
        mainLineText = [""];
        var neededWidth = 0;
        for(var i = 0; i < comboBoxTextRole.length; i++)
        {
            mainLineText[i] = "";
            comboBoxTextRoleWidth[i] = fontMetric.tightBoundingRect(getModelData(rowIndex, comboBoxTextRole[i])).width;
            neededWidth += comboBoxTextRoleWidth[i];
        }
        neededWidth += roleInterval * (comboBoxTextRole.length - 1);
        console.log("neededWidth", neededWidth, "rowWidth", rowWidth);
        var oneIndex = 0;
        if (neededWidth > rowWidth)
        {
            oneIndex = comboBoxTextRole.indexOf(mainLineRole);
            if(oneIndex === -1)
            {
                oneIndex = comboBoxTextRole.indexOf("text");
                if(oneIndex === -1)
                {
                    oneIndex = comboBoxTextRole.indexOf("name");
                    if(oneIndex === -1)
                        oneIndex = 0;
                }
            }
            if(comboBoxTextRoleWidth[oneIndex] < neededWidth - rowWidth)
            {
                comboBoxTextRoleWidth[oneIndex] = rowWidth / (comboBoxTextRole.length);
                var maxWidth = 0;
                var maxIndex = 0;
                while(neededWidth > rowWidth)
                {
                    maxWidth = comboBoxTextRoleWidth[0];
                    maxIndex = 0;
                    for(i = 0; i < comboBoxTextRoleWidth.length; i++)
                    {
                        if(maxWidth < comboBoxTextRoleWidth[i])
                        {
                            maxWidth = comboBoxTextRoleWidth[i];
                            maxIndex = i;
                        }
                    }
                    comboBoxTextRoleWidth[maxIndex] -= 10 * pt;
                    neededWidth = 0;
                    for(i = 0; i < comboBoxTextRoleWidth.length; i++)
                        neededWidth += comboBoxTextRoleWidth[i];
                }
            }
            return true;
        }
        if(neededWidth < rowWidth)
        {
            oneIndex = comboBoxTextRole.indexOf(mainLineRole);
            if(oneIndex === -1)
            {
                oneIndex = comboBoxTextRole.indexOf("text");
                if(oneIndex === -1)
                {
                    oneIndex = comboBoxTextRole.indexOf("name");
                    if(oneIndex === -1)
                        oneIndex = 0;
                }
            }
            comboBoxTextRoleWidth[oneIndex] += rowWidth - neededWidth;
            return true;
        }
        return false;
    }

}
