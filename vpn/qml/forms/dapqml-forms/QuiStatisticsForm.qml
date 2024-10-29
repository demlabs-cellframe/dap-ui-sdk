/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"
import QmlSchedulePainter 1.0
import DapQmlStyle 1.0

/****************************************//**
 * @brief Statistics Form
 * @ingroup groupDapQmlForms
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
    property string formName: "Statistics"

    property QtObject internal: QtObject {
        property string bytesRec:       "0"
        property string bytesSent:      "0"
        property string packetsRec:     "0"
        property string packetsSend:    "0"
        property string downSpeed:      "0"
        property string upSpeed:        "0"
        property string upTime:         "0"
    }

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
        root.internal.bytesRec      = a_bytesReceived;
        root.internal.bytesSent     = a_bytesSent;
        root.internal.packetsRec    = a_packetsReceived;
        root.internal.packetsSend   = a_packetsSent;
        root.internal.downSpeed     = a_downloadSpeedString;
        root.internal.upSpeed       = a_downloadSpeedString;
    }

    /// @brief change uptime label text
    function setUptime (a_text) {
        root.internal.upTime        = a_text;
    }

    /// @}
    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: compStatButton

        //property string topText
        //property string bottomText

        ColumnLayout {
            Layout.minimumWidth: container.width / 2
            height: scalerButtonFrame.height

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: parent.parent.topText
                color: fontScalerIndicator.color
                font.pixelSize: fontScalerIndicator.fontSize
                font.weight: fontScalerIndicator.fontWeight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
            }

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: parent.parent.bottomText
                color: fontScalerSubLabel.color
                font.pixelSize: fontScalerSubLabel.fontSize
                font.weight: fontScalerSubLabel.fontWeight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
            }
        }
    }

    Component {
        id: compStatButtonRow

//        property string leftTopText
//        property string leftBottomText

//        property string rightTopText
//        property string rightBottomText

        RowLayout {
            width: root.width
            height: scalerButtonFrame.height

            Loader {
                Layout.minimumWidth: container.width / 2
                Layout.fillHeight: true
                sourceComponent: compStatButton
                property string topText:    parent.parent.leftTopText
                property string bottomText: parent.parent.leftBottomText
            }

            Loader {
                Layout.minimumWidth: container.width / 2
                Layout.fillHeight: true
                sourceComponent: compStatButton
                property string topText:    parent.parent.rightTopText
                property string bottomText: parent.parent.rightBottomText
            }
        }
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlDummy {
        id: scalerButtonFrame
        visible: false
        qss: "stat-speed"
    }

    DapQmlDummy {
        id: fontScalerIndicator
        visible: false
        qss: "stat-scaler-ind stat-item-main"
        property string color
        property int fontSize
        property int fontWeight: Font.DemiBold
    }

    DapQmlDummy {
        id: fontScalerSubLabel
        visible: false
        qss: "stat-scaler-sub stat-item-sub"
        property string color
        property int fontSize
        property int fontWeight: Font.Normal
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        text: qsTr("Statistics") + lang.notifier
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
     * Indicators
     ********************************************/

    ColumnLayout {
        id: container
        width: parent.width
        DapQmlStyle { item: container; qss: "stat-indicators-container" }

        Loader {
            Layout.minimumWidth: container.width / 2
            Layout.fillHeight: true
            sourceComponent: compStatButtonRow
            property string leftTopText:        root.internal.downSpeed
            property string leftBottomText:     qsTr ("DOWNLOAD SPEED") + lang.notifier
            property string rightTopText:       root.internal.upSpeed
            property string rightBottomText:    qsTr ("UPLOAD SPEED") + lang.notifier
        }

        Loader {
            Layout.minimumWidth: container.width / 2
            Layout.fillHeight: true
            sourceComponent: compStatButtonRow
            property string leftTopText:        root.internal.bytesRec
            property string leftBottomText:     qsTr ("BYTES RECEIVED") + lang.notifier
            property string rightTopText:       root.internal.bytesSent
            property string rightBottomText:    qsTr ("BYTES SENT") + lang.notifier
        }

        Loader {
            Layout.minimumWidth: container.width / 2
            Layout.fillHeight: true
            sourceComponent: compStatButtonRow
            property string leftTopText:        root.internal.packetsRec
            property string leftBottomText:     qsTr ("PAKCETS RECEIVED") + lang.notifier
            property string rightTopText:       root.internal.packetsSend
            property string rightBottomText:    qsTr ("PACKETS SENT") + lang.notifier
        }

        Loader {
            Layout.minimumWidth: container.width / 2
            Layout.fillHeight: true
            sourceComponent: compStatButtonRow
            property string leftTopText:        root.internal.upTime
            property string leftBottomText:     qsTr ("UPTIME") + lang.notifier
            property string rightTopText
            property string rightBottomText
        }
    }
}

/*-----------------------------------------*/
