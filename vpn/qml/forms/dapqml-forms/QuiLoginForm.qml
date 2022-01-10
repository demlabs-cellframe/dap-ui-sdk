import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property int calcWidth: centerWidth()

    /* defs */
    enum CoordType
    {
        CTX,
        CTY
    }

    /* functions */
    function centerHor(item) {
        return root.width / 2 - item.width / 2;
    }
    function centerVer(item) {
        return root.height / 2 - item.height / 2;
    }

    function centerWidth() {
        return root.width - 78;
    }
    function adjustedCoord(type,coord,item) {
        var multH = root.width / 428;
        var multV = root.height / 926;
        switch(type) {
        case QuiLoginForm.CTX: return (coord * multH) - (item.width / 2);
        case QuiLoginForm.CTY: return (coord * multV);
        }
        return 0;
    }
    function adjustedSize(type,size,item) {
        var multH = root.width / 428;
        var multV = root.height / 926;
        switch(type) {
        case QuiLoginForm.CTX: return (size * multH);
        case QuiLoginForm.CTY: return (size * multV);
        }
        return size;
    }

    /* W I P */
    Timer {
        interval: 500
        running: true
        repeat: false
        onTriggered: {
            calcWidth                   = centerWidth();
            btnChooseServer.separator   = false;
            btnEnterSerial.separator    = false;
            btnChooseServer.separator   = true;
            btnEnterSerial.separator    = true;
        }
    }

    /* logo */
    Image {
        x: adjustedCoord(QuiLoginForm.CTX, 428 / 2, this) // centerHor(this)
        y: adjustedCoord(QuiLoginForm.CTY, 117, this) // 117
        z: 15
        width: 260
        height: 89
        source: "qrc:/light/login-logo.png"
    }

    /* top separator */
    DapQmlSeparator {
        x: adjustedCoord(QuiLoginForm.CTX, 428 / 2, this) // centerHor(this)
        y: adjustedCoord(QuiLoginForm.CTY, 291, this) // 291
        z: 15
        width: root.calcWidth
    }

    /* choose server */
    DapQmlButton {
        id: btnChooseServer
        x: adjustedCoord(QuiLoginForm.CTX, 428 / 2, this) // centerHor(this)
        y: adjustedCoord(QuiLoginForm.CTY, 292, this) // 292
        z: 15
        width: root.calcWidth
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
        id: btnEnterSerial
        x: adjustedCoord(QuiLoginForm.CTX, 428 / 2, this) // centerHor(this)
        y: adjustedCoord(QuiLoginForm.CTY, 406, this) // 406
        z: 15
        width: root.calcWidth
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
        x: adjustedCoord(QuiLoginForm.CTX, 428 / 2, this) // centerHor(this)
        y: adjustedCoord(QuiLoginForm.CTY, 570, this) // 570
        z: 15

        text: qsTr("CONNECT")
    }

    /* obtain key */
    RowLayout {
        x: adjustedCoord(QuiLoginForm.CTX, 428 / 2, this) // centerHor(this)
        y: adjustedCoord(QuiLoginForm.CTY, 675, this) // 675
        z: 15
        width: root.calcWidth
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
