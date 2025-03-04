import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TextField {
    id: root
    property bool bottomLineVisible: false
    property int bottomLineSpacing: 8
    property color bottomLineColor: borderColor
    property int bottomLineLeftRightMargins: 0
    property alias bottomLine: bottomLine

    property int borderWidth: 0
    property int borderRadius: 0

    property color selectColor: currTheme.inputActive
    property color selectTextColor: currTheme.mainBackground
    property color borderColor: currTheme.input
    property color backgroundColor: currTheme.secondaryBackground
    property color placeholderColor: currTheme.gray
    property color textColor: currTheme.white
    property string passwordChar: ""

    property int indicatorTopMargin: 0
    property bool indicatorVisible: false
    property url indicatorSourceEnabled: ""
    property url indicatorSourceDisabled: ""
    property url indicatorSourceEnabledHover: ""
    property url indicatorSourceDisabledHover: ""
    property alias indicator: indicator

    Keys.onReturnPressed: focus = false

    style: TextFieldStyle {
        textColor: root.textColor
        placeholderTextColor: root.placeholderColor
        selectionColor: root.selectColor
        selectedTextColor: root.selectTextColor
        padding.right: indicatorVisible ? indicator.width + 8 : 4

        background: Rectangle {
            radius: root.borderRadius
            border.width: root.borderWidth
            border.color: root.borderColor
            color: root.backgroundColor
        }
    }

    DapImageRender {
        id: indicator
        z: 10
        property bool isActive: false

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: parent.height / 2 - height / 2 + root.indicatorTopMargin

        visible: root.indicatorVisible
        source: root.indicatorSourceDisabled

        MouseArea {
            id: area
            anchors.fill: parent
            hoverEnabled: true

            onEntered: updateIndicator(true)
            onExited: updateIndicator(false)
            onClicked: {
                indicator.isActive = !indicator.isActive
                updateIndicator(area.containsMouse)
            }
        }
    }

    function updateIndicator(hovered) {
        if (indicator.isActive) {
            indicator.source = hovered ? root.indicatorSourceEnabledHover : root.indicatorSourceEnabled;
        } else {
            indicator.source = hovered ? root.indicatorSourceDisabledHover : root.indicatorSourceDisabled;
        }
    }

    Rectangle {
        id: bottomLine
        visible: root.bottomLineVisible

        anchors.top: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: root.bottomLineLeftRightMargins
        anchors.rightMargin: root.bottomLineLeftRightMargins
        anchors.topMargin: root.bottomLineSpacing

        height: 1
        color: root.bottomLineColor

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    }
}
