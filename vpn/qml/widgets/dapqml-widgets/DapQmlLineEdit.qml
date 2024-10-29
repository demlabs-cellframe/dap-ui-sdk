/* INCLUDES */

import QtQuick 2.10
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
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
    property string placeholderQss: ""
    property bool link: false

    property var labelMain
    property var labelIcon

    DapQmlStyle { id: style; qss: root.qss; item: root }

//    MouseArea {
//        anchors.fill: root;
//        onClicked: root.clicked();
//    }

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
        root.labelIcon          = lineEditIcon;
    }

    /// @}
    /****************************************//**
     * @name CONTENT
     ********************************************/

    /* icon */
    DapQmlLabel {
        id: lineEditIcon
        x: y
        y: (root.height - height) / 2 + height * 0.1
        qss: "line-edit-icon " + root.iconQss

        onClicked: root.iconClicked();
    }

    /* text edit field */
    TextField {
        id: lineEditField
        x: lineEditIcon.width + lineEditIcon.x * 1.5
        y: (root.height - height) / 2
        width: parent.width - x * 1.25
        height: parent.height * 0.75
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        text: root.mainText
        background: Rectangle { color: "transparent" }
        // android virtual keyboard
        inputMethodHints: Qt.ImhSensitiveData
        selectByMouse: true

        /* placeholder */
        DapQmlLabel {
            id: placeholderLabel
            x: height * 0.25
            width: parent.width
            height: parent.height * 0.85
            horizontalAlign: Text.AlignLeft
            verticalAlign: Text.AlignBottom
            text: root.placeHolderText
            qss: "ch-country-filter-text c-grey " + root.placeholderQss
            enabled: false
            visible: lineEditField.text.length === 0
        }

        TextEditContextMenu {
            id: lineEditMenu
            Component.onCompleted: {
                setSerialInpoutMode(false);
                setTextEditWidget(lineEditField);
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            cursorShape: Qt.IBeamCursor
            onClicked: {
                if (Scaling.isDesktop())
                    if (mouse.button === Qt.RightButton)
                        lineEditContextMenu.open()//contextMenu.open()
            }
            onPressAndHold: {
                if (Scaling.isDesktop())
                    if (mouse.source === Qt.MouseEventNotSynthesized)
                        lineEditContextMenu.open()//contextMenu.open()
            }

//            Menu {
//                id: lineEditContextMenu
//                MenuItem {
//                    text: "Cut"
//                    shortcut: "Ctrl+X"
//                    onTriggered: lineEditMenu.execCut();
//                }
//                MenuItem {
//                    text: "Copy"
//                    shortcut: "Ctrl+C"
//                    onTriggered: lineEditMenu.execCopy();
//                }
//                MenuItem {
//                    text: "Paste"
//                    shortcut: "Ctrl+V"
//                    onTriggered: lineEditMenu.execPaste();
//                }
//                MenuItem {
//                    text: "Delete"
//                    //shortcut: "Delete"
//                    onTriggered: lineEditMenu.execDelete();
//                }
//            }

            DapQmlMenu {
                id: lineEditContextMenu
                shortcuts: [
                    "Ctrl+X",
                    "Ctrl+C",
                    "Ctrl+V",
                    ""
                ]
                Action {
                    text: qsTr("Cut") + lang.notifier
                    shortcut: "Ctrl+X"
                    onTriggered: lineEditMenu.execCut();
                    Component.onCompleted: console.log(shortcut)
                }
                Action {
                    text: qsTr("Copy") + lang.notifier
                    shortcut: "Ctrl+C"
                    onTriggered: lineEditMenu.execCopy();
                }
                Action {
                    text: qsTr("Paste") + lang.notifier
                    shortcut: "Ctrl+V"
                    onTriggered: lineEditMenu.execPaste();
                }
                Action {
                    text: qsTr("Delete") + lang.notifier
                    //shortcut: "Delete"
                    onTriggered: lineEditMenu.execDelete();
                }
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
