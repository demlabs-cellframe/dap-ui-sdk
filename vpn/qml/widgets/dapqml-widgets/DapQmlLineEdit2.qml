/* INCLUDES */

import QtQuick 2.10
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1
import DapQmlStyle 1.0
import DapQmlSerialKeyInput 1.0
import TextEditContextMenu 1.0
import Brand 1.0
import Scaling 1.0

/****************************************//**
 * @brief Dap QML Button Widget
 * @ingroup groupDapQmlWidgets
 *
 * @note Doesn't have public functions
 *
 * @date 08.22
 * @author Stanislav
 *******************************************/

Rectangle {
    id: root

    /* user no background */
    color: "transparent"

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string mainText: "Main text"
    property string placeHolderText: ""
    property string qss: ""
    property string mainQss: ""
    property string iconQss: ""
    property bool link: false

    property var labelMain
    property var labelIcon

    DapQmlStyle { id: style; qss: root.qss; item: root }

    MouseArea {
        anchors.fill: root;
        onClicked: root.clicked();
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief icon clicked
    signal iconClicked();

    /// @brief text field edited
    signal textEdited();

    /// @brief text field content changed
    signal textChanged();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setFocus() {
        lineEditField.forceActiveFocus();
        console.log("setFocus")
    }

    Component.onCompleted: {
        root.labelMain          = lineEditField;
    }

    /// @}
    /****************************************//**
     * @name CONTENT
     ********************************************/

    /* text edit field */
    TextField {
        id: lineEditField
        x: 10
        y: (root.height - height) / 2
        width: parent.width - x * 1.25
        height: parent.height * 0.75
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: root.mainText
        background: Rectangle { color: "transparent" }
        // android virtual keyboard
        inputMethodHints: Qt.ImhSensitiveData

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: {
                if (Scaling.isDesktop())
                    if (mouse.button === Qt.RightButton)
                        contextMenu.open()
            }
            onPressAndHold: {
                if (Scaling.isDesktop())
                    if (mouse.source === Qt.MouseEventNotSynthesized)
                        contextMenu.open()
            }
        }

        /* vars */
        property var fontFamiliy: Brand.fontName()
        property int fontSize: 14
        property int fontWeight: Font.Normal

        /* style */
        DapQmlStyle { qss: root.mainQss; item: lineEditField }

        /* font config */
        font {
            family: lineEditField.fontFamiliy
            pixelSize: lineEditField.fontSize
            weight: lineEditField.fontWeight
        }

        /* signals */
        onTextEdited: { root.mainText = text; root.textEdited(); }
        onTextChanged: { root.mainText = text; root.textChanged(); }
    }

}

/*-----------------------------------------*/
