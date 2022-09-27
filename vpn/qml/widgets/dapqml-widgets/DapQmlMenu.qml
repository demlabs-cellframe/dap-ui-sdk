/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12

/****************************************//**
 * @brief Dap QML Menu
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Stylized qml menu
 *
 * @date 18.08.22
 * @author Mikhail Shilenko
 *******************************************/

Menu {
    id: root
    padding: colorBackground.fontSize / 3

    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function _applyChanges() {
        /* change colors */
        background.color        = colorBackground.color;

        /* turn off border */
        background.border.color = colorLabel.color;

        /* change menu radius */
        background.radius       = colorBackground.fontSize / 5
    }

    /// @}
    /****************************************//**
     * Item display rules
     ********************************************/

    delegate: MenuItem {
        id: menuDelegate
        contentItem: Text {
            text: menuDelegate.text
            font: colorLabel.label.font
            color: colorLabel.color
        }
    }

    /****************************************//**
     * Style rules
     ********************************************/

    DapQmlLabel {
        id: colorLabel
        visible: false
        qss: "c-label"
        onColorChanged: root._applyChanges()
    }

    DapQmlLabel {
        id: colorBackground
        visible: false
        qss: "c-background menu-item-size"
        onColorChanged: root._applyChanges()
    }
}

/*-----------------------------------------*/
