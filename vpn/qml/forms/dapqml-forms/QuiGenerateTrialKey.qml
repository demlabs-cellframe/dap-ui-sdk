/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Generate Trial Key Form
 * @ingroup groupDapQmlForms
 *
 * @date 13.07.23
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "GenerateTrialKey"

    property QtObject internal: QtObject {
        property bool generated: false
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigClose();
    signal sigGenerate();
    signal sigActivate();

    //Component.onCompleted: reset()
    Component.onCompleted: setKey("TEST-TEST-TEST-TEST")

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function reset() {
        root.internal.generated = false;
        actionButton.text       = qsTr("GENERATE KEY");
    }

    function setKey(a_key) {
        root.internal.generated = true;
        actionButton.text       = qsTr("ACTIVATE");
    }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    DapQmlRectangle {
        anchors.bottom: parent.bottom
        qss: !root.internal.generated
            ? "gentri-request-rect c-background"
            : "gentri-serialkey-rect c-background"

        Behavior on height { PropertyAnimation { duration: 250 } }

        /****************************************//**
         * Title
         ********************************************/

        DapQmlLabel {
            z: 10
            qss: "gentri-title c-label"
            text: qsTr("Generate trial key")
        }

        /****************************************//**
         * Close Button
         ********************************************/

        DapQmlPushButton {
            x: parent.width - width - (y * 3)
            qss: "gentri-close-btn"
            onClicked: root.sigClose();
        }

        /****************************************//**
         * Action button
         ********************************************/

        DapQmlRectangle {
            anchors.bottom: parent.bottom
            qss: "gentri-action-rect"

            DapQmlSeparator {
                qss: "gentri-action-separator"
            }

            DapQmlPushButton {
                id: actionButton
                qss: "gentri-action-btn push-button"
                onClicked:
                    root.internal.generated
                    ? root.sigActivate()
                    : root.sigGenerate();
            }
        }

        /****************************************//**
         * Generate Description
         ********************************************/

        DapQmlLabel {
            visible: !root.internal.generated
            qss: "gentri-gen-description c-label"
            text: qsTr("This key give you all the functionalities of\nKelVPN for 3 days.");
        }
    }
}

/*-----------------------------------------*/
