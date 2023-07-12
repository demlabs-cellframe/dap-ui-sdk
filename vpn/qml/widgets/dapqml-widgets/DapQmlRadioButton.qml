/* INCLUDES */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import Brand 1.0

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

Item {
    id: root
    property string text
    property bool checked: false

    signal clicked();

    DapQmlStyle { id: style; qss: root.qss; item: root }

    onTextChanged: label.text   = root.text

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

    property QtObject internal : QtObject {
        property real padding: 0//root.width / 20
        property real width: root.width - root.internal.padding * 2
    }

    /// @}

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: height * 0.125
        anchors.rightMargin: height * 0.125

        /****************************************//**
         * Icon image / Indicator / Checkbox
         ********************************************/

//        DapQmlLabel {
//            id: radioIndicator
//            Layout.preferredWidth: root.iconSize
//            Layout.preferredHeight: root.iconSize
//            qss: root.checked ? "radio-on" : "radio-off"
//        }

        Item {
            Layout.preferredWidth: root.iconSize
            Layout.preferredHeight: root.iconSize

            DapQmlImage {
                id: checkboxOn
                anchors.fill: parent
                visible: root.checked
                DapQmlStyle { item: checkboxOn; qss: "radio-on" }
            }

            DapQmlImage {
                id: checkboxOff
                anchors.fill: parent
                visible: !root.checked
                DapQmlStyle { item: checkboxOff; qss: "radio-off" }
            }
        }


        /****************************************//**
         * Text label
         ********************************************/

        Text {
            id: label
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: root.text
            font {
                family: Brand.fontName()
                pixelSize: fontSize
                weight: fontWeight
            }
            property int fontSize: 16
            property int fontWeight: Font.Normal
            DapQmlStyle { item: label; qss: root.textQss }
        }

//        DapQmlLabel {
//            id: label
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            horizontalAlign: Text.AlignLeft
//            verticalAlign: Text.AlignVCenter
//            text: root.text
//            qss: root.textQss
//        }
    }

    /****************************************//**
     * Separator
     ********************************************/

    DapQmlSeparator {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        width: root.width
        visible: root.separator
    }

    /****************************************//**
     * Clickable
     ********************************************/

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()//root.checked = !root.checked
    }
}

/*-----------------------------------------*/
