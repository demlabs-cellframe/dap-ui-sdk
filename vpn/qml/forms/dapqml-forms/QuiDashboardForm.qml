import QtQuick 2.0
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {
    id: root

    function centerHor(item) {
        return root.width / 2 - item.width / 2;
    }

    /* title */
    DapQmlLabel {
        x: centerHor(this)
        y: 62
        width: 340
        height: 28
        text: "VPN Connected"
        qss: "dashboard-title"
    }

    /* status */
    RowLayout {
        x: centerHor(this)
        y: 107
        width: 108
        height: 22

        DapQmlLabel {
            Layout.fillWidth: false
            Layout.preferredWidth: 5
            Layout.preferredHeight: 5
            id: indicator
            qss: "ic_online"
        }

        DapQmlLabel {
            Layout.fillWidth: false
            Layout.preferredWidth: 86
            Layout.preferredHeight: 22
            id: statusTime
            text: "56:30:10"
            qss: "dashboard-status"
        }
    }

    /* switch */
    DapQmlSwitch {
        x: root.width / 2 - 270 / 2;
        y: 220
        width: 270
        height: 174
    }

    /* speed */
    RowLayout {
        x: centerHor(this)
        y: 471
        width: root.width - 88
        height: 99

        DapQmlButton {
            Layout.fillWidth: true
            Layout.preferredWidth: 154
            Layout.preferredHeight: 99
            mainText: "120 Mb/s"
            subText: "DOWNLOAD"
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }

        DapQmlButton {
            Layout.fillWidth: true
            Layout.preferredWidth: 154
            Layout.preferredHeight: 99
            mainText: "34.7 Mb/s"
            subText: "UPLOAD"
            mainQss: "dashboard-speed-main"
            subQss: "dashboard-speed-sub"
        }
    }

    /* change server button */
    DapQmlButton {
        x: centerHor(this)
        y: 575
        width: 410
        height: 137
        link: true
        frame: true

        buttonStyle: DapQmlButton.TopSubBottomMain
        mainText: "North America - Canada"
        subText: "231:532:531:20"
        mainQss: "dashboard-server-main"
        subQss: "dashboard-server-sub"
    }
}
