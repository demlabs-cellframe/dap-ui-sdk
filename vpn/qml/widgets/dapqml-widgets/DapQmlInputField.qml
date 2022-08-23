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
    property string value: inputField.text
    property var fontFamiliy: Brand.fontName()
    property int fontSize: 12
    property int fontWeight: Font.Normal

    onValueChanged: {
        inputField.text = value;
        _updateTitlePos();
    }

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
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function clear() {
        value = '';
    }

    function _updateTitlePos() {
        titleLabel.qss = (inputField.activeFocus || inputField.text.length)
                ? "inputfield-placeholder-out c-grey"
                : "inputfield-placeholder-in c-grey";
    }

    /// @}
    /****************************************//**
     * Separator
     ********************************************/

    DapQmlSeparator {
        id: separator
        x: 0
        y: root.height - height
        width: root.width
        qss: "inputfield-separator"
    }

    /****************************************//**
     * Title Label
     ********************************************/

    DapQmlLabel {
        id: titleLabel
        width: root.width
        horizontalAlign: Text.AlignLeft
        qss: "inputfield-placeholder-in c-grey"
        text: root.title
        property int duration: 0
        Behavior on y { PropertyAnimation { duration: titleLabel.duration } }
        Behavior on fontSize { PropertyAnimation { duration: titleLabel.duration } }
    }

    /****************************************//**
     * Text Input Field
     ********************************************/

    TextField {
        id: inputField
        width: root.width
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

        DapQmlStyle { item: inputField; qss: "inputfield-input" }

        onActiveFocusChanged: root._updateTitlePos()
        onTextEdited: root.value = text
    }
}

/*-----------------------------------------*/
