import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ComboBox 
{
    ///@detalis normalColorText Text color in normal state.
    property string normalColorText
    ///@detalis hilightColorText Text color in selected state.
    property string hilightColorText
    ///@detalis normalColorTopText Text color in the main line in normal state.
    property string normalColorTopText
    ///@detalis hilightColorTopText Text color in the main line in active state.
    property string hilightColorTopText
    ///@detalis normalColor Item color in normal state.
    property string normalColor
    ///@detalis hilightColor Item color in selected state.
    property string hilightColor
    ///@detalis normalTopColor Top string color in normal state.
    property string normalTopColor
    ///@detalis hilightTopColor Top string color in selected state.
    property string hilightTopColor
    ///@detalis widthPopupComboBoxNormal Width of the combo box in the normal state.
    property int widthPopupComboBoxNormal
    ///@detalis widthPopupComboBoxActive Width of the ComboBox in the active state.
    property int widthPopupComboBoxActive
    ///@detalis heightComboBoxNormal Height of the ComboBox in the normal state.
    property int heightComboBoxNormal
    ///@detalis heightComboBoxActive Height of the ComboBox in the active state.
    property int heightComboBoxActive
    ///@detalis sidePaddingNormal: Sets the indent from the edge of the right and left edges of the parent in the normal state.
    property int sidePaddingNormal
    ///@detalis sidePaddingActive Sets the indent from the edge of the right and left edges of the parent in the active state.
    property int sidePaddingActive
    ///@detalis topIndentNormal Sets the indent from the edge of the upper of the parent in the normal state.
    property int topIndentNormal
    ///@detalis topIndentActive Sets the indent from the edge of the upper of the parent in the active state.
    property int topIndentActive
    ///@detalis bottomIndentNormal Sets the indent from the edge of the lower of the parent in the normal state.
    property int bottomIndentNormal
    ///@detalis bottomIndentActive Sets the indent from the edge of the lower of the parent in the active state.
    property int bottomIndentActive
    ///@detalis paddingTopItemDelegate Indent above from item delegate.
    property int paddingTopItemDelegate
    ///@detalis paddingBottomItemDelegate Indent below from item delegate.
    property int paddingBottomItemDelegate
    ///@detalis heightListElement List item height.
    property int heightListElement
    ///@detalis intervalListElement Spacing between items in a list.
    property int intervalListElement
    ///@detalis bottomIntervalListElement Spacing from bottom to bottom.
    property int bottomIntervalListElement
    ///@detalis topEffect Using an effect for the top element.
    property bool topEffect
    ///@detalis indicatorImageNormal Indicator picture address for normal state.
    property string indicatorImageNormal
    ///@detalis indicatorImageActive Indicator picture address for active state.
    property string indicatorImageActive
    ///@detalis indicatorWidth Indicator width.
    property int indicatorWidth
    ///@detalis indicatorHeight Indicator height.
    property int indicatorHeight
    /// colorTopNormalDropShadow Color of the shadow effect of the combo box when minimized.
    property string colorTopNormalDropShadow
    ///@detalis colorDropShadow Unboxed shadow color in expanded state.
    property string colorDropShadow
    ///@detalis fontComboBox Font setting combobox.
    property alias fontComboBox:customComboBox.font

    id: customComboBox
    width: popup.visible ? widthPopupComboBoxActive : widthPopupComboBoxNormal
    height: popup.visible ? heightComboBoxActive : heightComboBoxNormal
    anchors.verticalCenter: parent.verticalCenter

    //Icon icon near the text (arrow)
    indicator: 
        Image 
        {
            source: parent.popup.visible ? indicatorImageActive : indicatorImageNormal
            width: indicatorWidth
            height: indicatorHeight
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
        }

    //Defining the background for the main line
    background: 
        Rectangle 
        {
            anchors.fill: parent
            color: parent.popup.visible ? hilightTopColor : normalTopColor
            radius: 2 * pt
            height: parent.height
        }
    //Main line text settings
    contentItem: 
        Text 
        {
            id:textTopComboBox
            anchors.fill: parent
            anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
            text: parent.displayText
            font: parent.font
            color: popup.visible ? hilightColorTopText : normalColorTopText
            verticalAlignment: Text.AlignVCenter
        }

    //Customize drop-down list with shadow effect
    popup: 
        Popup 
        {
            y: parent.height - 1
            width: parent.width + 1
            padding: 1
            contentItem:
                ListView 
                {
                    clip: true
                    implicitHeight: contentHeight
                    model: customComboBox.popup.visible ? customComboBox.delegateModel : null
                    ScrollIndicator.vertical: ScrollIndicator {}
                }
    
            background: 
                Rectangle
                {
                    width: customComboBox.background.width
                    color: normalColor
                    Rectangle 
                    {
                        id: contentCorner
                        anchors.fill: parent
                    }
        
                    DropShadow 
                    {
                        anchors.fill: parent
                        source: contentCorner
                        verticalOffset: 9 * pt
                        samples: 13 * pt
                        color: colorDropShadow
                    }
                }
        }

        //Shadow effect for the top element.
        DropShadow 
        {
            anchors.fill: if (topEffect)
                              parent
            source: if (topEffect)
                        background
            verticalOffset: if (topEffect)
                                9 * pt
                            else 0
            samples: if (topEffect)
                         13 * pt
                        else 0
            color: if (topEffect)
                       customComboBox.popup.visible ? colorDropShadow : colorTopNormalDropShadow
                    else "#00000000"
        }
    }
