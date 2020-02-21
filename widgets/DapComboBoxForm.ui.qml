import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ComboBox 
{
    id: dapComboBox

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
    ///@detalis paddingTopItemDelegate Indent above from item delegate.
    property int paddingTopItemDelegate
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
    ///@detalis indicatorLeftInterval Space between indicator border and text border.
    property int indicatorLeftInterval
    /// colorTopNormalDropShadow Color of the shadow effect of the combo box when minimized.
    property string colorTopNormalDropShadow
    ///@detalis colorDropShadow Unboxed shadow color in expanded state.
    property string colorDropShadow

    ///@detalis fontComboBox Font setting combobox.
    property var fontComboBox: []
    ///@detalis colorTextComboBox Color text setting combobox: array [normalColorText, hilightColorText] for every role
    property var colorTextComboBox: []
    ///@detalis colorMainTextComboBox Color mainline text setting combobox: array [normalTopColorText, hilightTopColorText] for every role
    property var colorMainTextComboBox: []
    ///@detalis elideTextComboBox Elide setting combobox.
    property var elideTextComboBox: [Text.ElideRight]
    ///@detalis alignTextComboBox Align setting combobox.
    property var alignTextComboBox: [Text.AlignLeft]

    ///@detalis comboBoxTextRole The model roles used for the ComboBox.
    property var comboBoxTextRole: ["text"]
    ///@detalis comboBoxTextRoleWidth The model roles width used for the ComboBox.
    property var comboBoxTextRoleWidth: [100]
    ///@detalis roleInterval The width between text of model roles used for the ComboBox.
    property int roleInterval: 10
    ///@detalis endRowPadding The width of padding at the end of one row at ComboBox where it is need.
    property int endRowPadding: 10

    ///@detalis mainRow The model role for the main line of cloded ComboBox.
    property var mainRow: [""]
    //@detalis mainLineText Text without unneccesary part.
    property string mainLineText


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
            height: parent.height
        }

    //Main line text settings
    contentItem:
        Rectangle
        {
            id: mainRectangle
            anchors.fill: parent
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
             color: "transparent"

             Row
            {
                id: topComboBoxRow
                width: widthPopupComboBoxNormal - indicatorWidth - indicatorLeftInterval
                height: mainRectangle.height
                spacing: roleInterval
                Repeater
                {
                    id: textCurrentRepeater
                    model: (popup.visible) ? comboBoxTextRole.length : 1
                    Text
                    {
                        anchors.verticalCenter: parent.verticalCenter
                        text: (popup.visible) ?
                                  (index < mainRow.length ? mainRow[index] : ""):
                                  mainLineText
                        font: popup.visible ?
                            ((fontComboBox.length > index) ?
                                  fontComboBox[index] :
                                  fontComboBox[0]
                             ) :
                                  fontComboBox[0];
                        width: popup.visible ?
                                 (topComboBoxRow.width - roleInterval * (comboBoxTextRole.length - 1)) / comboBoxTextRole.length :
                                   contentWidth
                        color: popup.visible ?
                                   colorMainTextComboBox[index][1] :
                                   colorMainTextComboBox[index][0]
                        horizontalAlignment: popup.visible ?
                                                 alignTextComboBox[index] :
                                                 Text.AlignLeft
                    }
                }
            }
        }


    //Customize drop-down list with shadow effect
    popup: 
        Popup 
        {
            y: parent.height - 1
            width: parent.width
            padding: 0
            contentItem:
                ListView 
                {
                    id: popupList
                    clip: true
                    implicitHeight: contentHeight
                    model: dapComboBox.popup.visible ? dapComboBox.delegateModel : null
                    ScrollIndicator.vertical: ScrollIndicator {}
                }
    
            background: 
                Rectangle
                {
                    width: dapComboBox.background.width
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
            anchors.fill: topEffect ? parent : null
            source: topEffect ? background : null
            verticalOffset: topEffect ? 9 * pt : 0
            samples: topEffect ? 13 * pt : 0
            color: topEffect ? (dapComboBox.popup.visible ? colorDropShadow : colorTopNormalDropShadow) : "#000000"
            z: -1
        }
    }
