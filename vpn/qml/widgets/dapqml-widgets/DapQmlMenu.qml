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
    property var shortcuts: new Array()
    property var __menuItems: new Array()

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

    Component.onCompleted: {
        for (let i = 0; i < root.shortcuts.length; i++)
        {
            let shortcut        = root.shortcuts[i];
            __menuItems[i].text = shortcut;
        }
    }

    /// @}
    /****************************************//**
     * Item display rules
     ********************************************/

    delegate: MenuItem {
        id: menuDelegate
        contentItem: Rectangle {
            color: "transparent"
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                text: menuDelegate.text
                font: colorLabel.label.font
                color: colorLabel.color
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.Text.AlignRight
                //text: menuDelegate.sequence // menuDelegate.shortcut
                font: colorLabel.label.font
                color: colorGray.color
                Component.onCompleted: root.__menuItems.push(this)
            }
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
        id: colorGray
        visible: false
        qss: "c-grey"
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
