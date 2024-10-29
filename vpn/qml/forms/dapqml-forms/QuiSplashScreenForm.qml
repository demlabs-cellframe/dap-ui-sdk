/* INCLUDES */

import QtQuick 2.12
import QtGraphicalEffects 1.5
import Brand 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Splash Screen Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * - Background gradient
 * - Stripes on top of the gradient
 * - Logo
 * - Status
 * - Version
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true
    enabled: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "SplashScreen"

    /// @brief default status text
    property string statusText: "Loading..."

    /// @brief default version text
    property string versionText: "..."

    /// @brief stripes filename
    property string stripesPng: "qrc:/stripes.png"

    /// @brief logo filename
    property string logoPng: "qrc:/logo.png"

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    ///
    signal sigShowCdbManager();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change status label text
    function setStatusText(a_message) {
        statusText  = a_message;
    }

    /// @brief change version label text
    function setVersion(a_version) {
        versionText = a_version;
    }

    /// @brief free memory used by PNG's
    function freeMem() {
        stripesPng  = "";
        logoPng     = "";
    }

//    Timer {
//        interval: 10
//        running: true
//        repeat: false
//        onTriggered: rect.z = 10
//    }

    /// @}
    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlLabel {
        id: gradientColorTop
        visible: false
        qss: "splash-gradient-top"
    }

    DapQmlLabel {
        id: gradientColorBottom
        visible: false
        qss: "splash-gradient-bottom"
    }

    DapQmlLabel {
        id: splashSettings
        visible: false
        qss: "c-text"
    }

    /****************************************//**
     * Background
     ********************************************/

    DapQmlRectangle {
        anchors.fill: parent
        z: 10
        qss: "c-background"
    }

    /****************************************//**
     * Gradient background
     ********************************************/

//    Rectangle {
//        id: rect
//        z: 20
//        anchors.fill: parent

//        gradient: Gradient {
//            GradientStop { position: 0.0; color: gradientColorTop.color } //"#E62CC7" }
//            GradientStop { position: 1.0; color: gradientColorBottom.color } //"#F53172" }
//        }
//    }

    /****************************************//**
     * Stripes
     ********************************************/

//    DapQmlImage {
//        z: 11
//        anchors.fill: parent
//        scaledPixmap: root.stripesPng
//    }

    /****************************************//**
     * Logo
     ********************************************/

    DapQmlLabel {
        x: parent.width / 2 - width / 2
        z: 12
        qss: "splash-logo"
    }

    /****************************************//**
     * Text hyperlink
     ********************************************/

    DapQmlLabel {
        visible: root.enabled
        qss: "splash-hyperlink c-label"
        z: 12
        width: parent.width
        text: qsTr("Tap here to show cdb management")
        color: splashSettings.color
        onClicked: {
            root.sigShowCdbManager()
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            enabled: false
        }
    }

    /****************************************//**
     * Status text
     ********************************************/

    DapQmlLabel {
        qss: "splash-status c-label"
        z: 12
        width: parent.width
        text: root.statusText
        color: splashSettings.color
    }

    /****************************************//**
     * Version text
     ********************************************/

    DapQmlLabel {
        qss: "splash-version c-label"
        y: parent.height - height
        z: 12
        width: parent.width
        text: root.versionText
        color: splashSettings.color
    }
}

/*-----------------------------------------*/
