/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Separator Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Horizontal stretching separator image
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

Rectangle {
    id: root
    width: 354
    height: sep1.height
    color: "transparent"

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief widget qss style
    property string qss

    DapQmlDummy {
        id: sizer
        qss: "sep-rect"
    }

    /// @}
    /****************************************//**
     * Left corner image
     ********************************************/

    DapQmlLabel {
        id: sep1
        x: 0
        width: sizer.width
        height: sizer.height > 1 ? sizer.height : 1
        qss: "sep-left"
    }

    /****************************************//**
     * Middle stretched image
     ********************************************/

    DapQmlLabel {
        id: sep2
        x: sep1.width - 2
        width: root.width - sep1.width - sep3.width + 4
        height: sizer.height > 1 ? sizer.height : 1
        qss: "sep-mid"
    }

    /****************************************//**
     * Right corner image
     ********************************************/

    DapQmlLabel {
        id: sep3
        x: root.width - width
        width: sizer.width
        height: sizer.height > 1 ? sizer.height : 1
        qss: "sep-right"
    }
}

/*-----------------------------------------*/
