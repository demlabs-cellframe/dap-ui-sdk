/* INCLUDES */

import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"
import "qrc:/dapqml-forms/tools"

/****************************************//**
 * @brief Dashboard Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * - Title
 * - Indicator with Connection Time label
 * - Error label
 * - Switch
 * - Speed indicators
 * - Switch Server button
 *
 * @date 06.06.22
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
    property string formName: "Dashboard"

    /* this is used only for width calc function _calcStatusWidth */
    //DapQmlStyle { id: style }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief switch toggled
    ///
    /// Will be sended before sigConnectionStatusChangeRequested
    signal sigSwitchToggle (bool checked);

    /// @brief "switch server" button clicked
    signal sigServerClicked();

    /// @brief will be sended on Switch clicked after sigSwitchToggle
    signal sigConnectionStatusChangeRequested();

    signal sigStartUpdate();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change status text
    function setStatusText(a_text) {
        statusLabel.text    = a_text;
    }

    /// @brief change switch state
    function setButtonState(a_checked) {
        dashboardSwitch.checked = a_checked;
    }

    /// @brief show error message
    function setErrorText(a_text) {
        dashboardError.text  = a_text;
    }

    function setServerChooseEnable(value) {
        serverChoose.setEnable(value);
    }

    function setSwitchButtonEnable(value) {
        dashboardSwitch.setEnable(value);
    }

    /// @brief change color of small icon close to connection time
    function setStatusIndicator(a_status) {
        if (a_status)
        {
            indicator.qss   = "dashboard-status-icon ic_online";
            statusLabel.qss = "dashboard-title c-brand";
        }
        else
        {
            indicator.qss   = "dashboard-status-icon ic_offline";
            statusLabel.qss = "dashboard-title c-label";
        }
        statusContainer.visible = a_status;
    }

    /// @brief change connection time
    function setTimeText(a_text) {
        statusTime.text = a_text;
    }

    /// @brief change speed indicators texts
    function updateSpeedIndicators(a_downSpeed, a_upSpeed) {
        speedDown.mainText  = a_downSpeed;
        speedUp.mainText    = a_upSpeed;
    }

    /// @brief change "switch server" button text
    function setServerInfo(a_name, a_ip) {
        serverChoose.mainText   = a_name;
        serverChoose.subText    = a_ip;
    }

//    function setTickerMessage(a_message, a_url) {
//        tickerLabel.text = a_message;
//        ticker.tickerUrl = a_url;
//        ticker.showTicker()
//    }

//    function showUpdateNotification(a_message) {
//        updateNotificationRect.showUpdateNotification()
//    }

    Component.onCompleted: setStatusIndicator(false);

    /// @}
//    /****************************************//**
//     * Ticker
//     ********************************************/

//    DapQmlRectangle {
//        id: ticker
//        objectName: "ticker"
//        y: -1 * ticker.height
//        qss: "ticker"
//        width: root.width
//        visible: false

//        property string tickerUrl:   ""
//        property bool tickerIsHidden: true

//        Behavior on y { PropertyAnimation { duration: 100 }}

//        onYChanged: updateNotificationRect._updatePos()

//        function showTicker() {
//            y = 0;
//            tickerIsHidden = false;
//        }

//        function hideTicker() {
//            y = -1 * ticker.height;
//            ticker.tickerIsHidden = true;
//        }

//        function tickerClicked() {
//            if (!ticker.tickerIsHidden)
//                Qt.openUrlExternally(ticker.tickerUrl);
//        }

//        function _updateTickerAnim() {
//            tickerAnimation.from    = tickerLableRect.width;
//            tickerAnimation.to      = 0 - tickerLabel.contentWidth;
//            tickerAnimation.running = true;
//        }

//        DapQmlRectangle {
//            id: tickerLableRect
//            objectName: "tickerLableRect"
//            qss: "ticker-lable-rect"
//            visible: true
//            anchors.left: parent.left

//            /* text */
//            DapQmlLabel {
//                id: tickerLabel
//                objectName: "tickerLabel"
//                width: contentWidth
//                qss: "ticker-label"
//                //text: tickerMessage
//                z: 2
//                horizontalAlign: Text.AlignHCenter
//                mipmap: false

//                onWidthChanged: ticker._updateTickerAnim()

//                NumberAnimation  {
//                    id: tickerAnimation
//                    objectName: "tickerAnimation"
//                    target: tickerLabel
//                    properties: "x"
//                    running: false
//                    duration: 10000
//                    loops: Animation.Infinite
//                }
//            }

//            MouseArea {
//                anchors.fill: tickerLableRect
//                z : 3
//                cursorShape: Qt.PointingHandCursor
//                onClicked: ticker.tickerClicked()
//            }

//            DapQmlRectangle {
//                id: tickerLabelBackgraund
//                qss: "ticker-label-background"
//                anchors.fill: parent
//            }
//        }

//        DapQmlRectangle {
//            id: tickerCloseRect
//            qss: "ticker-close-rect"
//            visible: true
//            anchors.right: parent.right

//            DapQmlPushButton {
//                id: tickerCloseButton
//                qss: "ticker-close-button"
//                x: parent.width - width - y
//                y: (parent.height - height) / 2
//                z: 14

//                onClicked: {
//                    ticker.hideTicker()
//                }
//            }

//            DapQmlRectangle {
//                id: tickerCloseBackground
//                qss: "ticker-label-background"
//                anchors.fill: parent
//            }
//        }
//    }

//     /****************************************//**
//      * Update notification
//      ********************************************/

//     DapQmlRectangle {
//         id: updateNotificationRect
//         qss: "update-notification-rect"
//         y: hidden
//            ? (statusLabel.y + statusLabel.height) //(ticker.tickerIsHidden ? 0 : updNotPosTickerOff.y)
//            : (statusLabel.y + statusLabel.height + updNotPosTickerOff.y) //(ticker.tickerIsHidden ? updNotPosTickerOff.y : updNotPosTickerOn.y)
//         z: 30
//         radius: 13
//         visible: true
//         opacity: 0

//         property bool hidden: true
//         //property real titlePos: statusLabel.y + statusLabel.height

//         Behavior on y { PropertyAnimation { duration: 100 }}
//         Behavior on opacity { PropertyAnimation { duration: 100 }}

//         function showUpdateNotification() {
//             updateNotificationButton.visible = true;
//             hidden     = false;
//             opacity    = 1;
//             _updatePos();
//         }

//         function hideUpdateNotification() {
//             updateNotificationButton.visible = false;
//             hidden     = true;
//             opacity    = 0;
//             _updatePos();
//         }

//         function _updatePos() {
//             y = hidden
//                 ? (statusLabel.y + statusLabel.height) //(ticker.tickerIsHidden ? 0 : updNotPosTickerOff.y)
//                 : (statusLabel.y + statusLabel.height + updNotPosTickerOff.y) //(ticker.tickerIsHidden ? updNotPosTickerOff.y : updNotPosTickerOn.y)
//             statusContainer._updatePos();
//         }

//         DapQmlDummy { id: updNotPosTickerOn;  qss: "update-notification-pos-ticker-on"  }
//         DapQmlDummy { id: updNotPosTickerOff; qss: "update-notification-pos-ticker-off" }

//         /* text */
//         DapQmlLabel {
//             id: updateNotificationLabel
//             qss: "update-notification-label"
//             text: "New version available"
//             height: contentHeight
//             width: contentWidth
//             horizontalAlign: Text.AlignHCenter
//         }

//         /* close button */
//         DapQmlPushButton {
//             id: updateNotificationCloseButton
//             x: parent.width - width - (y * 1.4)
//             z: 14

//             qss: "update-notification-close-button"

//             onClicked: updateNotificationRect.hideUpdateNotification()
//         }

//         /* update button */
//         DapQmlLabel {
//             id: updateNotificationButton
//             qss: "update-notification-button"
//             text: "Update"
//             height: contentHeight
//             width: contentWidth
//             horizontalAlign: Text.AlignHCenter

//             MouseArea {
//                 anchors.fill: updateNotificationButton
//                 z : 3
//                 enabled: updateNotificationRect.opacity !== 0
//                 cursorShape: Qt.PointingHandCursor
//                 onClicked: root.sigStartUpdate()
//             }
//         }
//    }

    /****************************************//**
     * Ticker & Update tools
     ********************************************/

    QuiToolTicker {}
    QuiToolUpdateNotification {
        dashboardStatusLabelBottom: statusLabel.y + statusLabel.height
        onSigVisibilityUpdated: statusContainer._updatePos()
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlLabel {
        id: statusLabel
        text: "Disconnected"
        qss: "dashboard-title c-label"
    }

//    DapQmlLabel {
//        width: contentWidth
//        height: contentHeight
//        text: "choose server"
//        onClicked: root.sigServerClicked()
//    }

    /****************************************//**
     * Status
     ********************************************/

    DapQmlRectangle {
        id: statusContainer
        y: statusPositioner.y
        qss: "dashboard-status-container"
        width: parent.width

        Behavior on y { PropertyAnimation { duration: 100 }}

        function _updatePos() {
            y = statusPositioner.y + (!TickerUpdateCtl.updateVisible ? 0 : statusContainer.height);
        }

//        Component.onCompleted: StyleDebugTree.describe (
//           "statusContainer",
//            ["x", "y", "width", "height"],
//           this);

        DapQmlDummy {
            id: statusPositioner
            qss: "dashboard-status-container-pos"
        }

        DapQmlLabel {
            id: statusTimeScaler
            qss: "dashboard-status-label-scaler"
        }

        Item{
            x: (parent.width - width) / 2
            width: indicator.width * 3 + statusTime.width
            height: parent.height

//            Component.onCompleted: StyleDebugTree.describe (
//               "statusContainer Column Layout",
//                ["x", "y", "width", "height"],
//               this);

            DapQmlLabel {
                id: indicator
                y: (statusContainer.height - height) / 2
                qss: "dashboard-status-icon ic_online"

//                Component.onCompleted: StyleDebugTree.describe (
//                   "indicator",
//                    ["x", "y", "width", "height"],
//                   this);
            }

            DapQmlLabel {
                id: statusTime
                x: indicator.width * 3
                width: contentWidth
                height: parent.height
                fontSize: statusTimeScaler.fontSize
                text: "00:00:00"
                qss: "dashboard-status-label"

//                Component.onCompleted: StyleDebugTree.describe (
//                   "statusTime",
//                    ["x", "y", "width", "height"],
//                   this);
            }
        }
    }

    /****************************************//**
     * Error label
     ********************************************/

    DapQmlLabel {
        id: dashboardError
        qss: "dashboard-error-label"
        wrapMode: Text.WordWrap
        //text: "Temporary network problems, request will be handled as soon as the network connection is re-established"
    }

    /****************************************//**
     * Switch
     ********************************************/

    DapQmlSwitch {
        id: dashboardSwitch
        qss: "dashboard-switch"
        onClicked: { root.sigSwitchToggle(checked); root.sigConnectionStatusChangeRequested(); }
    }

    /****************************************//**
     * Speed
     ********************************************/

    RowLayout {
        id: speedContainer

        DapQmlButton {
            id: speedDown
            Layout.fillWidth: true
            Layout.fillHeight: true
            mainText: "0 B/s"
            subText: qsTr("DOWNLOAD") + lang.notifier
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlButton {
            id: speedUp
            Layout.fillWidth: true
            Layout.fillHeight: true
            mainText: "0 B/s"
            subText: qsTr("UPLOAD") + lang.notifier
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlStyle { item: speedContainer; qss: "dashboard-speed-container" }
    }

    /****************************************//**
     * Change server button
     ********************************************/

    DapQmlButton {
        id: serverChoose
//        x: 0//centerHor(this)
//        y: 575
//        width: 410
//        height: 137
        link: true
        frame: true

        buttonStyle: DapQmlButton.TopSubBottomMain
        mainText: qsTr("Auto") + lang.notifier
        subText: qsTr("CHOOSING SERVER") + lang.notifier
        mainQss: "dashboard-server-main"
        subQss: "dashboard-server-sub"
        qss: "dashboard-server-container"
        onClicked: root.sigServerClicked()
    }
}

/*-----------------------------------------*/
