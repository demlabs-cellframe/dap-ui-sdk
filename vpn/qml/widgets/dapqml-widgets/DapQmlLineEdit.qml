/* INCLUDES */

import QtQuick 2.10
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1
import DapQmlStyle 1.0
import DapQmlSerialKeyInput 1.0
import TextEditContextMenu 1.0
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
    property string icon: ""
    property string iconLeft: ""
    property int iconLineEditSize: 20
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

    /// @brief right small button clicked
    signal rightClicked();

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


    /* Line edit */

    Component.onCompleted: {
        root.labelMain          = lineEditField;
        root.labelIcon          = lineEditIcon;
    }

    DapQmlLabel {
        id: lineEditIcon
        y: (root.height - height) / 2
        x: y
        width: root.iconLineEditSize
        height: root.iconLineEditSize

        qss: root.iconLeft
        scaledPixmap: "qrc:/light/ic_close_hover.png"
        onClicked: root.rightClicked();
    }

    /* main text */
    TextField {
        id: lineEditField
        y: (root.height - height) / 2
        x: lineEditIcon.width + lineEditIcon.x
        width: parent.width - lineEditIcon.width
        height: parent.height
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        text: root.mainText
        // android virtual keyboard
        inputMethodHints: Qt.ImhSensitiveData

        TextEditContextMenu {
            id: lineEditMenu
            Component.onCompleted: setTextEditWidget(lineEditField)
        }

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
            Menu {
                id: lineEditContextMenu
                MenuItem {
                    text: "Cut"
                    shortcut: "Ctrl+X"
                    onTriggered: lineEditMenu.execCut();
                }
                MenuItem {
                    text: "Copy"
                    shortcut: "Ctrl+C"
                    onTriggered: lineEditMenu.execCopy();
                }
                MenuItem {
                    text: "Paste"
                    shortcut: "Ctrl+V"
                    onTriggered: lineEditMenu.execPaste();
                }
                MenuItem {
                    text: "Delete"
                    //shortcut: "Delete"
                    onTriggered: lineEditMenu.execDelete();
                }
            }
        }

        /* vars */
        property string fontFamiliy: "Lato"
        property int fontSize: 14
        property int fontWeight: Font.Normal

        /* style */
        DapQmlStyle { qss: root.mainQss; item: lineEditField }

        /* background */
        background: DapQmlRectangle {
            qss: "ch-country-filter"
        }

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
