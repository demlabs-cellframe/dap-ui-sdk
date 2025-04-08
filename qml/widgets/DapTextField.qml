import QtQuick
import QtQuick.Controls

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
    property string defaultPlaceholderText: ""


    property int indicatorTopMargin: 0
    property bool indicatorVisible: false
    property url indicatorSourceEnabled: ""
    property url indicatorSourceDisabled: ""
    property url indicatorSourceEnabledHover: ""
    property url indicatorSourceDisabledHover: ""
    property alias indicator: indicator
    property bool isActiveCopy: true

    property var regExpValidator: /.*/

    signal updateFeild()

    validator: RegularExpressionValidator { regularExpression: regExpValidator}

    Keys.onReturnPressed: focus = false

    placeholderText: root.defaultPlaceholderText
    placeholderTextColor: root.placeholderColor
    color: root.textColor
    selectionColor: root.selectColor
    selectedTextColor: root.selectTextColor
    rightPadding: indicatorVisible ? indicator.width + 8 : 4

    background: Rectangle {
        radius: root.borderRadius
        border.width: root.borderWidth
        border.color: root.borderColor
        color: root.backgroundColor
    }

    onActiveFocusChanged: {
        // If the TextField is focused, hide the placeholder text
        if (root.activeFocus) {
            root.placeholderText = "";
        } else {
            root.placeholderText = root.defaultPlaceholderText;  // Or whatever default text you want
        }

        updateFeild()
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

        sourceSize: Qt.size(24 * guiApp.scaleFactor,24 * guiApp.scaleFactor)

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

        height: 1  * guiApp.scaleFactor
        color: root.bottomLineColor

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    }

    Connections{
        target: Qt.inputMethod
        function onVisibleChanged(){
            if(!Qt.inputMethod.visible)
            {
                // keyboard.visible = false
                root.focus = false
            }
        }
    }
}
