/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.5
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

    DapQmlLabel {
        id: icon
        y: (root.height - height) / 2
        z: 1
        qss: root.checked ? "checkbox-on" : "checkbox-off"
        width: root.iconSize
        height: root.iconSize
    }

    DapQmlLabel {
        id: iconHover
        y: (root.height - height) / 2
        z: 1
        qss: (!root.checked) ? "checkbox-on" : "checkbox-off"
        width: root.iconSize
        height: root.iconSize
        visible: root.internal.hovered
        opacity: 0.25
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
        z: 2
        hoverEnabled: true
        onEntered: root.internal.hovered = true
        onExited: root.internal.hovered = false
        onClicked: { root.toggle(); root.clicked(); }
    }
}

/*-----------------------------------------*/
