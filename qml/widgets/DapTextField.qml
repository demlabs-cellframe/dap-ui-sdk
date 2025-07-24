import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

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
    property bool contextMenuEnabled: false
    property color contextMenuTextColorNormal: currTheme.white
    property color contextMenuTextColorHover: currTheme.mainBackground
    property color contextMenuBackgroundColorNormal: currTheme.secondaryBackground
    property color contextMenuBackgroundColorHover: currTheme.lime

    property var regExpValidator: /.*/

    signal updateFeild

    validator: RegularExpressionValidator {
        regularExpression: regExpValidator
    }

    Keys.onReturnPressed: focus = false
    Keys.onEnterPressed: focus = false

    onPressAndHold: {
        if (contextMenuEnabled) {
            var mousePos = mapFromItem(null, 0, 0); // Get current mouse position
            contextMenu.x = mousePos.x;
            contextMenu.y = mousePos.y;
            contextMenu.open();
        }
    }

    onReleased: function (event) {
        if (event.button === Qt.RightButton && contextMenuEnabled) {
            contextMenu.x = event.x;
            contextMenu.y = event.y;
            contextMenu.open();
        }
    }

    // onEditingFinished: focus = false

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
            if (root.text === "")
                root.placeholderText = root.defaultPlaceholderText;  // Or whatever default text you want
        }

        updateFeild();
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

        sourceSize: Qt.size(24 * guiApp.scaleFactor, 24 * guiApp.scaleFactor)

        MouseArea {
            id: area
            anchors.fill: parent
            hoverEnabled: true

            onEntered: updateIndicator(true)
            onExited: updateIndicator(false)
            onClicked: {
                indicator.isActive = !indicator.isActive;
                updateIndicator(area.containsMouse);
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

        height: 1 * guiApp.scaleFactor
        color: root.bottomLineColor

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    }

    Connections {
        target: Qt.inputMethod
        function onVisibleChanged() {
            if (!Qt.inputMethod.visible) {
                // keyboard.visible = false
                root.focus = false;
            }
        }
    }

    Menu {
        id: contextMenu
        enabled: contextMenuEnabled
        topPadding: 0
        bottomPadding: 0

        font: mainFont.dapFont.medium12

        MenuItem {
            id: cut
            enabled: isActiveCopy
            text: qsTr("Cut")
            onTriggered: {
                root.cut();
            }
            topPadding: 8 * guiApp.scaleFactor
            bottomPadding: 8 * guiApp.scaleFactor
            leftPadding: 12 * guiApp.scaleFactor
            rightPadding: 12 * guiApp.scaleFactor

            contentItem: textComponent.createObject(cut)
            background: backgroundComponent.createObject(cut)
        }
        MenuItem {
            id: copy
            text: qsTr("Copy")
            enabled: isActiveCopy
            onTriggered: {
                root.copy();
            }
            topPadding: 8 * guiApp.scaleFactor
            bottomPadding: 8 * guiApp.scaleFactor
            leftPadding: 12 * guiApp.scaleFactor
            rightPadding: 12 * guiApp.scaleFactor

            contentItem: textComponent.createObject(copy)
            background: backgroundComponent.createObject(copy)
        }
        MenuItem {
            id: paste
            text: qsTr("Paste")
            onTriggered: {
                root.paste();
            }
            topPadding: 8 * guiApp.scaleFactor
            bottomPadding: 8 * guiApp.scaleFactor
            leftPadding: 12 * guiApp.scaleFactor
            rightPadding: 12 * guiApp.scaleFactor

            contentItem: textComponent.createObject(paste)
            background: backgroundComponent.createObject(paste)
        }

        background: Item {
            implicitWidth: 90 * guiApp.scaleFactor

            Rectangle {
                id: background
                anchors.fill: parent
                radius: 4
                color: backgroundColor
            }
            DropShadow {
                id: shadow
                anchors.fill: background
                horizontalOffset: 2
                verticalOffset: 2
                radius: 4
                samples: 10
                cached: true
                color: "#000000"
                opacity: 0.56
                source: background
            }
        }

        Component {
            id: textComponent
            Text {
                text: parent.text
                font: contextMenu.font
                opacity: enabled ? 1.0 : 0.3
                color: parent.hovered ? contextMenuTextColorHover : contextMenuTextColorNormal/*
                           menuItem.down ? contentTextColorDown : */
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }

        Component {
            id: backgroundComponent
            Rectangle {
                anchors.fill: parent
                anchors.margins: 4 * guiApp.scaleFactor
                radius: 4
                opacity: enabled ? 1 : 0.3
                color: parent.hovered ? contextMenuBackgroundColorHover : contextMenuBackgroundColorNormal
            }
        }
    }


}
