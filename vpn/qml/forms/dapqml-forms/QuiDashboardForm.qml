/* INCLUDES */

import QtQuick 2.0
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

    Component.onCompleted: setStatusIndicator(false);

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
                text: "00 : 00 : 00"
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
            mainText: "0 kB/s"
            subText: "DOWNLOAD"
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlButton {
            id: speedUp
            Layout.fillWidth: true
            Layout.fillHeight: true
            mainText: "0 kB/s"
            subText: "UPLOAD"
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
        mainText: "North America - Canada"
        subText: "231:532:531:20"
        mainQss: "dashboard-server-main"
        subQss: "dashboard-server-sub"
        qss: "dashboard-server-container"
        onClicked: root.sigServerClicked()
    }
}

/*-----------------------------------------*/
