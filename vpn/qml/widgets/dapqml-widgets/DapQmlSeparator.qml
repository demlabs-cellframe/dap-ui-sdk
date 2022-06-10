/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import DapQmlImage 1.0

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
    height: 4
    color: "transparent"

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief widget qss style
    property string qss

    /// @}
    /****************************************//**
     * Left corner image
     ********************************************/

    DapQmlImage {
        id: sep1
        x: 0
        width: 8
        height: 4

        DapQmlStyle { item: sep1; qss: "sep-left"; }
    }

    /****************************************//**
     * Middle stretched image
     ********************************************/

    DapQmlImage {
        id: sep2
        x: 8
        width: root.width - 16
        height: 4

        DapQmlStyle { item: sep2; qss: "sep-mid"; }
    }

    /****************************************//**
     * Right corner image
     ********************************************/

    DapQmlImage {
        id: sep3
        x: root.width - width
        width: 8
        height: 4

        DapQmlStyle { item: sep3; qss: "sep-right"; }
    }
}

/*-----------------------------------------*/
