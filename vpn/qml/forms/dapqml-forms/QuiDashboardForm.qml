import QtQuick 2.0
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root

    /* title */
    DapQmlLabel {
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

    /* switch */
    DapQmlSwitch {
        qss: "dashboard-switch"
    }

    /* speed */
    RowLayout {
        id: speedContainer

        DapQmlButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            mainText: "120 Mb/s"
            subText: "DOWNLOAD"
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlButton {
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
    }
}
