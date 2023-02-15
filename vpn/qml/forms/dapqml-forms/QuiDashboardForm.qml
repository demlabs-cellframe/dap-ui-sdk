/* INCLUDES */

import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

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

    function setTickerMessage(a_message, a_url) {
        tickerLabel.text = a_message;
        ticker.tickerUrl = a_url;
        ticker.showTicker()
    }

    function showUpdateNotification(a_message) {
        updateNotificationRect.showUpdateNotification()
    }



    Component.onCompleted: setStatusIndicator(false);

    /****************************************//**
    * Ticker
    ********************************************/

   DapQmlRectangle {
       id: ticker
       objectName: "ticker"
       qss: "ticker"
       width: root.width
       visible: false

       property string tickerUrl:   ""
       property bool tickerIsHidden: true

       function showTicker() {
           hideAnimation.from = -1 * ticker.height
           hideAnimation.to = 0
           hideAnimation.running = true
           tickerIsHidden = false
       }

       function hideTicker() {
           hideAnimation.from = 0
           hideAnimation.to = -1 * ticker.height
           hideAnimation.running = true
           ticker.tickerIsHidden = true
           updateNotificationRect.moveAfterHideTicker()
       }

       function tickerClicked() {
           Qt.openUrlExternally(ticker.tickerUrl);
       }

       DapQmlRectangle {
           id: tickerLableRect
           objectName: "tickerLableRect"
           qss: "ticker-lable-rect"
           visible: true
           anchors.left: parent.left

           DapQmlLabel {
               id: tickerLabel
               objectName: "tickerLabel"
               width: contentWidth
               qss: "ticker-label"
               text: tickerMessage
               z: 2
               horizontalAlign: Text.AlignHCenter
               mipmap: false

               NumberAnimation  {
                   id: tickerAnimation
                   objectName: "tickerAnimation"
                   target: tickerLabel
                   properties: "x"
                   from: tickerLableRect.width
                   duration: 10000
                   loops: Animation.Infinite
               }
           }

           MouseArea {
               anchors.fill: tickerLableRect
               z : 3
               cursorShape: Qt.PointingHandCursor
               onClicked: ticker.tickerClicked()
           }

           DapQmlRectangle {
               id: tickerLabelBackgraund
               qss: "ticker-label-background"
               anchors.fill: parent
           }
       }

       DapQmlRectangle {
           id: tickerCloseRect
           qss: "ticker-close-rect"
           visible: true
           anchors.right: parent.right

           DapQmlPushButton {
               id: tickerCloseButton
               qss: "ticker-close-button"
               x: parent.width - width - y
               y: (parent.height - height) / 2
               z: 14

               onClicked: {
                   ticker.hideTicker()
               }
           }

           DapQmlRectangle {
               id: tickerCloseBackground
               qss: "ticker-label-background"
               anchors.fill: parent
           }
       }

       NumberAnimation {
           id: hideAnimation
           objectName: "tickerHideAnimation"
           target: ticker
           properties: "y"
           duration: 100
           running: false
       }
   }

   /****************************************//**
    * Update notification
    ********************************************/

    DapQmlRectangle {
        id: updateNotificationRect
        qss: "update-notification-rect"
        z:30
        radius: 13
        visible: true
        opacity: 0

        function showUpdateNotification() {
            hideAnimationUpdateNotification.from    = ticker.tickerIsHidden ? 0 : updNotPosTickerOff.y
            hideAnimationUpdateNotification.to      = ticker.tickerIsHidden ? updNotPosTickerOff.y : updNotPosTickerOn.y
            hideAnimationUpdateNotification.running = true

            hideAnimationUpdateNotificationOpacity.from     = 0
            hideAnimationUpdateNotificationOpacity.to       = 1
            hideAnimationUpdateNotificationOpacity.running  = true
        }

        function hideUpdateNotification() {
            hideAnimationUpdateNotification.from    = ticker.tickerIsHidden ? updNotPosTickerOff.y : updNotPosTickerOn.y
            hideAnimationUpdateNotification.to      = ticker.tickerIsHidden ? 0 : updNotPosTickerOff.y
            hideAnimationUpdateNotification.running = true

            hideAnimationUpdateNotificationOpacity.from     = 1
            hideAnimationUpdateNotificationOpacity.to       = 0
            hideAnimationUpdateNotificationOpacity.running  = true
        }

        function moveAfterHideTicker() {
            hideAnimationUpdateNotification.from    = updNotPosTickerOn.y // 35
            hideAnimationUpdateNotification.to      = updNotPosTickerOff.y
            hideAnimationUpdateNotification.running = true
        }

        DapQmlDummy { id: updNotPosTickerOn;  qss: "update-notification-pos-ticker-on"  }
        DapQmlDummy { id: updNotPosTickerOff; qss: "update-notification-pos-ticker-off" }

        /* text */
        DapQmlLabel {
            id: updateNotificationLabel
            qss: "update-notification-label"
            text: "New version available"
            height: contentHeight
            width: contentWidth
            horizontalAlign: Text.AlignHCenter
        }

        /* close button */
        DapQmlPushButton {
            id: updateNotificationCloseButton
            x: parent.width - width - (y * 1.4)
            z: 14

            qss: "update-notification-close-button"

            onClicked: updateNotificationRect.hideUpdateNotification()
        }

        /* update button */
        DapQmlLabel {
            id: updateNotificationButton
            qss: "update-notification-button"
            text: "Update"
            height: contentHeight
            width: contentWidth
            horizontalAlign: Text.AlignHCenter

            MouseArea {
                anchors.fill: updateNotificationButton
                z : 3
                cursorShape: Qt.PointingHandCursor
                onClicked: root.sigStartUpdate()
            }
        }

        /* animations */

        NumberAnimation {
            id: hideAnimationUpdateNotification
            objectName: "hideAnimationUpdateNotification"
            target: updateNotificationRect
            properties: "y"
            duration: 100
            running: false
        }

        OpacityAnimator on opacity{
            id: hideAnimationUpdateNotificationOpacity
            target: updateNotificationRect
            duration: 100
            running: false
        }
    }


    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlLabel {
        id: statusLabel
        text: "Disconnected"
        qss: "dashboard-title c-label"
    }

    /****************************************//**
     * Status
     ********************************************/

    DapQmlRectangle {
        id: statusContainer
        qss: "dashboard-status-container"
        width: parent.width

//        Component.onCompleted: StyleDebugTree.describe (
//           "statusContainer",
//            ["x", "y", "width", "height"],
//           this);

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
