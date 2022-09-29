import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    id: root
    property bool bottomLineVisible: false
    property int bottomLineSpacing: 8
    property string bottomLineColor: borderColor
    property int bottomLineLeftRightMargins: 0

    property int borderWidth:0
    property int borderRadius:0

    property string selectColor: "#AABCDE"
    property string selectTextColor: "#2E3138"
    property string borderColor: currTheme.borderColor
    property string backgroundColor: currTheme.backgroundElements
    property string placeholderColor: currTheme.placeHolderTextColor
    property string textColor: currTheme.textColor

    style:
        TextFieldStyle
        {
            id: style
            textColor: root.textColor
            placeholderTextColor: placeholderColor
            selectionColor: selectColor
            selectedTextColor: selectTextColor

            background:
                Rectangle
                {
                    radius: borderRadius 
                    border.width: borderWidth
                    border.color: borderColor
                    color: backgroundColor
                }
        }

    //bottom line
    Rectangle {
        visible: bottomLineVisible

        anchors.top: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: bottomLineLeftRightMargins
        anchors.rightMargin: bottomLineLeftRightMargins
        anchors.topMargin: bottomLineSpacing

        height: 1

        color: bottomLineColor

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    }
}
