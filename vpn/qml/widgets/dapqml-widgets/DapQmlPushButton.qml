/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import Qt5Compat.GraphicalEffects
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Push Button Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget with text label and background image
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

Button {
    id: root
    width: 374
    height: 90
    opacity: disabled.length === 0
             ? (enabled ? 1.0 : 0.25)
             : 1.0

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief background when button is active
    property string active: "qrc:/light/btn_bg_hover_active.png"

    /// @brief background when button is passive
    property string inactive: "qrc:/light/btn_bg.png"

    /// @brief background when button is disabled
    property string disabled

    /// @brief label font size
    property int fontSize: 14

    /// @brief qss style
    property string qss

    /// @brief button state, tells when button is hovered or checked
    property bool isActive: root.down ? true : (root.hovered ? true : (root.checked ? true : false))

    /// @}
    /****************************************//**
     * Text label
     ********************************************/

    contentItem: Item{}
    Text {
        anchors.fill: parent
        anchors.bottomMargin: root.fontSize * 0.25
        z: 10

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: "#FFFFFF"
        text: root.text
        font.family: "Roboto"
        font.pixelSize: root.fontSize
        font.weight: Font.Bold
    }

    /****************************************//**
     * Background image
     ********************************************/

    background: Item{}
    DapQmlImage {
        anchors.fill: parent
        z: 5
        visible: isActive && !disabledBgItem.visible
        scaledPixmap: root.active
    }

    DapQmlImage {
        anchors.fill: parent
        z: 5
        visible: !isActive && !disabledBgItem.visible
        scaledPixmap: root.inactive
    }

    DapQmlImage {
        id: disabledBgItem
        anchors.fill: parent
        z: 5
        visible: root.enabled === false && root.disabled.length !== 0
        scaledPixmap: root.disabled
    }
}

/*-----------------------------------------*/
