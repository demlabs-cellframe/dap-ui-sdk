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
///@sidePaddingActive and @sidePaddingNormal - padding width of the combo box in the active state
///                                                                     and in the normal state
///@sidePaddingActive and @sidePaddingNormal - sets the indent from the edge of the right and left
///                                           edges of the parent in the active and normal state
///@topIndentActive and @topIndentNormal
///@bottomIndentActive and @bottomIndentNormal
/// - sets the indent from the edge of the upper and lower edges of the parent in the active and normal state
///@indicatorImageNormal and @indicatorImageActive - indicator picture address for active and normal state
///@indicatorWidth and @indicatorHeight - indicator width and height

ComboBox {

    property string normalColorText: "#070023"
    property string hilightColorText: "#FFFFFF"
    property string normalColorTopText:normalColorText
    property string hilightColorTopText:normalColorText
    property string normalColor: "#FFFFFF"
    property string hilightColor: "#330F54"
    property int fontSizeComboBox: 16*pt

    property int widthPopupComboBoxNormal: parent.width
    property int widthPopupComboBoxActive: widthPopupComboBoxNormal

    property int sidePaddingNormal:16 * pt
    property int sidePaddingActive:sidePaddingNormal

    property int topIndentNormal:12 * pt
    property int topIndentActive:topIndentNormal

    property int bottomIndentNormal:14 * pt
    property int bottomIndentActive:bottomIndentNormal

    property string indicatorImageNormal: "qrc:/res/icons/ic_arrow_drop_down_dark_blue.png"
    property string indicatorImageActive: "qrc:/res/icons/ic_arrow_drop_up_dark_blue.png"
    property int indicatorWidth: 24*pt
    property int indicatorHeight: indicatorWidth


    id: customComboBox
    width: popup.visible ? widthPopupComboBoxActive : widthPopupComboBoxNormal
    height: parent.height

    delegate:ItemDelegate {
        width: parent.width
        height:{
            if(index == currentIndex) return 0
            else return 42*pt
        }
        contentItem: Text {
            id:textDelegateComboBox
            anchors.fill: parent
            anchors.topMargin: 8 * pt
            anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
            verticalAlignment: Qt.AlignTop
            font.family: fontRobotoRegular.name
                font.pixelSize: fontSizeComboBox
                text: {if(index != currentIndex) return modelData;}
                color: hovered ? hilightColorText : normalColorText
        }

        background: Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: 10 * pt
            color: hovered ? hilightColor : normalColor
        }

        highlighted: parent.highlightedIndex === index

    }


    indicator: Image {    
        source: parent.popup.visible ? indicatorImageActive : indicatorImageNormal
        width: indicatorWidth
        height: indicatorHeight
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
    }

    background: Rectangle {
        anchors.fill: parent
        color: parent.popup.visible ? normalColor : "transparent"
        radius: 2 * pt
        height: parent.height

    }

    contentItem: Text {
        anchors.fill: parent
        anchors.leftMargin: popup.visible ? sidePaddingActive : sidePaddingNormal
        anchors.topMargin: popup.visible ? topIndentActive : topIndentNormal
        text: parent.displayText
        font.family: fontRobotoRegular.name
        font.pixelSize: fontSizeComboBox
        color: popup.visible ? hilightColorTopText : normalColorTopText
        verticalAlignment: Text.AlignTop
    }

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
            Rectangle {
                id: contentCorner
                anchors.fill: parent
            }

            DropShadow {
                anchors.fill: parent
                source: contentCorner
                verticalOffset: 9 * pt
                samples: 13 * pt
                color: "#40000000"
            }
        }
    }

    DropShadow {
        anchors.fill: parent
        source: background
        verticalOffset: 9 * pt
        samples: 13 * pt
        color: "#40000000"
    }
}
