/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import Qt5Compat.GraphicalEffects
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Checkbox Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget with text label and checkbox image
 *
 * @date 13.07.22
 * @author Mikhail Shilenko
 *******************************************/

Rectangle {
    id: root
    color: "transparent"

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property QtObject internal: QtObject {
        property bool hovered: false;
        property real calcIconSize: root.iconSize * (root.switchMode ? 1.735 : 1)
        property string checkOn:    root.switchMode ? "checkbox-switch-on"  : "checkbox-on"
        property string checkOff:   root.switchMode ? "checkbox-switch-off" : "checkbox-off"
    }

    /// @brief label text
    property string text: ""

    /// @brief check state
    property bool checked: false

    /// @brief font family
    property string fontName: "Roboto"

    /// @brief label font size
    property int fontSize: 14

    /// @brief label font size
    property int fontWeight: Font.Normal

    /// @brief icon size (checkbox)
    property int iconSize: 60

    /// @brief show checkbox as switch
    property bool switchMode: false

    /// @brief disable mouse area
    property bool disableClicking: false

    /// @brief qss style
    property string qss

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief label clicked
    signal clicked();

    /// @brief label clicked
    signal toggled(bool a_state);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief checkbox toggled
    function toggle() {
        checked = !checked;
        root.toggled (checked);
    }

    /// @}
    /****************************************//**
     * Icon
     ********************************************/

    DapQmlImage {
        id: icon
        y: (root.height - height) / 2
        z: 1
        width: root.internal.calcIconSize
        height: root.iconSize
        clip: false

        DapQmlStyle { item: icon; qss: root.checked ? root.internal.checkOn : root.internal.checkOff }
    }

    DapQmlImage {
        id: iconHover
        y: (root.height - height) / 2
        z: 1
        width: root.internal.calcIconSize
        height: root.iconSize
        visible: root.internal.hovered && !root.switchMode
        clip: false
        opacity: 0.25

        DapQmlStyle { item: iconHover; qss: (!root.checked) ? root.internal.checkOn : root.internal.checkOff }
    }

    /****************************************//**
     * Text label
     ********************************************/

    Text {
        x: icon.width
        z: 0
        width: root.width - x
        height: root.height

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        text: root.text

        font.family: root.fontName
        font.pixelSize: root.fontSize
        font.weight: root.fontWeight
    }

    /****************************************//**
     * Hover
     ********************************************/

    MouseArea {
        anchors.fill: root
        enabled: !root.disableClicking
        z: 2
        hoverEnabled: !root.disableClicking
        onEntered: root.internal.hovered = true
        onExited: root.internal.hovered = false
        onClicked: { root.toggle(); root.clicked(); }
    }
}

/*-----------------------------------------*/
