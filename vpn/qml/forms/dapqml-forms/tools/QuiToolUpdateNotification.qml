/* INCLUDES */

import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Qui Tool Update Notification
 * @date 26.05.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlDummy {
    id: root
    x: (parent.width - width) / 2
    width: contentSizer.width * 1.25
    height: contentSizer.height * 1.25

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    property string color: contentSizer.color
    property bool isDashobard: false
    property string updateMessage // TickerUpdateCtl.updateMessage
    property bool showingTicker: TickerUpdateCtl.tickerVisible
    property bool showingUpdate: TickerUpdateCtl.updateVisible

    /// @}
    /****************************************//**
     * @name STATES
     ********************************************/
    /// @{

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
        console.log(`update showing: ${showingUpdate}`);
        if (showingUpdate)
            showUpdateNotification();
        else
            hideUpdateNotification();
    }

    onIsDashobardChanged: {
        state = !isDashobard ? "onLogin" : "onDashboard"
        //console.log (`new updnot state: ${state} , ${y}|${TickerUpdateCtl.loginUpdNotPos}|${TickerUpdateCtl.dashUpdNotPos}`);
    }

    //onYChanged: console.log (`updnot[${isDashobard}] y: ${y}|${TickerUpdateCtl.loginUpdNotPos}|${TickerUpdateCtl.dashUpdNotPos}`)

//    onOpacityChanged:
//        if (opacity < 0.1)
//            TickerUpdateCtl.updateVisible = false

    Component.onCompleted: hideUpdateNotification()

//    Component.onCompleted: StyleDebugTree.describe (
//       "upd-not" + isDashobard,
//        ["x", "y", "width", "height", "showingUpdate", "loginPos", "dashboardPos", "dashboardStatusLabelBottom"],
//       this);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function showUpdateNotification() {
        TickerUpdateCtl.updateVisible   = true;
        opacity    = 1;
    }

    function hideUpdateNotification() {
        TickerUpdateCtl.updateVisible   = false;
        opacity    = 0;
    }

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/
    /// @{

    DapQmlDummy { id: contentSizer; qss: "update-notification-rect"; property string color }
    DapQmlDummy { id: updNotPosTickerOn;  qss: "update-notification-pos-ticker-on"  }
    DapQmlDummy { id: updNotPosTickerOff; qss: "update-notification-pos-ticker-off" }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    DapQmlRectangle {
        id: contentRoot
        color: root.color
        radius: height / 6.5
        z: 20
        enabled: root.opacity !== 0

        anchors.centerIn: parent
        width: contentSizer.width
        height: contentSizer.height

        /* text */
        DapQmlLabel {
            id: updateNotificationLabel
            x: (parent.width - width) / 2
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
            x: (parent.width - width) / 2
            qss: "update-notification-button"
            text: "Update"
            height: contentHeight
            width: contentWidth
            horizontalAlign: Text.AlignHCenter

            MouseArea {
                anchors.fill: updateNotificationButton
                z : 3
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.sigStartUpdate();
                    TickerUpdateCtl.startUpdate();
                }
            }
        }
    }

    /****************************************//**
     * Content Shadow
     ********************************************/

    DropShadow {
        anchors.fill: contentRoot
        z: 10
        radius: 18//contentRoot.radius
        samples: 17
        color: `#30${contentShadowColor.color.substring(1)}`
        source: contentRoot

        DapQmlDummy {
            id: contentShadowColor
            property string color
            qss: "notification-shadow"
        }
    }
}

/*-----------------------------------------*/
