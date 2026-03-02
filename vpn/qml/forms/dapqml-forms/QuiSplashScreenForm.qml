/* INCLUDES */

import QtQuick 2.12
import Qt5Compat.GraphicalEffects
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
    signal sigTelegramBot();
    signal sigShareLog();

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

    DapQmlLabel {
        id: splashSettings
        visible: false
        qss: "c-text"
    }

    // Icon size resizer
    DapQmlLabel {
        id: iconResizer
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
        id: cdbHyperlink
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
        id: statusLabel
        qss: "splash-status c-label"
        z: 12
        width: parent.width
        text: root.statusText
        color: splashSettings.color
    }

    /****************************************//**
     * Simple Loading Spinner
     ********************************************/

    Item {
        id: loadingSpinner
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: statusLabel.bottom
        anchors.topMargin: 15
        width: 60
        height: 60
        z: 12
        
        Repeater {
            id: spinnerRepeater
            model: 8
            Rectangle {
                width: 6
                height: 6
                radius: 3
                color: statusLabel.color
                opacity: 1.0 - (index * 0.12)
                x: loadingSpinner.width / 2 - width / 2
                y: 6
                
                transform: Rotation {
                    origin.x: 3
                    origin.y: loadingSpinner.height / 2 - 6
                    angle: index * 45
                }
            }
        }
        
        RotationAnimation {
            id: spinnerAnimation
            target: loadingSpinner
            from: 0
            to: 360
            duration: 2000
            loops: Animation.Infinite
            running: true
        }
    }

    /****************************************//**
     * Connection issues text
     ********************************************/

    DapQmlLabel {
        id: connectionIssuesLabel
        qss: "splash-status c-label"
        z: 12
        width: parent.width
        anchors.top: loadingSpinner.bottom
        anchors.topMargin: 10
        anchors.left: statusLabel.left
        text: ""
        color: splashSettings.color
        opacity: 0
        
        property real animationOffset: 20
        transform: Translate { y: connectionIssuesLabel.animationOffset }
        
        Behavior on opacity {
            PropertyAnimation {
                duration: 800
                easing.type: Easing.OutQuad
            }
        }
        
        Behavior on animationOffset {
            PropertyAnimation {
                duration: 800
                easing.type: Easing.OutQuad
            }
        }
    }

    /****************************************//**
     * Support Buttons Row
     ********************************************/

    Row {
        id: supportButtonsRow
        visible: root.enabled
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: versionLabel.top
        anchors.bottomMargin: 15
        spacing: 15
        z: 13
        opacity: 0
        
        property real animationOffset: 30
        transform: Translate { y: supportButtonsRow.animationOffset }
        
        Behavior on opacity {
            PropertyAnimation {
                duration: 1000
                easing.type: Easing.OutQuad
            }
        }
        
        Behavior on animationOffset {
            PropertyAnimation {
                duration: 1000
                easing.type: Easing.OutQuad
            }
        }
        
        // Custom Support Button
        Rectangle {
            id: telegramButton
            width: 100
            height: 35
            color: "transparent"
            border.width: 1
            border.color: statusLabel.color
            radius: 5
            
            Text {
                anchors.centerIn: parent
                text: qsTr("Support")
                color: statusLabel.color
                font.pixelSize: 14
            }
            
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    console.log("Telegram button clicked!")
                    root.sigTelegramBot()
                }
            }
        }

        // Custom Logs Button  
        Rectangle {
            id: shareLogsButton
            width: 100
            height: 35
            color: "transparent"
            border.width: 1
            border.color: statusLabel.color
            radius: 5
            
            Text {
                anchors.centerIn: parent
                text: qsTr("Logs")
                color: statusLabel.color
                font.pixelSize: 14
            }
            
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    console.log("Share logs button clicked!")
                    root.sigShareLog()
                }
            }
        }
    }

    /****************************************//**
     * Version text
     ********************************************/

    DapQmlLabel {
        id: versionLabel
        qss: "splash-version c-label"
        y: parent.height - height
        z: 12
        width: parent.width
        text: root.versionText
        color: splashSettings.color
    }

    /****************************************//**
     * @name ADDITIONAL SIGNALS
     ********************************************/
    /// @{

    /// @}

    // Timer for connection issues message
    Timer {
        id: connectionIssuesTimer
        interval: 5000
        running: true
        repeat: false
        onTriggered: {
            if (root.enabled && root.visible) {
                connectionIssuesLabel.text = "Connection issues. Please contact support."
                connectionIssuesLabel.opacity = 1.0
                connectionIssuesLabel.animationOffset = 0
            }
        }
    }

    // Timer for support buttons (delayed for cascading effect)
    Timer {
        id: supportButtonsTimer
        interval: 5500
        running: true
        repeat: false
        onTriggered: {
            if (root.enabled && root.visible) {
                supportButtonsRow.opacity = 1.0
                supportButtonsRow.animationOffset = 0
            }
        }
    }

    onVisibleChanged: {
        if (!visible) {
            connectionIssuesTimer.stop()
            supportButtonsTimer.stop()
            connectionIssuesLabel.text = ""
            connectionIssuesLabel.opacity = 0
            connectionIssuesLabel.animationOffset = 20
            supportButtonsRow.opacity = 0
            supportButtonsRow.animationOffset = 30
        }
    }
}

/*-----------------------------------------*/
