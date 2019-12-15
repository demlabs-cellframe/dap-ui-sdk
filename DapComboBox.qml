import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0


///About property
///@hilightColor - color of the selected item
///@normalColor - color of item
///@normalColorText and @hilightColorText - normal and selected text color
///@normalColorTopText and @hilightColorTopText - text color in the main line in normal and active state
///@fontSizeComboBox - font size for the entire widget  (px).
///@widthPopupComboBoxActive and @widthPopupComboBoxNormal - width of the combo box
///                                                 in the active state and in the normal state
///@heightComboBoxNormal and @heightComboBoxActive - height of the combo box
///                                                 in the active state and in the normal state
///@sidePaddingActive and @sidePaddingNormal - padding width of the combo box in the active state
///                                                                     and in the normal state
///@sidePaddingActive and @sidePaddingNormal - sets the indent from the edge of the right and left
///                                           edges of the parent in the active and normal state
///@topIndentActive and @topIndentNormal
///@bottomIndentActive and @bottomIndentNormal
/// - sets the indent from the edge of the upper and lower edges of the parent in the active and normal state
///@indicatorImageNormal and @indicatorImageActive - indicator picture address for active and normal state
///@indicatorWidth and @indicatorHeight - indicator width and height
///@heightListElement - List item height
///@intervalListElement - spacing between items in a list
///@bottomIntervalListElement - spacing from bottom to bottom
///@topEffect - Using an effect for the top element
///@colorTopNormalDropShadow - Color of the shadow effect of the combo box when minimized
///@colorDropShadow - Unboxed shadow color in expanded state
///@paddingTopItemDelegate - indent above and below from item delegate

ComboBox {

    property string normalColorText: "#070023"
    property string hilightColorText: "#FFFFFF"

    property string normalColorTopText:normalColorText
    property string hilightColorTopText:normalColorText

    property string normalColor: "#FFFFFF"
    property string hilightColor: "#330F54"
    property string normalTopColor: normalColor
    property string hilightTopColor: normalColor

    property int fontSizeComboBox: 16*pt

    property int widthPopupComboBoxNormal: parent.width
    property int widthPopupComboBoxActive: widthPopupComboBoxNormal

    property int heightComboBoxNormal: parent.height
    property int heightComboBoxActive: heightComboBoxNormal

    property int sidePaddingNormal:16 * pt
    property int sidePaddingActive:sidePaddingNormal

    property int topIndentNormal:12 * pt
    property int topIndentActive:topIndentNormal

    property int bottomIndentNormal:14 * pt
    property int bottomIndentActive:bottomIndentNormal

    property int paddingTopItemDelegate:8 * pt
    property int paddingBottomItemDelegate:paddingTopItemDelegate

    property int heightListElement: 32 * pt
    property int intervalListElement: 10 * pt
    property int bottomIntervalListElement:intervalListElement
    property bool topEffect: true

    property string indicatorImageNormal: "qrc:/res/icons/ic_arrow_drop_down_dark_blue.png"
    property string indicatorImageActive: "qrc:/res/icons/ic_arrow_drop_up_dark_blue.png"
    property int indicatorWidth: 24*pt
    property int indicatorHeight: indicatorWidth

    property string colorTopNormalDropShadow:"#00000000"
    property string colorDropShadow:"#40ABABAB"

    id: customComboBox
    width: popup.visible ? widthPopupComboBoxActive : widthPopupComboBoxNormal
    height: popup.visible ? heightComboBoxActive : heightComboBoxNormal
    anchors.verticalCenter: parent.verticalCenter

    delegate:ItemDelegate {
        width: parent.width

        ///Adjusting the height of the line, taking into account that the second element from the end may be the last.
        height:{
            if(index != currentIndex)
            {
                if(index == customComboBox.count - 2)
                {
                    if(index+1 == currentIndex) return heightListElement + bottomIntervalListElement
                    else return heightListElement + intervalListElement
                }
                if (index == customComboBox.count - 1) return heightListElement + bottomIntervalListElement
                return heightListElement + intervalListElement
            }
            else return 0
        }
        ///Text item
        contentItem: Text {
            id:textDelegateComboBox
            anchors.fill: parent
            anchors.topMargin: paddingTopItemDelegate
            anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
            font.family: fontRobotoRegular.name
            font.pixelSize: fontSizeComboBox
            text: {if(index != currentIndex) return modelData;}
            color: hovered ? hilightColorText : normalColorText
        }


        //Indent from the bottom edge or the next line that will not stand out when you hover over the mouse
        background: Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: {
                if(index == customComboBox.count - 2)
                {
                    if(index+1 == currentIndex) return bottomIntervalListElement
                    else return intervalListElement
                }
                if (index == customComboBox.count - 1) return bottomIntervalListElement
                return intervalListElement
            }

            color: hovered ? hilightColor : normalColor
            //the list text

        }
        highlighted: parent.highlightedIndex === index
    }

    //Icon icon near the text (arrow)
    indicator: Image {
        source: parent.popup.visible ? indicatorImageActive : indicatorImageNormal
        width: indicatorWidth
        height: indicatorHeight
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
    }

    ///Defining the background for the main line
    background: Rectangle {
        anchors.fill: parent
        color: parent.popup.visible ? hilightTopColor : normalTopColor
        radius: 2 * pt
        height: parent.height

    }
    ///Main line text settings
    contentItem: Text {
        anchors.fill: parent
        anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
        text: parent.displayText
        font.family: fontRobotoRegular.name
        font.pixelSize: fontSizeComboBox
        color: popup.visible ? hilightColorTopText : normalColorTopText
        verticalAlignment: Text.AlignVCenter
    }

    ///Customize drop-down list with shadow effect
    popup: Popup {
        y: parent.height - 1
        width: parent.width + 1
        padding: 1
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: customComboBox.popup.visible ? customComboBox.delegateModel : null
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            width: customComboBox.background.width
            color: parent.color
            Rectangle {
                id: contentCorner
                anchors.fill: parent
            }

            DropShadow {
                anchors.fill: parent
                source: contentCorner
                verticalOffset: 9 * pt
                samples: 13 * pt
                color: colorDropShadow
            }
        }
    }

    ///Shadow effect for the top element.
    DropShadow {
        anchors.fill: if(topEffect) parent
        source: if(topEffect) background
        verticalOffset: if(topEffect) 9 * pt
        samples: if(topEffect) 13 * pt
        color: if(topEffect) customComboBox.popup.visible ? colorDropShadow : colorTopNormalDropShadow
    }
}
