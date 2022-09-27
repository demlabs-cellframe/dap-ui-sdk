/* INCLUDES */

import QtQuick 2.0
import DapQmlStyle 1.0
import PageCtl 1.0

/****************************************//**
 * @brief Dap QML Dialog Title Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget with text label and close button
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief title text
    property string text: "Untitled"

    /// @brief qss style
    property string qss

    /// @brief hide close button
    property bool hideClose: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief close button clicked
    signal sigClose();

    /// @}
    /****************************************//**
     * Close button
     ********************************************/

    DapQmlPushButton {
        inactive: "qrc:/light/ic_close.png"
        active: "qrc:/light/ic_close_hover.png"
        qss: "form-title-close-btn"
        visible: hideClose === false
        onClicked: { root.sigClose(); PageCtl.slotBackwardAuto(); }
    }

    /****************************************//**
     * Title label
     ********************************************/

    DapQmlLabel {
        qss: "form-title-label"
        text: root.text
        clip: false
    }
}

/*-----------------------------------------*/
