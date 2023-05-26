/* INCLUDES */

import QtQuick 2.15
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Qui Tool Update Notification
 * @date 26.05.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlRectangle {
    id: root
    qss: "update-notification-rect"
    y: hidden
       ? (statusLabel.y + statusLabel.height)
       : (statusLabel.y + statusLabel.height + updNotPosTickerOff.y)
    z: 30
    radius: 13
    visible: true
    opacity: 0

    property string updateMessage
    property bool hidden: true
    //property real titlePos: statusLabel.y + statusLabel.height

    Behavior on y { PropertyAnimation { duration: 100 }}
    Behavior on opacity { PropertyAnimation { duration: 100 }}

    function showUpdateNotification() {
        updateNotificationButton.visible = true;
        hidden     = false;
        opacity    = 1;
        _updatePos();
    }

    function hideUpdateNotification() {
        updateNotificationButton.visible = false;
        hidden     = true;
        opacity    = 0;
        _updatePos();
    }

    function _updatePos() {
        y = hidden
            ? (statusLabel.y + statusLabel.height)
            : (statusLabel.y + statusLabel.height + updNotPosTickerOff.y)
        statusContainer._updatePos();
    }

    DapQmlDummy { id: updNotPosTickerOn;  qss: "update-notification-pos-ticker-on"  }
    DapQmlDummy { id: updNotPosTickerOff; qss: "update-notification-pos-ticker-off" }

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
