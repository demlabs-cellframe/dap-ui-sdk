/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"
import QmlSchedulePainter 1.0
import DapQmlStyle 1.0

/****************************************//**
 * @brief Statistics Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * - 2 resizers
 * - Graph provided by QmlSchedulePainter
 * - 2 rows of indicators
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
    property string formName: "Statistics"

    /// @brief coordinate for left row items (calculated on resize)
    property int leftColumnPos: 42

    /// @brief coordinate for right row items (calculated on resize)
    property int rightColumnPos: 228

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change all indicators text
    function updateIndicators(a_bytesReceived, a_bytesSent,
                              a_packetsReceived, a_packetsSent,
                              a_uploadSpeed, a_downloadSpeed,
                              a_uploadSpeedString, a_downloadSpeedString)
    {
        stBytesRec.mainText     = a_bytesReceived;
        stBytesSent.mainText    = a_bytesSent;
        stPacketsRec.mainText   = a_packetsReceived;
        stPacketsSent.mainText  = a_packetsSent;
        stDownSpeed.mainText    = a_downloadSpeedString;
        stUpSpeed.mainText      = a_downloadSpeedString;
    }

    /// @brief change uptime label text
    function setUptime (a_text) {
        stUptime.mainText   = a_text;
    }

    /// @brief resize font for provided DapQmlButton
    function resizeFont(a_btn) {
        a_btn.labelMain.fontSize  = fontScalerIndicator.fontSize;
        a_btn.labelSub.fontSize   = fontScalerSubLabel.fontSize;
    }

    /// @brief calculate column positions
    function moveColumns() {
        leftColumnPos   = (root.width / 2 - fontScalerIndicator.width) / 2
        rightColumnPos  = (root.width / 2 - fontScalerIndicator.width) / 2 + root.width / 2
    }

    onWidthChanged: moveColumns()
    onHeightChanged: moveColumns()

    /// @}
    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlLabel {
        id: fontScalerIndicator
        visible: false
        qss: "stat-scaler-ind"
    }

    DapQmlLabel {
        id: fontScalerSubLabel
        visible: false
        qss: "stat-scaler-sub"
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        text: "Statistics"
        qss: "dialog-title"
        hideClose: true
    }

    /****************************************//**
     * Graph
     ********************************************/

    QmlSchedulePainter {
        id: schedulePainter
        objectName: "SchedulePainter"
        DapQmlStyle { item: schedulePainter; qss: "stat-graph"; }
    }

    /****************************************//**
     * Font resize event
     ********************************************/

    Timer {
        interval: 500
        running: true
        repeat: false
        onTriggered: {
            resizeFont(stDownSpeed);
            resizeFont(stUpSpeed);
            resizeFont(stBytesRec);
            resizeFont(stBytesSent);
            resizeFont(stPacketsRec);
            resizeFont(stPacketsSent);
            resizeFont(stUptime);
            resizeFont(stPing);
            moveColumns();
        }
    }

    /****************************************//**
     * Speed
     ********************************************/

    DapQmlButton {
        id: stDownSpeed
        x: leftColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "10 205 Mbps"
        subText: "DOWNLOAD SPEED"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-speed"

        onWidthChanged: resizeFont(stDownSpeed);
        onHeightChanged: resizeFont(stDownSpeed);
    }

    DapQmlButton {
        id: stUpSpeed
        x: rightColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "20 896 Mbps"
        subText: "UPLOAD SPEED"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-speed"

        onWidthChanged: resizeFont(stUpSpeed);
        onHeightChanged: resizeFont(stUpSpeed);
    }

    /****************************************//**
     * Bytes
     ********************************************/

    DapQmlButton {
        id: stBytesRec
        x: leftColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "29 896 Gb"
        subText: "BYTES RECEIVED"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-bytes"

        onWidthChanged: resizeFont(stBytesRec);
        onHeightChanged: resizeFont(stBytesRec);
   }

    DapQmlButton {
        id: stBytesSent
        x: rightColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "20 896 Mb"
        subText: "BYTES SENT"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-bytes"

        onWidthChanged: resizeFont(stBytesSent);
        onHeightChanged: resizeFont(stBytesSent);
    }

    /****************************************//**
     * Packets
     ********************************************/

    DapQmlButton {
        id: stPacketsRec
        x: leftColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "454 086"
        subText: "PACKETS RECEIVED"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-packets"

        onWidthChanged: resizeFont(stPacketsRec);
        onHeightChanged: resizeFont(stPacketsRec);
    }

    DapQmlButton {
        id: stPacketsSent
        x: rightColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "4 086"
        subText: "PACKETS SENT"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-packets"

        onWidthChanged: resizeFont(stPacketsSent);
        onHeightChanged: resizeFont(stPacketsSent);
    }

    /****************************************//**
     * Misc
     ********************************************/

    DapQmlButton {
        id: stUptime
        x: leftColumnPos

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "146:7:27"
        subText: "UPTIME"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-misc"

        onWidthChanged: resizeFont(stUptime);
        onHeightChanged: resizeFont(stUptime);
    }

    DapQmlButton {
        id: stPing
        x: rightColumnPos
        visible: false

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "34 ms"
        subText: "PING"
        mainQss: "stat-item-main"
        subQss: "stat-item-sub"
        qss: "stat-misc"

        onWidthChanged: resizeFont(stPing);
        onHeightChanged: resizeFont(stPing);
    }

}

/*-----------------------------------------*/
