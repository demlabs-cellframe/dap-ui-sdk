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
                Text
                {
                    id:textDelegateComboBox
                    anchors.fill: parent
                    anchors.topMargin: paddingTopItemDelegate
                    anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
                    anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
                    font: fontComboBox

                    //Calculates various properties of a given string of text for a particular font
                    TextMetrics
                    {
                        id: tm
                        font: fontComboBox
                        elide: Text.ElideRight
                        text: model[comboBoxTextRole]
                        elideWidth:
                            {
                                if(index != currentIndex)
                                    return widthPopupComboBoxActive - 2*sidePaddingActive;
                                else
                                    return widthPopupComboBoxNormal - indicatorWidth - indicatorLeftInterval;
                            }
                    }
                    FontMetrics
                    {
                        id: fm
                        font: fontComboBox
                    }
                    text:
                        {
                            if(index != currentIndex)
                            {
                                if(tm.elidedText.length < tm.text.length)
                                    return tm.elidedText.substring(0, tm.elidedText.length-1) +
                                            ((fm.tightBoundingRect(tm.elidedText.substring(0, tm.elidedText.length-1)).width +
                                              fm.tightBoundingRect(tm.text.charAt(tm.elidedText.length-1) + '..').width) < tm.elideWidth ?
                                                 (tm.text.charAt(tm.elidedText.length-1) + '..')
                                                    : '..');
                                return tm.elidedText.replace('…', '..');
                            }
                            else
                            {
                                if(tm.elidedText.length < tm.text.length)
                                    mainLineText = tm.elidedText.substring(0, tm.elidedText.length-1) +
                                            ((fm.tightBoundingRect(tm.elidedText.substring(0, tm.elidedText.length-1)).width +
                                              fm.tightBoundingRect(tm.text.charAt(tm.elidedText.length-1) + '..').width) < tm.elideWidth ?
                                                 (tm.text.charAt(tm.elidedText.length-1) + '..')
                                                    : '..');
                                else
                                    mainLineText = tm.elidedText.replace('…', '..');
                                return "";
                            }
                        }
                    color: hovered ? hilightColorText : normalColorText
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
}
