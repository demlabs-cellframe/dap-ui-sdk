import QtQuick 2.0
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "Dashboard"

    /* signals */
    signal sigSwitchToggle (bool checked);
    signal sigServerClicked();
    signal sigConnectionStatusChangeRequested();

    /* functions */
    function setStatusText(a_text) {
        statusLabel.text    = a_text;
    }

    function setButtonState(a_checked) {
        dashboardSwitch.checked = a_checked;
    }

    function setErrorText(a_text) {
        dashboardError  = a_text;
    }

    function setStatusIndicator(a_status) {
        if (a_status)
            indicator.qss = "dashboard-status-icon ic_online";
        else
            indicator.qss = "dashboard-status-icon ic_offline";
    }

    function setTimeText(a_text) {
        statusTime.text = a_text;
    }

    function updateSpeedIndicators(a_downSpeed, a_upSpeed) {
        speedDown.mainText  = a_downSpeed;
        speedUp.mainText    = a_upSpeed;
    }

    function setServerInfo(a_name, a_ip) {
        serverChoose.mainText   = a_name;
        serverChoose.subText    = a_ip;
    }

    /* title */
    DapQmlLabel {
        id: statusLabel
//        x: centerHor(this)
//        y: 62
//        width: 340
//        height: 28
        text: "VPN Connected"
        qss: "dashboard-title"
    }

    /* status */
    DapQmlRectangle {
        id: statusContainer
        qss: "dashboard-status-container"

        DapQmlLabel {
            id: indicator
            x: statusTime.x - width * 2
            y: (statusContainer.height - height) / 2
            horizontalAlign: Text.AlignRight
            qss: "dashboard-status-icon ic_online"
        }

        DapQmlLabel {
            id: statusTime
            x: (parent.width - width) / 2
            horizontalAlign: Text.AlignLeft
            text: "56:30:10"
            qss: "dashboard-status-label"
        }
    }

    /* error label */
    DapQmlLabel {
        id: dashboardError
        qss: "dashboard-error-label"
        wrapMode: Text.WordWrap
        text: "Temporary network problems, request will be handled as soon as the network connection is re-established"
    }

    /* switch */
    DapQmlSwitch {
        id: dashboardSwitch
        qss: "dashboard-switch"
        onClicked: { root.sigSwitchToggle(checked); root.sigConnectionStatusChangeRequested(); }
    }

    /* speed */
    RowLayout {
        id: speedContainer

        DapQmlButton {
            id: speedDown
            Layout.fillWidth: true
            Layout.fillHeight: true
            mainText: "120 Mb/s"
            subText: "DOWNLOAD"
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlButton {
            id: speedUp
            Layout.fillWidth: true
            Layout.fillHeight: true
            mainText: "34.7 Mb/s"
            subText: "UPLOAD"
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlStyle { item: speedContainer; qss: "dashboard-speed-container" }
    }

    /* change server button */
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
