import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import "qrc:/dapqml-widgets"

Item {
    id: root

    function centerHor(item) {
        return root.width / 2 - item.width / 2;
    }

    function centerWidth() {
        return root.width - 78;
    }

    /* background */
    Rectangle {
        anchors.fill: parent
        z: 10
        color: "#F7F8FA"
    }

    /* logo */
    Image {
        x: centerHor(this)
        y: 117
        z: 15
        width: 260
        height: 89
        source: "qrc:/light/login-logo.png"
    }

    /* top separator */
    DapQmlSeparator {
        x: centerHor(this)
        y: 291
        z: 15
        width: centerWidth()
    }

    /* choose server */
    DapQmlButton {
        x: centerHor(this)
        y: 292
        z: 15
        width: centerWidth()
        height: 108

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "Auto select"
        subText: "CHOOSING SERVER"
        qss: "login-btn"
        mainQss: "login-btn-main"
        subQss: "login-btn-sub"
        separator: true
    }

    /* enter serial */
    DapQmlButton {
        x: centerHor(this)
        y: 406
        z: 15
        width: centerWidth()
        height: 108

        buttonStyle: DapQmlButton.Style.TopMainBottomSub
        mainText: "____ ____ ____ ____"
        subText: "SERIAL KEY"
        qss: "login-btn"
        mainQss: "login-btn-main"
        subQss: "login-btn-sub"
        separator: true
    }

    /* connect */
    DapQmlPushButton {
        id: btnConnect
        x: centerHor(this)
        y: 570
        z: 15

        text: qsTr("CONNECT")
    }

    /* obtain key */
    RowLayout {
        x: centerHor(this)
        y: 675
        z: 15
        height: 19
        Text {
            Layout.fillWidth: true
            text: "Don't have a serial key?"
            color: "#5C5B74"
            font.family: "Lato"
            font.pixelSize: 16
            font.weight: Font.Normal
        }

        Button {
            Layout.fillWidth: true
            background: Rectangle { color: "transparent" }

            contentItem: Text {
                text: "Tap here to obtain one"
                color: "#DA0B82"
                font.family: "Lato"
                font.pixelSize: 16
                font.weight: Font.Normal

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    enabled: false
                }
            }
        }
    }
}
