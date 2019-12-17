import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ComboBox {
    ///@detalis normalColorText Text color in normal state.
    property string normalColorText: "#070023"
    ///@detalis hilightColorText Text color in selected state.
    property string hilightColorText: "#FFFFFF"
    ///@detalis normalColorTopText Text color in the main line in normal state.
    property string normalColorTopText: normalColorText
    ///@detalis hilightColorTopText Text color in the main line in active state.
    property string hilightColorTopText: normalColorText
    ///@detalis normalColor Item color in normal state.
    property string normalColor: "#FFFFFF"
    ///@detalis hilightColor Item color in selected state.
    property string hilightColor: "#330F54"
    ///@detalis normalTopColor Top string color in normal state.
    property string normalTopColor: normalColor
    ///@detalis hilightTopColor Top string color in selected state.
    property string hilightTopColor: normalColor
    ///@detalis fontSizeComboBox Font size for the entire widget.
    property int fontSizeComboBox: 16 * pt
    ///@detalis widthPopupComboBoxNormal Width of the combo box in the normal state.
    property int widthPopupComboBoxNormal: parent.width
    ///@detalis widthPopupComboBoxActive Width of the ComboBox in the active state.
    property int widthPopupComboBoxActive: widthPopupComboBoxNormal
    ///@detalis heightComboBoxNormal Height of the ComboBox in the normal state.
    property int heightComboBoxNormal: parent.height
    ///@detalis heightComboBoxActive Height of the ComboBox in the active state.
    property int heightComboBoxActive: heightComboBoxNormal
    ///@detalis sidePaddingNormal: Sets the indent from the edge of the right and left edges of the parent in the normal state.
    property int sidePaddingNormal: 16 * pt
    ///@detalis sidePaddingActive Sets the indent from the edge of the right and left edges of the parent in the active state.
    property int sidePaddingActive: sidePaddingNormal
    ///@detalis topIndentNormal Sets the indent from the edge of the upper of the parent in the normal state.
    property int topIndentNormal: 12 * pt
    ///@detalis topIndentActive Sets the indent from the edge of the upper of the parent in the active state.
    property int topIndentActive: topIndentNormal
    ///@detalis bottomIndentNormal Sets the indent from the edge of the lower of the parent in the normal state.
    property int bottomIndentNormal: 14 * pt
    ///@detalis bottomIndentActive Sets the indent from the edge of the lower of the parent in the active state.
    property int bottomIndentActive: bottomIndentNormal
    ///@detalis paddingTopItemDelegate Indent above from item delegate.
    property int paddingTopItemDelegate: 8 * pt
    ///@detalis paddingBottomItemDelegate Indent below from item delegate.
    property int paddingBottomItemDelegate: paddingTopItemDelegate
    ///@detalis heightListElement List item height.
    property int heightListElement: 32 * pt
    ///@detalis intervalListElement Spacing between items in a list.
    property int intervalListElement: 10 * pt
    ///@detalis bottomIntervalListElement Spacing from bottom to bottom.
    property int bottomIntervalListElement: intervalListElement
    ///@detalis topEffect Using an effect for the top element.
    property bool topEffect: true
    ///@detalis indicatorImageNormal Indicator picture address for normal state.
    property string indicatorImageNormal: "qrc:/res/icons/ic_arrow_drop_down_dark_blue.png"
    ///@detalis indicatorImageActive Indicator picture address for active state.
    property string indicatorImageActive: "qrc:/res/icons/ic_arrow_drop_up_dark_blue.png"
    ///@detalis indicatorWidth Indicator width.
    property int indicatorWidth: 24 * pt
    ///@detalis indicatorHeight Indicator height.
    property int indicatorHeight: indicatorWidth
    /// colorTopNormalDropShadow Color of the shadow effect of the combo box when minimized.
    property string colorTopNormalDropShadow: "#00000000"
    ///@detalis colorDropShadow Unboxed shadow color in expanded state.
    property string colorDropShadow: "#40ABABAB"
    ///@detalis countComboBox this variable indicates the number of lines in the list for
    ///the delegate in the file DapComboBox.qml.
    property int countComboBox: customComboBox.count

    id: customComboBox
    width: popup.visible ? widthPopupComboBoxActive : widthPopupComboBoxNormal
    height: popup.visible ? heightComboBoxActive : heightComboBoxNormal
    anchors.verticalCenter: parent.verticalCenter

    //Icon icon near the text (arrow)
    indicator: Image {
        source: parent.popup.visible ? indicatorImageActive : indicatorImageNormal
        width: indicatorWidth
        height: indicatorHeight
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
    }

    //Defining the background for the main line
    background: Rectangle {
        anchors.fill: parent
        color: parent.popup.visible ? hilightTopColor : normalTopColor
        radius: 2 * pt
        height: parent.height
    }
    //Main line text settings
    contentItem: Text {
        anchors.fill: parent
        anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
        text: parent.displayText
        font.family: fontRobotoRegular.name
        font.pixelSize: fontSizeComboBox
        color: popup.visible ? hilightColorTopText : normalColorTopText
        verticalAlignment: Text.AlignVCenter
    }

    //Customize drop-down list with shadow effect
    popup: Popup {
        y: parent.height - 1
        width: parent.width + 1
        padding: 1
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: customComboBox.popup.visible ? customComboBox.delegateModel : null
            ScrollIndicator.vertical: ScrollIndicator {}
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

    //Shadow effect for the top element.
    DropShadow {
        anchors.fill: if (topEffect)
                          parent
        source: if (topEffect)
                    background
        verticalOffset: if (topEffect)
                            9 * pt
        samples: if (topEffect)
                     13 * pt
        color: if (topEffect)
                   customComboBox.popup.visible ? colorDropShadow : colorTopNormalDropShadow
    }
}
