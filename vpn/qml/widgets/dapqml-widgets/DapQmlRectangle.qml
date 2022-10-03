/* INCLUDES */

import QtQuick 2.0
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Rectangle Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Simple rectangle widget with style support
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

Rectangle {
    id: root
    color: "transparent"
    border.color: borderColor
    border.width: borderWidth

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief widget qss style
    property string qss

    /// @brief rectangle border color
    property string borderColor

    /// @brief rectangle border width
    property int borderWidth

    /// @}
}

/*-----------------------------------------*/
