/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import StyleDebugTree 1.0
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

    property bool show: false

    property QtObject internal: QtObject {
        property bool generated: false
        property string key
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigClose();
    signal sigGenerate();
    signal sigActivate();

    onShowChanged: {
        if (show)
            root.reset();
        else
            root.close();
    }

    //Component.onCompleted: reset()
    Component.onCompleted: {
        close();

        // test purposes //
        //setKey("TEST-TEST-TEST-TEST");
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function reset() {
        root.internal.generated = false;
        actionButton.text       = qsTr("GENERATE KEY");
        content.y               = parent.height - sizerRequest.height;
    }

    function setKey(a_key) {
        root.internal.generated = true;
        actionButton.text       = qsTr("ACTIVATE");
        root.internal.key       = a_key;
        content.y               = parent.height - sizerSerialKey.height;
    }

    function close() {
        content.y               = parent.height * 1.1;
    }

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/
    /// @{

    DapQmlDummy {
        id: sizerRequest
        qss: "gentri-request-rect c-background"
    }

    DapQmlDummy {
        id: sizerSerialKey
        qss: "gentri-serialkey-rect c-background"
    }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    DapQmlRectangle {
        id: content
        //anchors.bottom: parent.bottom
        clip: true
        qss: {
            if (!root.internal.generated)
                return "gentri-request-rect c-background";
            else
                return "gentri-serialkey-rect c-background";
        }

        Behavior on y { PropertyAnimation { duration: 250 } }
        Behavior on height { PropertyAnimation { duration: 250 } }

        onQssChanged: {
            if (qss == "c-background")
                height = 0;
        }

        DapQmlRectangle {
            anchors.bottom: parent.bottom
            anchors.topMargin: parent.radius * 3
            anchors.fill: parent
            color: parent.color
            z: 2
        }

        /****************************************//**
         * Title
         ********************************************/

        DapQmlLabel {
            z: 10
            qss: "gentri-title c-label"
            text: !root.internal.generated
                  ? qsTr("Generate trial key")
                  : qsTr("Serial key")
        }

        /****************************************//**
         * Close Button
         ********************************************/

        DapQmlPushButton {
            x: parent.width - width - (y * 3)
            z: 14
            qss: "gentri-close-btn"
            onClicked: {
//                console.log("boi");
//                root.show = false;
                root.sigClose();
            }
        }

        /****************************************//**
         * Action button
         ********************************************/

        DapQmlRectangle {
            anchors.bottom: parent.bottom
            qss: "gentri-action-rect"
            z: 4

            DapQmlSeparator {
                qss: "gentri-action-separator"
            }

            DapQmlPushButton {
                id: actionButton
                qss: "gentri-action-btn push-button"
                onClicked: {
                    root.internal.generated
                    ? root.sigActivate()
                    : root.sigGenerate();
                    root.setKey("TEST-TEST-TEST-TEST");
                }
            }
        }

        /****************************************//**
         * Generate Description
         ********************************************/

        DapQmlLabel {
            visible: !root.internal.generated
            qss: "gentri-gen-description"
            z: 4
            text: qsTr("This key give you all the functionalities of\nKelVPN for 3 days.");

//            Component.onCompleted: StyleDebugTree.describe (
//               "gentri-gen-description",
//                ["x", "y", "width", "height", "visible", "color", "text"],
//               this);
        }

        /****************************************//**
         * Serial Key
         ********************************************/

        Item {
            anchors.fill: parent
            z: 4
            visible: root.internal.generated

            /* key */
            DapQmlLabel {
                qss: "gentri-serkey-description"
                text: root.internal.key
            }

            /* bottom label */
            DapQmlLabel {
                qss: "gentri-serkey-label"
                text: qsTr("SERIAL KEY")
            }

            /* copy button */
        }
    }
}

/*-----------------------------------------*/
