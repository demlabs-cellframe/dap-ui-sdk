/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

/****************************************//**
 * @brief Dap QML Radio Button Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget with text label, background image
 * and separator at the bottom
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

RadioButton {
    id: root
    text: ""
    checked: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief text label qss style
    property string textQss: "radio-label"

    /// @brief widget qss style
    property string qss: ""

    /// @brief icon size (checkbox)
    property int iconSize: 60

    /// @brief enable\\disable separator
    property bool separator: false

    /// @brief padding between icon and text label
    //property real textPadding: radioIndicator.width + root.spacing

    indicator: Item {}
    contentItem: Item {}

    property QtObject internal : QtObject {
        property real padding: 0//root.width / 20
        property real width: root.width - root.internal.padding * 2
    }

    /// @}
    /****************************************//**
     * Icon image / Indicator / Checkbox
     ********************************************/

    //indicator: DapQmlLabel {
    DapQmlLabel {
        id: radioIndicator
        x: internal.padding
        qss: root.checked ? "radio-on" : "radio-off"
        width: root.iconSize
        height: root.iconSize
        onClicked: { root.toggle(); root.clicked(); }
    }

    /****************************************//**
     * Text label
     ********************************************/

    //contentItem: DapQmlLabel {
    DapQmlLabel {
        x: internal.padding + radioIndicator.width + root.spacing
        width: internal.width - radioIndicator.width - root.spacing
        height: radioIndicator.height
        //leftPadding: textPadding // root.indicator.width + root.spacing
        horizontalAlign: Text.AlignLeft
        verticalAlign: Text.AlignVCenter
        text: root.text
        qss: root.textQss
        onClicked: { root.toggle(); root.clicked(); }
    }

    /****************************************//**
     * Separator
     ********************************************/

    DapQmlSeparator {
        x: (root.width - width) / 2
        y: root.height// - height
        width: root.width
        visible: root.separator
    }
}

/*-----------------------------------------*/
