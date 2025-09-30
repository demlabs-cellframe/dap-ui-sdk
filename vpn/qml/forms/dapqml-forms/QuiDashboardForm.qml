/* INCLUDES */

import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import com.DapQmlWidgets 1.0
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

    property QtObject internal: QtObject {
        property bool allowChooseServer: true
        property bool noCdbMode: false
        property bool nodeDetected: false
    }

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

    signal sigTitleButtonBackClicked();
    signal sigTitleButtonOrdersClicked();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change status text
    function setStatusText(a_text) {
        if (a_text !== "")
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
        //serverChoose.setEnable(value);
        root.internal.allowChooseServer = value;
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
        serverChooseSpinner.visible = a_name === "";
    }

    function setNoCDBServerInfo(a_name, a_ip) {
        noCdbServerChoose.mainText   = a_name;
        noCdbServerChoose.subText    = a_ip;
    }

//    function setTickerMessage(a_message, a_url) {
//        tickerLabel.text = a_message;
//        ticker.tickerUrl = a_url;
//        ticker.showTicker()
//    }

//    function showUpdateNotification(a_message) {
//        updateNotificationRect.showUpdateNotification()
//    }

    function setNoCdbMode(a_value) {
        root.internal.noCdbMode = a_value;
    }

    function setNodeDetected(a_value) {
        root.internal.nodeDetected = a_value;
    }

    Component.onCompleted: setStatusIndicator(false);

    /// @}
    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: titleButton

        //property int type

        DapQmlRectangle {
            qss: "dashboard-nocdb-title-button"
            radius: height / 5
            enabled: root.internal.noCdbMode || root.internal.nodeDetected
            visible: root.internal.noCdbMode || root.internal.nodeDetected
            opacity: enabled * 0.5 + 0.5

            DapQmlImage {
                anchors.fill: parent
                anchors.margins: parent.width / 10
                scaledPixmap: parent.parent.type === 0
                              ? dashboardNoCdbTitleButtonBack.scaledPixmap
                              : dashboardNoCdbTitleButtonOrders.scaledPixmap
            }

            MouseArea {
                anchors.fill: parent
                onClicked: parent.parent.type === 0
                           ? root.sigTitleButtonBackClicked()
                           : root.sigTitleButtonOrdersClicked()
            }
        }
    }

    /****************************************//**
     * Ticker & Update tools
     ********************************************/

    QuiToolTicker {}
    QuiToolUpdateNotification {
        id: updNot
        isDashobard: true
        //dashboardStatusLabelBottom: statusLabel.y + statusLabel.height
        //onSigVisibilityUpdated: statusContainer._updatePos()
        onSigStartUpdate: root.sigStartUpdate()
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlLabel {
        id: statusLabel
        text: qsTr("VPN Disconnected")
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
        //y: TickerUpdateCtl.dashStatConPos // statusPositioner.y + (!TickerUpdateCtl.updateVisible ? 0 : statusContainer.height);
        qss: "dashboard-status-container"
        width: parent.width

        Behavior on y { PropertyAnimation { duration: 100 }}

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
        y: TickerUpdateCtl.dashErrorPos
        qss: "dashboard-error-label"
        Behavior on y { PropertyAnimation { duration: 100 }}
        wrapMode: Text.WordWrap
        //text: "Temporary network problems, request will be handled as soon as the network connection is re-established"
    }

    /****************************************//**
     * Switch
     ********************************************/

    DapQmlSwitch {
        id: dashboardSwitch
        qss: "dashboard-switch"
        //onClicked: { root.sigSwitchToggle(checked); root.sigConnectionStatusChangeRequested(); }
        onCheckedChanged: { root.sigSwitchToggle(checked); root.sigConnectionStatusChangeRequested(); }
        Component.onCompleted: dashboardSwitchItem = this
    }

    /****************************************//**
     * NoCDB Content
     ********************************************/

    /* sizers */

    DapQmlDummy {
        id: dashboardNoCdbIndicatorSizer
        qss: "dashboard-nocdb-shield"
    }

    DapQmlDummy {
        id: dashboardNoCdbTitleButtonBack
        qss: "ic_back"
        property string scaledPixmap
    }

    DapQmlDummy {
        id: dashboardNoCdbTitleButtonOrders
        qss: "ic_orders"
        property string scaledPixmap
    }

    DapQmlDummy {
        id: dashboardNoCdbTitleButtonBackPos
        qss: "dashboard-nocdb-title-button-back"
    }

    DapQmlDummy {
        id: dashboardNoCdbTitleButtonOrdersPos
        qss: "dashboard-nocdb-title-button-orders"
    }

    /* indicator */

    RowLayout {
        id: dashboardNoCdbIndicator
        anchors.horizontalCenter: dashboardSwitch.horizontalCenter
        anchors.top: dashboardSwitch.bottom
        //width: childrenRect.width
        height: dashboardNoCdbIndicatorSizer.height
        visible: root.internal.noCdbMode

        DapQmlLabel {
            Layout.preferredWidth: dashboardNoCdbIndicatorSizer.width
            Layout.preferredHeight: dashboardNoCdbIndicatorSizer.height
            qss: "ic_nocdb-shield"
        }

        DapQmlLabel {
            Layout.preferredWidth: contentWidth
            Layout.preferredHeight: dashboardNoCdbIndicatorSizer.height
            horizontalAlign: Text.AlignLeft
            text: "NoCDB Mode"
            qss: "dashboard-nocdb-label"
        }
    }

    /* buttons */

    Loader {
        anchors.fill: dashboardNoCdbTitleButtonBackPos
        sourceComponent: titleButton
        //enabled: root.internal.allowChooseServer
        property int type: 0
    }

    Loader {
        anchors.fill: dashboardNoCdbTitleButtonOrdersPos
        sourceComponent: titleButton
        enabled: root.internal.allowChooseServer
        property int type: 1
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
        link: true
        frame: true
        visible: !root.internal.noCdbMode
        enabled: root.internal.allowChooseServer

        buttonStyle: DapQmlButton.TopSubBottomMain
        mainText: qsTr("Auto") + lang.notifier
        subText: qsTr("CHOOSING SERVER") + lang.notifier
        mainQss: "dashboard-server-main"
        subQss: "dashboard-server-sub"
        qss: "dashboard-server-container"
        onClicked: root.sigServerClicked()

        DapQmlArcAnimation {
            id: serverChooseSpinner
            anchors.centerIn: parent
            width: parent.height * 0.75
            height: width
            qss: "c-dashboard-accent"
        }
    }

    DapQmlButton {
        id: noCdbServerChoose
        link: true
        frame: true
        visible: root.internal.noCdbMode
        enabled: root.internal.allowChooseServer

        buttonStyle: DapQmlButton.TopSubBottomMain
        mainText: qsTr("Order") + lang.notifier
        subText: qsTr("SEARCH ORDERS") + lang.notifier
        mainQss: "dashboard-server-main"
        subQss: "dashboard-server-sub"
        qss: "dashboard-server-container"
        onClicked: root.sigServerClicked()
    }
}

/*-----------------------------------------*/
