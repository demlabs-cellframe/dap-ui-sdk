/* INCLUDES */

import QtQuick 2.15
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Qui Tool Update Notification
 * @date 26.05.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlRectangle {
    id: root
    qss: "update-notification-rect"
//    y: !isDashobard
//       ? TickerUpdateCtl.loginUpdNotPos
//       : TickerUpdateCtl.dashUpdNotPos
//    y: dashboardStatusLabelBottom === 0
//       ? loginPos
//       : dashboardPos
    z: 30
    radius: 13
    visible: opacity > 0
    opacity: 0

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property bool isDashobard: false
    property string updateMessage // TickerUpdateCtl.updateMessage
    //property real titlePos: statusLabel.y + statusLabel.height

    property bool showingTicker: TickerUpdateCtl.tickerVisible
    property bool showingUpdate: TickerUpdateCtl.updateVisible
//    property real loginPos: !TickerUpdateCtl.updateVisible
//                            ? (!TickerUpdateCtl.tickerVisible ? 0 : updNotPosTickerOff.y)
//                            : (!TickerUpdateCtl.tickerVisible ? updNotPosTickerOff.y : updNotPosTickerOn.y)
//    property real dashboardPos: !TickerUpdateCtl.updateVisible
//                                ? (dashboardStatusLabelBottom + updNotPosTickerOn.y)
//                                : (dashboardStatusLabelBottom + updNotPosTickerOff.y)
//    property real dashboardStatusLabelBottom // (statusLabel.y + statusLabel.height)

    state: "onLogin"
    states: [
        State {
            name: "onLogin"
            PropertyChanges { target: root; y: TickerUpdateCtl.loginUpdNotPos }
        },
        State {
            name: "onDashboard"
            PropertyChanges { target: root; y: TickerUpdateCtl.dashUpdNotPos }
        }
    ]

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigStartUpdate();

    Behavior on y { PropertyAnimation { duration: 100 }}
    Behavior on opacity { PropertyAnimation { duration: 100 }}

    onShowingUpdateChanged: {
        if (showingUpdate)
            showUpdateNotification();
        else
            hideUpdateNotification();
//        _updatePos();
    }
//    onShowingTickerChanged: _updatePos()
//    onLoginPosChanged: _updatePos()
//    onDashboardPosChanged: _updatePos()
//    onDashboardStatusLabelBottomChanged: _updatePos()

    onIsDashobardChanged: {
        state = !isDashobard ? "onLogin" : "onDashboard"
        //console.log (`new updnot state: ${state} , ${y}|${TickerUpdateCtl.loginUpdNotPos}|${TickerUpdateCtl.dashUpdNotPos}`);
    }

    //onYChanged: console.log (`updnot[${isDashobard}] y: ${y}|${TickerUpdateCtl.loginUpdNotPos}|${TickerUpdateCtl.dashUpdNotPos}`)

    onOpacityChanged:
        if (opacity < 0.1)
            TickerUpdateCtl.updateVisible = false

    Component.onCompleted: StyleDebugTree.describe (
       "upd-not" + isDashobard,
        ["x", "y", "width", "height", "showingUpdate", "loginPos", "dashboardPos", "dashboardStatusLabelBottom"],
       this);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function showUpdateNotification() {
        opacity    = 1;
    }

    function hideUpdateNotification() {
        opacity    = 0;
    }

//    function _updatePos() {
//        y = dashboardStatusLabelBottom === 0
//            ? loginPos
//            : dashboardPos
////        root.sigVisibilityUpdated() // statusContainer._updatePos();
//    }

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/
    /// @{

    DapQmlDummy { id: updNotPosTickerOn;  qss: "update-notification-pos-ticker-on"  }
    DapQmlDummy { id: updNotPosTickerOff; qss: "update-notification-pos-ticker-off" }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    /* text */
    DapQmlLabel {
        id: updateNotificationLabel
        qss: "update-notification-label"
        text: updateMessage.length === 0 ? "New version available" : updateMessage
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

        onClicked: root.hideUpdateNotification()
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
            enabled: root.opacity !== 0
            cursorShape: Qt.PointingHandCursor
            onClicked: root.sigStartUpdate()
        }
    }
}

/*-----------------------------------------*/
