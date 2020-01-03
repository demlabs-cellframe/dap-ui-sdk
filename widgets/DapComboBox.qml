import QtQuick 2.0
import QtQuick.Controls 2.0

DapComboBoxForm
{
    id:dapComboBox

    delegate:ItemDelegate
    {

        width: parent.width

        //Adjusting the height of the line, taking into account that the second element from the end may be the last
        height:{
            if(index != currentIndex)
            {
                if(index == (count - 2))
                {
                    if(index+1 == currentIndex) return heightListElement + bottomIntervalListElement
                    else return heightListElement + intervalListElement
                }
                if (index == count - 1) return heightListElement + bottomIntervalListElement
                return heightListElement + intervalListElement
            }
            else return 0
        }
        //Text item
        contentItem: Text {
            id:textDelegateComboBox
            anchors.fill: parent
            anchors.topMargin: paddingTopItemDelegate
            anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
            font:dapComboBox.font
            text: if(index != currentIndex)
                      return modelData;
                  else return ""
            color: hovered ? hilightColorText : normalColorText
        }


        //Indent from the bottom edge or the next line that will not stand out when you hover over the mouse
        background: Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: {
                if(index == count - 2)
                {
                    if(index+1 == currentIndex) return bottomIntervalListElement
                    else return intervalListElement
                }
                if (index == count - 1) return bottomIntervalListElement
                return intervalListElement
            }
            color: hovered ? hilightColor : normalColor
        }
        highlighted: parent.highlightedIndex === index
    }
}
