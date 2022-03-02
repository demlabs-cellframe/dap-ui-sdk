import QtQuick 2.4
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {
    id: root
    /* vars */
    property string formName: "Statistics"

    /* functions */
    function updateIndicators(a_bytesReceived, a_bytesSent,
                              a_packetsReceived, a_packetsSent,
                              a_uploadSpeed, a_downloadSpeed,
                              a_uploadSpeedString, a_downloadSpeedString)
    {
        stBytesRec.mainText;
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        GridLayout {
            anchors.fill: parent
            columns: 1
            Layout.alignment: Qt.AlignHCenter

            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignVCenter
                height: 28+64
                text: "Statistics"
                qss: "stat-title"
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                width: 360
                height: 162
                color: "gray"
            }

            GridLayout {
                columns: 2
                Layout.alignment: Qt.AlignHCenter

                DapQmlButton {
                    id: stDownSpeed
                    width: 154
                    height: 99

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "10 205 Mbps"
                    subText: "DOWNLOAD SPEED"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stUpSpeed
                    width: 154
                    height: 99

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "20 896 Mbps"
                    subText: "UPLOAD SPEED"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stBytesRec
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "29 896 Gb"
                    subText: "BYTES RECEIVED"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stBytesSent
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "20 896 Mb"
                    subText: "BYTES SENT"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stPacketsRec
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "454 086"
                    subText: "PACKETS RECEIVED"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stPacketsSent
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "4 086"
                    subText: "PACKETS SENT"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stUptime
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "146:7:27"
                    subText: "UPTIME"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

                DapQmlButton {
                    id: stPing
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "34 ms"
                    subText: "PING"
                    mainQss: "stat-item-main"
                    subQss: "stat-item-sub"
                }

            }
        }
    }
}
