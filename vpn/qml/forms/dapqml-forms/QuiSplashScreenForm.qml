/* INCLUDES */

import QtQuick 2.12
import QtGraphicalEffects 1.5
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
    enabled: false

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "SplashScreen"

    /// @brief text color
    property color mainColor: "#FFFFFF"

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

    /****************************************//**
     * Gradient background
     ********************************************/

    Rectangle {
        id: rect
        z: 10
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: gradientColorTop.color } //"#E62CC7" }
            GradientStop { position: 1.0; color: gradientColorBottom.color } //"#F53172" }
        }
    }

    /****************************************//**
     * Stripes
     ********************************************/

    Image {
        z: 11
        anchors.fill: parent
        mipmap: true
        source: root.stripesPng
    }

    /****************************************//**
     * Logo
     ********************************************/

    DapQmlLabel {
        x: parent.width / 2 - width / 2
        y: 248
        z: 12
        width: 230
        height: 59
        qss: "splash-logo"
    }

    /****************************************//**
     * Status text
     ********************************************/

    DapQmlLabel {
        qss: "splash-status"
        z: 12
        width: parent.width
        text: root.statusText
        color: root.mainColor
    }

    /****************************************//**
     * Version text
     ********************************************/

    DapQmlLabel {
        qss: "splash-version"
        y: parent.height - height
        z: 12
        width: parent.width
        text: root.versionText
        color: root.mainColor
    }
}

/*-----------------------------------------*/
