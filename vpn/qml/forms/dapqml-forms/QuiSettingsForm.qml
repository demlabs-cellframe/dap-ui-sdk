import QtQuick 2.4
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {

    Rectangle {
        anchors.fill: parent
        color: "#F7F8FA"

        GridLayout {
            width: 400
            columns: 1

            DapQmlButton {
                width: 350
                height: 108

                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: width
                Layout.preferredHeight: height

                buttonStyle: DapQmlButton.Style.TopMainBottomSub
                mainText: "Test item 1"
                subText: "TopMainBottomSub"
            }

            DapQmlButton {
                width: 350
                height: 108

                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: width
                Layout.preferredHeight: height

                buttonStyle: DapQmlButton.Style.TopSubBottomMain
                mainText: "Test item 2"
                subText: "TopSubBottomMain"
            }

            DapQmlButton {
                width: 350
                height: 108

                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: width
                Layout.preferredHeight: height

                buttonStyle: DapQmlButton.Style.LeftTopMainBottomSub
                leftText: "$ Third"
                mainText: "Test item 3"
                subText: "TopSubBottomMain"
            }

            DapQmlSeparator {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: width
                Layout.preferredHeight: height
            }

            DapQmlPushButton {
                id: btnConnect
                checkable: true
                checked: true
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: width
                Layout.preferredHeight: height
                text: qsTr("CONNECT")
            }

        }
    }
}
