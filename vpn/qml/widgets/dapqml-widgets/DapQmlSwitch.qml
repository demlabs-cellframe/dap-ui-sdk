/* INCLUDES */

import QtQuick 2.0
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Switch Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Checkbox-like widget presented as a
 * turn on / turn off switch
 *
 * Built using two images:
 * - Background frame image
 * - Foreground toggle image
 *
 * @note changes style based on "checked" state
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    width: 270
    height: 174

    opacity: 0.5
    enabled: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief checkbox state
    /// @note changes hooked to _setStyle()
    property bool checked: false

    /// @brief widget qss style
    property string qss

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief switch is clicked (toggled)
    signal clicked();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change switch state
    function setState(value) {
        checked = value;
    }

    /// @brief toggle switch
    function toggle() {
        setState(!checked);
        root.clicked();
    }

    function setEnable(value) {
        root.enabled = value;
        root.opacity = (value)? 1.0: 0.5;
    }

    /// @brief change style based on checkbox state
    function _setStyle() {
        bg.qss  = (!checked) ? "switch-bg-off" : "switch-bg-on";
        tgl.qss = (!checked) ? "switch-toggle-off" : "switch-toggle-on";
    }

    /// @brief calc horizontal centering
    function _centerHor(item) {
        return root.width / 2 - item.width / 2;
    }

    /// @brief calc vertical centering
    function _centerVer(item) {
        return root.height / 2 - item.height / 2;
    }

    onCheckedChanged: _setStyle()

    /// @}
    /****************************************//**
     * Background frame
     ********************************************/

    DapQmlLabel {
        id: bg
        x: _centerHor(this)
        y: _centerVer(this)
        z: 0
        width: root.width - 12 * (root.width / 270)
        height: root.height - 36 * (root.height / 174)
        qss: "switch-bg-off"

        onClicked: toggle()
    }

    /****************************************//**
     * Toggle
     ********************************************/

    DapQmlLabel {
        id: tgl
        x: (checked === false) ? (-12 * (root.width / 270)) : (root.width - width + 12 * (root.width / 270))
        y: 0
        z: 1
        width: root.height
        height: root.height
        qss: "switch-toggle-off"

        onClicked: toggle()
        Behavior on x { PropertyAnimation { duration: 125; easing.type: Easing.InQuad } }
    }

    /****************************************//**
     * Mouce area
     ********************************************/

    MouseArea {
        anchors.fill: parent
        onClicked: toggle()
    }
}

/*-----------------------------------------*/
