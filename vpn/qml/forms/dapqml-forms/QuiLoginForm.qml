/* INCLUDES */

import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlSerialKeyInput 1.0
import StyleDebugTree 1.0
import DapQmlStyle 1.0
import Brand 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Login Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * - Logo
 * - Error label
 * - Choose server
 * - Enter serial
 * - Connect button
 * - Obtain key link
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum Mode
    {
        M_SERIAL,
        M_PASSWORD
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "Login"

    /// @brief internal variables
    property QtObject internal: QtObject {

        /// @brief login mode
        property int mode: QuiLoginForm.Mode.M_SERIAL

        /// @brief show password contents
        property bool showPassword: false

        function forgotLabel() {
            return mode === QuiLoginForm.Mode.M_SERIAL
                ? "Don't have a serial key?"
                : "Forgot your password?"
        }

        function tapHereLabel() {
            return mode === QuiLoginForm.Mode.M_SERIAL
                ? "Tap here to obtain one"
                : "Tap here to recover"
        }
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief choose server button clicked
    signal sigChooseServer();

    /// @brief enter serial key clicked
    signal sigChooseSerial();

    /// @brief connect button clicked
    signal sigConnect();

    /// @brief buy serial clicked
    signal sigObtainNewKey();

    /// @brief start recovering password
    signal sigRecoverPassword();

    /// @brief entered serial is incorerct
    signal sigSerialFillingIncorrect();

    signal textEditedAndCleaned();
    signal textEditedAndFilledOut (string serial);

    signal textChangedAndCleaned();
    signal textChangedAndFilledOut (string serial);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change error label text
    function setStatusMessage(a_message) {
        loginErrorLabel.text    = a_message;
    }

    /// @brief change serial key button content
    function setSerial(a_serial) {
        btnEnterSerial.mainText = a_serial;
    }

    /// @brief enable\\disable connect button
    function setConnectionEnabled(a_value) {
        btnConnect.enabled  = a_value;
    }

    /// @brief show\\hide serial enter button
    function setKeyEnterEnabled(a_value) {
        btnEnterSerial.visible  = a_value;
    }

    /// @brief change current chosen server name
    function setServer(a_name) {
        btnChooseServer.mainText    = a_name;
    }

    /// @brief set input mask for serial input
    function setupInputMask() {
        //btnEnterSerial.inputMask    = ">NNNN-NNNN-NNNN-NNNN;_"
    }

    /// @}
    /****************************************//**
     * Separator fix
     ********************************************/

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

    /****************************************//**
     * Logo
     ********************************************/

    DapQmlRectangle {
        qss: "login-logo-container"
        DapQmlLabel {
            x: (parent.width - width) / 2
            z: 15
            qss: "login-logo"
        }
    }

//    TextField {
//        x: (parent.width - width) / 2
//        y: 15
//        width: parent.width - 74
//        height: 64
//        color: "#333333"
//        inputMethodHints: Qt.ImhSensitiveData

//        DapQmlSerialKeyInput {
//            //anchors.fill: parent
//            id: filter
//            objectName: "serialInputFilter"
//        }

//        Component.onCompleted: filter.setup(this)
//    }

    /****************************************//**
     * Error label
     ********************************************/

    DapQmlLabel {
        id: loginErrorLabel
        qss: "login-error-label"
        wrapMode: Text.WordWrap
        //text: "Temporary network problems, request will be handled as soon as the network connection is re-established"
    }

    /****************************************//**
     * Login type select
     ********************************************/

//    Component.onCompleted: StyleDebugTree.describe (
//       "login",
//        ["x", "y", "width", "height"],
//       this);

    RowLayout {
        id: loginTypeContainer
        spacing: height / 3
        clip: true
        visible: Brand.name() === "RiseVPN"
        DapQmlStyle { item: loginTypeContainer; qss: "login-type-container" }

//        Component.onCompleted: StyleDebugTree.describe (
//           "loginTypeContainer",
//            ["x", "y", "width", "height"],
//           this);

        DapQmlRadioButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: "With password"
            textPadding: indicator.width * 0.8

            onCheckedChanged: {
                if (checked)
                    internal.mode   = QuiLoginForm.Mode.M_PASSWORD;
            }

//            Component.onCompleted: StyleDebugTree.describe (
//               "With password",
//                ["x", "y", "width", "height"],
//               this);
        }

        DapQmlRadioButton {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: "With serial"
            textPadding: indicator.width * 0.8
            checked: true

            onCheckedChanged: {
                if (checked)
                    internal.mode   = QuiLoginForm.Mode.M_SERIAL;
            }

//            Component.onCompleted: StyleDebugTree.describe (
//               "With serial",
//                ["x", "y", "width", "height"],
//               this);
        }
    }

    /****************************************//**
     * Top separator
     ********************************************/

    DapQmlRectangle {
        qss: "login-separator-container"
        DapQmlSeparator {
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            qss: "login-separator"
        }
    }

    /****************************************//**
     * Choose server
     ********************************************/

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
            link: true
            onClicked: root.sigChooseServer()
        }
    }

    /****************************************//**
     * Enter serial
     ********************************************/

    DapQmlRectangle {
        qss: "login-btn-serial-container"
        visible: internal.mode === QuiLoginForm.Mode.M_SERIAL

        DapQmlButton {
            id: btnEnterSerial
            objectName: "btnEnterSerial"
            property int maxCountChar: 19
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            //height: parent.height

//            Rectangle {
//                color: "gray"
//                anchors.fill: parent
//            }

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            mainText: ""
            subText: "SERIAL KEY"
            qss: "login-btn-serial"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            placeHolderText: "____ ____ ____ ____"
            placeHolderQss: "login-btn-main"
            //inputMask: ">NNNN-NNNN-NNNN-NNNN;_"
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

    /****************************************//**
     * Enter e-mail & password
     ********************************************/

    DapQmlRectangle {
        qss: "login-btn-email-container"
        visible: internal.mode === QuiLoginForm.Mode.M_PASSWORD

        DapQmlButton {
            id: btnEnterEmail
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            mainText: ""
            subText: "EMAIL"
            qss: "login-btn-email"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
        }
    }

    DapQmlRectangle {
        qss: "login-btn-password-container"
        visible: internal.mode === QuiLoginForm.Mode.M_PASSWORD

        DapQmlButton {
            id: btnEnterPassword
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            mainText: ""
            subText: "PASSWORD"
            qss: "login-btn-password"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            editEchoMode: (internal.showPassword)
                          ? TextInput.Normal
                          : TextInput.Password
            separator: true
        }

        Button {
            id: checkShowPassword
            checkable: true
            checked: false
            icon {
                source: (internal.showPassword)
                        ? "qrc:/light/password-show.png"
                        : "qrc:/light/password-hide.png"
                color: "transparent"
                width: checkShowPassword.width
                height: checkShowPassword.height
            }
            background: Rectangle { color: "transparent" }
            x: parent.width - width - (74 / 2)
            y: (parent.height / 2 - height) / 2 + height / 12
            z: 16
            width: parent.height * 0.5
            height: parent.height * 0.5

            onCheckedChanged: internal.showPassword = checked
        }
    }

    /****************************************//**
     * Connect button
     ********************************************/

    DapQmlPushButton {
        id: btnConnect
        x: (parent.width - width) / 2
        z: 15
        qss: "login-connect"

        text: qsTr("CONNECT")
        onClicked: root.sigConnect()
    }

    /****************************************//**
     * Obtain key link
     ********************************************/

    DapQmlRectangle {
        qss: "login-obtain-container"

        DapQmlLabel {
            id: obtainLabel
            text: internal.forgotLabel()
            color: "#5C5B74"
            width: parent.width / 2 - 2
            height: parent.height
            horizontalAlign: Text.AlignRight
            qss: "login-obtain-font c-label"
            mipmap: false
//          font.family: "Lato"
//          font.pixelSize: 16
//          font.weight: Font.Normal
        }

        DapQmlLabel {
            id: obtainLinkLabel
            x: parent.width / 2 + 2
            text: internal.tapHereLabel()
            color: "#DA0B82"
            width: parent.width / 2
            height: parent.height
            horizontalAlign: Text.AlignLeft
            qss: "login-obtain-font c-brand"
            onClicked: (internal.mode === QuiLoginForm.Mode.M_SERIAL)
                       ? root.sigObtainNewKey()
                       : root.sigRecoverPassword()
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

/*-----------------------------------------*/
