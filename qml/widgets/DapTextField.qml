import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    id: root
    property bool bottomLineVisible: false
    property int bottomLineSpacing: 8
    property string bottomLineColor: borderColor
    property int bottomLineLeftRightMargins: 0
    property alias bottomLine: bottomLine

    property int borderWidth:0
    property int borderRadius:0

    property string selectColor: "#AABCDE"
    property string selectTextColor: "#2E3138"
    property string borderColor: currTheme.borderColor
    property string backgroundColor: currTheme.backgroundElements
    property string placeholderColor: currTheme.placeHolderTextColor
    property string textColor: currTheme.textColor

    property bool indicatorVisible: false
    property string indicatorSourceEnabled: ""
    property string indicatorSourceDisabled: ""
    property string indicatorSourceEnabledHover: ""
    property string indicatorSourceDisabledHover: ""
    property alias indicator: indicator

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


    DapImageRender{
        id: indicator

        property bool isActive: false

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        visible: indicatorVisible
        source: indicatorSourceDisabled

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                if(indicator.isActive)
                    indicator.source = indicatorSourceEnabledHover
                else
                    indicator.source = indicatorSourceDisabledHover
            }

            onExited: {
                if(indicator.isActive)
                    indicator.source = indicatorSourceEnabled
                else
                    indicator.source = indicatorSourceDisabled
            }
            onClicked: {
                indicator.isActive = !indicator.isActive

                if(containsMouse){
                    if(indicator.isActive)
                        indicator.source = indicatorSourceEnabledHover
                    else
                        indicator.source = indicatorSourceDisabledHover
                }
                else
                {
                    if(indicator.isActive)
                        indicator.source = indicatorSourceEnabled
                    else
                        indicator.source = indicatorSourceDisabled
                }
            }
        }
    }

    //bottom line
    Rectangle {
        id: bottomLine
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
