/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12
import DapQmlStyle 1.0
import Brand 1.0

/****************************************//**
 * @brief Dap QML Input Field Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget with text label, separator and
 * TextField
 *
 * @date 15.08.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string qss
    property string title: ""
    property var fontFamiliy: Brand.fontName()
    property int fontSize: 12
    property int fontWeight: Font.Normal

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /// @brief animation fixer
    Timer {
        id: titleLabelFixer
        interval: 100
        running: true
        repeat: false
        onTriggered: {
            titleLabel.duration = 250
        }
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief label clicked
    //signal clicked();

    /// @}
    /****************************************//**
     * Separator
     ********************************************/

    DapQmlSeparator {
        qss: "inputfield-separator"
    }

    /****************************************//**
     * Placeholder Title Label
     ********************************************/

    DapQmlLabel {
        id: titleLabel
        horizontalAlign: Text.AlignLeft
        qss: "inputfield-placeholder-in c-grey"
        text: root.title
        property int duration: 0
        Behavior on fontSize { PropertyAnimation { duration: titleLabel.duration } }
        Behavior on y { PropertyAnimation { duration: titleLabel.duration } }
    }

    /****************************************//**
     * Text Input Field
     ********************************************/

    TextField {
        id: inputField
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        echoMode: TextInput.Normal
        background: Rectangle { color: "transparent" }

        /* font config */
        font {
            family: root.fontFamiliy
            pixelSize: root.fontSize
            weight: root.fontWeight
        }

        DapQmlStyle { item: emailInput; qss: "inputfield-input" }

        onActiveFocusChanged: {
            titleLabel.qss = activeFocus
                    ? "inputfield-placeholder-out c-grey"
                    : "inputfield-placeholder-in c-grey";
        }
    }
}

/*-----------------------------------------*/
