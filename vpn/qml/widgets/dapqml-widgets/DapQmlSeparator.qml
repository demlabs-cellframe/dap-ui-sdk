/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import DapQmlSeparatorItem 1.0
import Brand 1.0

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
    height: sizer.height > 2 ? sizer.height : 2
    color: "transparent"

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief widget qss style
    property string qss

    property DapQmlLabel sep1
    property DapQmlLabel sep2
    property DapQmlLabel sep3

    DapQmlDummy {
        id: sizer
        qss: "sep-rect"
    }

    /// @}
    /****************************************//**
     * @name COMPONENTS
     ********************************************/
    /// @{

    Component {
        id: compImage

        Item {
            anchors.fill: parent

            DapQmlLabel {
                id: sep1
                x: 0
                width: sizer.width
                height: sizer.height > 2 ? sizer.height : 2
                qss: "sep-left"

                Component.onCompleted: root.sep1    = sep1
            }

            DapQmlLabel {
                id: sep2
                x: sep1.width - 2
                width: root.width - sep1.width - sep3.width + 4
                height: sizer.height > 2 ? sizer.height : 2
                qss: "sep-mid"

                Component.onCompleted: root.sep2    = sep2
            }

            DapQmlLabel {
                id: sep3
                x: root.width - width
                width: sizer.width
                height: sizer.height > 2 ? sizer.height : 2
                qss: "sep-right"

                Component.onCompleted: root.sep3    = sep3
            }
        }
    }

    Component {
        id: compPainter

        DapQmlSeparatorItem {
            anchors.fill: parent
        }
    }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    Loader {
        width: root.width + 4
        height: sizer.height > 2 ? sizer.height : 2
        sourceComponent: //compImage
            Brand.legacyStyle()
            ? compImage
            : compPainter
    }
}

/*-----------------------------------------*/
