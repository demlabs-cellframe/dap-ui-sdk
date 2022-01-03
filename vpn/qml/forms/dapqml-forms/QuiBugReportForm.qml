import QtQuick 2.4
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {
    id: root

    signal close()

    GridLayout {
        anchors.fill: parent
        columns: 1

        /* title */
        Item {
            Layout.fillWidth: true

            Image {
                id: btnClose
                width: 28
                height: 28
                source: "qrc:/light/ic_close.png"

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.close()
                }
            }

            DapQmlLabel {
                text: "Bug report"
                width: parent.width
            }
        }

        /* text edit */
        Item {
            Layout.fillWidth: true
            Image {
                width: 390
                height: 200
                source: "qrc:/dark/report_bg.png"
            }
        }

        /* letter counter */
        DapQmlLabel {
            Layout.fillWidth: true
            text: "0/200"
            color: "#A4A3C0"
            horizontalAlign: Text.AlignRight
        }

        /* attach */
        DapQmlLabel {
            Layout.fillWidth: true
            text: "Click here to attach a screenshot"
            color: "#DA0B82"
        }

        /* send button */
        DapQmlPushButton {
            text: "SEND REPORT"
        }
    }
}
