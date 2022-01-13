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
        var multV = root.height / (926 - 172);
        switch(type) {
        case QuiLoginForm.CTX: return (coord * multH) - (item.width / 2);
        case QuiLoginForm.CTY: return (coord * multV);
        }
        return 0;
    }
    function adjustedSize(type,size,item) {
        var multH = root.width / 428;
        var multV = root.height / (926 - 172);
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
    DapQmlRectangle {
        qss: "login-logo-container"
        DapQmlLabel {
            x: (parent.width - width) / 2
            z: 15
            qss: "login-logo"
        }
    }

    /* top separator */
    DapQmlRectangle {
        qss: "login-separator-container"
        DapQmlSeparator {
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            qss: "login-separator"
        }
    }

    /* choose server */
    DapQmlRectangle {
        qss: "login-btn-server-container"
        DapQmlButton {
            id: btnChooseServer
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: "Auto select"
            subText: "CHOOSING SERVER"
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
        }
    }

    /* enter serial */
    DapQmlRectangle {
        qss: "login-btn-serial-container"
        DapQmlButton {
            id: btnEnterSerial
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: "____ ____ ____ ____"
            subText: "SERIAL KEY"
            qss: "login-btn-serial"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
        }
    }

    /* connect */
    DapQmlRectangle {
        qss: "login-connect-container"
        DapQmlPushButton {
            id: btnConnect
            qss: "login-connect"
            x: (parent.width - width) / 2
            z: 15

            text: qsTr("CONNECT")
        }
    }

    /* obtain key */
    DapQmlRectangle {
        qss: "login-obtain-container"

        DapQmlLabel {
            id: obtainLabel
            text: "Don't have a serial key?"
            color: "#5C5B74"
            width: parent.width / 2 - 2
            height: parent.height
            horizontalAlign: Text.AlignRight
            qss: "login-obtain-font"
//          font.family: "Lato"
//          font.pixelSize: 16
//          font.weight: Font.Normal
        }

        DapQmlLabel {
            id: obtainLinkLabel
            x: parent.width / 2 + 2
            text: "Tap here to obtain one"
            color: "#DA0B82"
            width: parent.width / 2
            height: parent.height
            horizontalAlign: Text.AlignLeft
            qss: "login-obtain-font"
//          font.family: "Lato"
//          font.pixelSize: 16
//          font.weight: Font.Normal

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                enabled: false
            }
        }
    }
}
