/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.5
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
    opacity: enabled ? 1.0 : 0.25

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief background when button is active
    property string active: "qrc:/light/btn_bg_hover_active.png"

    /// @brief background when button is passive
    property string inactive: "qrc:/light/btn_bg.png"

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

    contentItem: Text {
        anchors.centerIn: root

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

    background: DapQmlImage {
        anchors.fill: parent
        scaledPixmap: isActive ? root.active : root.inactive
    }
}

/*-----------------------------------------*/
