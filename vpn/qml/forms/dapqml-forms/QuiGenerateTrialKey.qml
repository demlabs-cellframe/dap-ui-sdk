/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import StyleDebugTree 1.0
import InterfaceManager 1.0
import ShelfCtl 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Generate Trial Key Form
 * @ingroup groupDapQmlForms
 *
 * @date 13.07.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlRectangle {
    id: root
    clip: true

    qss: {
        if (!root.internal.generated)
            return "gentri-request-rect";
        else
            return "gentri-serialkey-rect";
    }

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
        property string key

        function updateTitle() {
            ShelfCtl.title = !generated
                             ? qsTr("Generate trial key") + lang.notifier
                             : qsTr("Serial key") + lang.notifier
        }

        onGeneratedChanged: updateTitle()
        Component.onCompleted: updateTitle()
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigGenerate();
    signal sigActivate();

    //Component.onCompleted: reset()
    Component.onCompleted: {
        reset();

        // test purposes //
        //setKey("TEST-TEST-TEST-TEST");

        InterfaceManager.setupForm (root);
    }

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
        root.internal.key       = a_key;
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
     * Action button
     ********************************************/

    DapQmlRectangle {
        y: parent.height - height
        qss: "gentri-action-rect"

        Behavior on y { PropertyAnimation { duration: 250 } }

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

/*-----------------------------------------*/
