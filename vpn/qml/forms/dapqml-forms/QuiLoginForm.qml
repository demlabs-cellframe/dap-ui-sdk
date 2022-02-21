import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import StyleDebugTree 1.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "Login"

    /* signals */
    signal sigChooseServer();
    signal sigChooseSerial();
    signal sigConnect();
    signal sigObtainNewKey();

    signal textEditedAndCleaned();
    signal textEditedAndFilledOut (string serial);

    signal textChangedAndCleaned();
    signal textChangedAndFilledOut (string serial);

    signal sigSerialFillingIncorrect();

    /* functions */
    function setStatusMessage(a_message) {
        loginErrorLabel.text    = a_message;
    }

    function setSerial(a_serial) {
        btnEnterSerial.mainText = a_serial;
    }

    function setConnectionEnabled(a_value) {
        btnConnect.enabled  = a_value;
    }

    function setKeyEnterEnabled(a_value) {
        btnEnterSerial.visible  = a_value;
    }

    /* W I P */
    Timer {
        interval: 500
        running: true
        repeat: false
        onTriggered: {
//            calcWidth                   = centerWidth();
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

    /* error label */
    DapQmlLabel {
        id: loginErrorLabel
        qss: "login-error-label"
        wrapMode: Text.WordWrap
        text: "Temporary network problems, request will be handled as soon as the network connection is re-established"
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
            onClicked: root.sigChooseServer()
        }
    }

    /* enter serial */
    DapQmlRectangle {
        qss: "login-btn-serial-container"
        DapQmlButton {
            id: btnEnterSerial
            objectName: "btnEnterSerial"
            property int maxCountChar: 16
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            //mainText: "____ ____ ____ ____"
            subText: "SERIAL KEY"
            qss: "login-btn-serial"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            inputMask: ">NNNN-NNNN-NNNN-NNNN;_"
            separator: true

            onClicked: root.sigChooseSerial()
            onTextChanged: {
                var text    = mainText;

                if (text.length == maxCountChar)
                    root.textChangedAndFilledOut (mainText);
                else if (text.length == 0)
                    root.textChangedAndCleaned();
                else
                    root.sigSerialFillingIncorrect();
            }
            onTextEdited: {
                var text    = mainText;

                if (text.length == maxCountChar)
                    root.textEditedAndFilledOut (mainText);
                else if (text.length == 0)
                    root.textEditedAndCleaned();
                else
                    root.sigSerialFillingIncorrect();
            }
        }
    }

    /* connect */
    DapQmlRectangle {
        qss: "login-connect-container"
        DapQmlPushButton {
            id: btnConnect
            enabled: false
            qss: "login-connect"
            x: (parent.width - width) / 2
            z: 15

            text: qsTr("CONNECT")
            onClicked: root.sigConnect()
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
            onClicked: root.sigObtainNewKey()
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
