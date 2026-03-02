/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import Qt5Compat.GraphicalEffects
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Tab Button Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget used as tab at the bottom menu
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

TabButton {
    id: root
    width: 130
    height: 130
    background: Rectangle { color: "transparent" }

    DapQmlStyle { item: root; qss: root.qss; }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief active image filename
    property string active

    /// @brief inactive image filename
    property string inactive

    /// @brief widget qss style
    property string qss

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function _isStill()
    {
        return !root.checked;//!root.hovered && !root.down;
    }

    /// @}
    /****************************************//**
     * @name Content
     ********************************************/

    DapQmlImage {
        anchors.fill: parent
        visible: _isStill()
        scaledPixmap: root.inactive
    }

    DapQmlImage {
        anchors.fill: parent
        visible: !_isStill()
        scaledPixmap: root.active
    }
}

/*-----------------------------------------*/
