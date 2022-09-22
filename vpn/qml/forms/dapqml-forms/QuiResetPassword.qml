/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0
import PageCtl 1.0

/****************************************//**
 * @brief reset password Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * TODO
 *
 * @date 27.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum Mode
    {
        M_INPUT,
        M_FINISHED
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ResetPassword"

    /// @brief form's current mode
    property int mode: QuiResetPassword.Mode.M_INPUT

    /// @brief email animation fixer
    Timer {
        id: placeholderFixer
        interval: 100
        running: true
        repeat: false
        onTriggered: {
            placeholder.duration = 250
        }
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief
    function show() {
    }

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: "Restore password"
        qss: "dialog-title"
    }

    /****************************************//**
     * Description
     ********************************************/

    DapQmlLabel {
        id: description
        horizontalAlign: Text.AlignLeft
        lineHeight: (fontSize / 14) * 1.7
        visible: root.mode === QuiResetPassword.Mode.M_INPUT
        qss: "passreset-description c-label"
        text: "Enter the email address you used when you joined and\n"
              + "we’ll send you instructions to reset your password.\n"
              + "\n"
              + "For security reasons, we do NOT store your password. So\n"
              + "rest assured that we will never send your password via\n"
              + "email.\n"
    }

    DapQmlLabel {
        id: resultDescription
        lineHeight: (fontSize / 14) * 1.7
        visible: root.mode === QuiResetPassword.Mode.M_FINISHED
        qss: "passreset-result-description c-label"
        text: "Password reset instructions sent\n"
              + "successfully! Check your email."
    }

    /****************************************//**
     * Email input
     ********************************************/

    DapQmlSeparator {
        visible: root.mode === QuiResetPassword.Mode.M_INPUT
        enabled: root.mode === QuiResetPassword.Mode.M_INPUT
        qss: "passreset-separator"
    }

    DapQmlLabel {
        id: placeholder
        horizontalAlign: Text.AlignLeft
        visible: root.mode === QuiResetPassword.Mode.M_INPUT
        enabled: root.mode === QuiResetPassword.Mode.M_INPUT
        qss: "passreset-placeholder-in c-grey"
        text: "Email"
        property int duration: 0
        Behavior on fontSize { PropertyAnimation { duration: placeholder.duration } }
        Behavior on y { PropertyAnimation { duration: placeholder.duration } }
    }

    TextField {
        id: emailInput
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom
        echoMode: TextInput.Normal
        visible: root.mode === QuiResetPassword.Mode.M_INPUT
        enabled: root.mode === QuiResetPassword.Mode.M_INPUT
        background: Rectangle { color: "transparent" }

        /* vars */
        property string fontFamiliy: "Exo2"
        property int fontSize: 12
        property int fontWeight: Font.Normal

        /* font config */
        font {
            family: emailInput.fontFamiliy
            pixelSize: emailInput.fontSize
            weight: emailInput.fontWeight
        }

        DapQmlStyle { item: emailInput; qss: "passreset-email-input c-label" }

        onActiveFocusChanged: {
            placeholder.qss = activeFocus
                    ? "passreset-placeholder-out c-grey"
                    : "passreset-placeholder-in c-grey";
        }
    }

    /****************************************//**
     * Send button
     ********************************************/

    DapQmlPushButton {
        text: (root.mode === QuiResetPassword.Mode.M_INPUT)
            ? "SEND RESET INSTRUCTIONS"
            : "BACK"
        qss: "passreset-send-btn push-button"
        enabled: emailInput.length >= 3
        opacity: 0.4 + 0.6 * enabled

        onClicked: {
            if (root.mode === QuiResetPassword.Mode.M_INPUT)
                root.mode = QuiResetPassword.Mode.M_FINISHED;
            else
                PageCtl.slotBackwardAuto();
        }
    }
}

/*-----------------------------------------*/
