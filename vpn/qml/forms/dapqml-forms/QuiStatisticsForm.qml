import QtQuick 2.4
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {

    Rectangle {
        anchors.fill: parent
        color: "#F7F8FA"

        GridLayout {
            anchors.fill: parent
            columns: 1
            Layout.alignment: Qt.AlignHCenter

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Statistics"
                color: "#04004E"
                font.family: "Lato"
                font.pixelSize: 24
                font.weight: Font.DemiBold
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
                    width: 154
                    height: 99

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "10 205 Mbps"
                    subText: "DOWNLOAD SPEED"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "20 896 Mbps"
                    subText: "UPLOAD SPEED"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "29 896 Gb"
                    subText: "BYTES RECEIVED"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "20 896 Mb"
                    subText: "BYTES SENT"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "454 086"
                    subText: "PACKETS RECEIVED"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "4 086"
                    subText: "PACKETS SENT"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "146:7:27"
                    subText: "UPTIME"
                    separator: false
                }

                DapQmlButton {
                    width: 154
                    height: 99
                    
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    mainText: "34 ms"
                    subText: "PING"
                    separator: false
                }

            }
        }
    }
}
