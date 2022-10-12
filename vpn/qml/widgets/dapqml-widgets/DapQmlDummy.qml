/* INCLUDES */

import QtQuick 2.0
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Dummy Widget
 * @ingroup groupDapQmlWidgets
 *
 * Used to apply style rules, but show nothing
 *
 * ### Structure
 *
 * Simple item widget with style support
 *
 * @date 25.09.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief widget qss style
    property string qss

    /// @}
}

/*-----------------------------------------*/
