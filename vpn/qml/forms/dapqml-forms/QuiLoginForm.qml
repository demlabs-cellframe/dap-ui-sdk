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
    clip: true

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum Mode
    {
        M_SERIAL,
        M_PASSWORD,
        M_CERT
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

        property bool changedServer: false
        property bool changedCert:   false
        property string serverName: ""
        property string certName:   ""
        property string tickerMessage:   ""
        property string tickerUrl:   ""

        /// @brief login mode
        property int mode: QuiLoginForm.Mode.M_SERIAL

        /// @brief show password contents
        property bool showPassword: false

        function forgotLabel() {
//   First variant for Rise
//            return mode === QuiLoginForm.Mode.M_SERIAL
//                ? qsTr("Don't have a serial key?")
//                : qsTr("Forgot your password?")
            return mode === QuiLoginForm.Mode.M_SERIAL
                ? qsTr("Don't have a serial key?")
                : qsTr("")

        }

        function tapHereLabel() {
//   First variant for Rise
//            return mode === QuiLoginForm.Mode.M_SERIAL
//                ? qsTr("Tap here to obtain one")
//                : qsTr("Tap here to recover")
            return Brand.name() !== "RiseVPN"
                ? qsTr("Tap here to obtain one")
                : qsTr("Tap here to show cdb management")

        }
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief choose server button clicked
    signal sigChooseServer();

    /// @brief choose certificate button clicked
    signal sigChooseCert();

    /// @brief enter serial key clicked
    signal sigChooseSerial();

    /// @brief connection by serial requested
    signal sigConnectBySerial();

    /// @brief connection by login & password requested
    signal sigConnectByPassword();

    /// @brief connection by certificate requested
    signal sigConnectByCert();

    /// @brief buy serial clicked
    signal sigObtainNewKey();

    /// @brief start recovering password
    signal sigRecoverPassword();

    /// @brief entered serial is incorerct
    signal sigSerialFillingIncorrect();

    /// @brief show server manager
    signal sigShowCdbManager();

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
        internal.changedServer      = true;
        internal.serverName         = a_name;
        //btnChooseServer.mainText    = a_name;
        btnChooseServer.updateServerName();
    }

    function setTickerMessage(a_message, a_url) {
        console.log(a_message)
        internal.tickerMessage      = a_message;
        internal.tickerUrl         = a_url;
        tickerLabel.updateTickerMessage()
    }

    /// @brief set input mask for serial input
    function setupInputMask() {
        //btnEnterSerial.inputMask    = ">NNNN-NNNN-NNNN-NNNN;_"
    }

    function tickerClicked() {
        tickerLabel.text = "open url open url open url open url open url open url "

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
        * Ticker
        ********************************************/

       DapQmlRectangle {
           id: ticker
           qss: "ticker"
           width: root.width
           visible: false

           DapQmlRectangle {
               id: tickerLableRect
               qss: "ticker_lable_rect"
               visible: true
               anchors.left: parent.left

               DapQmlLabel {
                   id: tickerLabel
                   qss: "ticker_label"
                   text: internal.tickerMessage
                   z: 2
                   horizontalAlign: Text.AlignHCenter
                   mipmap: false


                   NumberAnimation on x {
                       id: tickerAnimation
                       from: parent.width
                       to: -1*(parent.width + tickerLabel.width)
                       duration: 10000
                       loops: Animation.Infinite
                       running: true
                   }

                   function updateTickerMessage() {
                       text = internal.tickerMessage
                       ticker.visible = true
                       tickerAnimation.to = -1*(parent.width + tickerLabel.width)
                       tickerAnimation.running = true
                   }
               }

               MouseArea {
                   anchors.fill: tickerLableRect
                   z : 3
                   onClicked: root.tickerClicked()
               }

               DapQmlRectangle {
                   id: tickerLabelBackgraund
                   qss: "ticker_label_backgraund"
                   anchors.fill: parent
               }
           }

           DapQmlRectangle {
               id: tickerCloseRect
               qss: "ticker_close_rect"
               visible: true
               anchors.right: parent.right

               DapQmlPushButton {
                   id: tickerCloseButton
                   x: parent.width - width - y
                   y: (parent.height - height) / 2
                   z: 14

                   height: 24
                   width: 24
                   qss: "ticker_close_button"

                   onClicked: {
                       ticker.visible = false;
                   }
               }

               DapQmlRectangle {
                   id: tickerCloseBackground
                   qss: "ticker_label_backgraund"
                   anchors.fill: parent
               }
           }
       }

    /****************************************//**
     * Logo
     ********************************************/

    DapQmlRectangle {
        qss: "login-logo-container"
        visible: Brand.isEnterprise() === false
        DapQmlLabel {
            x: (parent.width - width) / 2
            z: 15
            qss: "login-logo"
        }
    }

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

    Rectangle {
        id: loginTypeContainer
        width: root.width
        color: "transparent"
        clip: true
        visible: Brand.isEnterprise() === true
        DapQmlStyle { item: loginTypeContainer; qss: "login-type-container" }

        function update() {
            tabCert.checked         = internal.mode === QuiLoginForm.Mode.M_CERT;
            tabSerial.checked       = internal.mode === QuiLoginForm.Mode.M_SERIAL;
            tabLoginPass.checked    = internal.mode === QuiLoginForm.Mode.M_PASSWORD;
        }

        DapQmlTabButton {
            id: tabCert
            qss: "login-mode-btn-cert"
            checked:    internal.mode === QuiLoginForm.Mode.M_CERT
            onClicked:  { internal.mode   = QuiLoginForm.Mode.M_CERT; loginTypeContainer.update(); }
        }

        DapQmlTabButton {
            id: tabSerial
            qss: "login-mode-btn-serial"
            checked:    internal.mode === QuiLoginForm.Mode.M_SERIAL
            onClicked:  { internal.mode   = QuiLoginForm.Mode.M_SERIAL; loginTypeContainer.update(); }
        }

        DapQmlTabButton {
            id: tabLoginPass
            qss: "login-mode-btn-loginpass"
            checked:    internal.mode === QuiLoginForm.Mode.M_PASSWORD
            onClicked:  { internal.mode   = QuiLoginForm.Mode.M_PASSWORD; loginTypeContainer.update(); }
        }
    }

    /****************************************//**
     * Top spacer
     ********************************************/

    DapQmlDummy {
        id: loginSpacer
        qss: (internal.mode === QuiLoginForm.Mode.M_CERT) ? "login-space-for2rows" : "login-space-for3rows"
        Component.onCompleted: StyleDebugTree.describe (
           "loginSpacer",
            ["x", "y", "width", "height"],
           this);
    }

    /****************************************//**
     * Top mode name
     ********************************************/

    DapQmlLabel {
        property string textCert:       qsTr("Standart Certificate") + lang.notifier
        property string textSerial:     qsTr("Serial + HW based DAP Cert") + lang.notifier
        property string textLoginPass:  qsTr("Login + Password") + lang.notifier

        id: loginTypeName
        y: loginSpacer.y
        width:       loginTypeNamePlacer.width
        height:      loginTypeNamePlacer.height
        fontSize:    loginTypeNamePlacer.fontSize
        fontFamiliy: loginTypeNamePlacer.fontFamiliy
        fontWeight:  loginTypeNamePlacer.fontWeight
        color:       loginTypeNamePlacer.color
        visible: Brand.isEnterprise() === true
        wrapMode: Text.WordWrap
        text: (internal.mode === QuiLoginForm.Mode.M_CERT)
              ? textCert
              : (internal.mode === QuiLoginForm.Mode.M_SERIAL)
                ? textSerial
                : textLoginPass

        DapQmlDummy {
            property var fontFamiliy
            property int fontSize
            property int fontWeight
            property color color
            id: loginTypeNamePlacer
            qss: "login-typename-label font-brand c-grey"
        }
    }

    /****************************************//**
     * Top separator
     ********************************************/

    DapQmlRectangle {
        x: loginSepsPlacer.x
        y: loginSpacer.y + loginSepsPlacer.y
        width: loginSepsPlacer.width
        height: loginSepsPlacer.height
        DapQmlSeparator {
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            qss: "login-separator"
        }
        DapQmlDummy {
            id: loginSepsPlacer
            qss: "login-separator-container"
        }
    }

    /****************************************//**
     * Choose server
     ********************************************/

    DapQmlRectangle {
        x:      loginServerPlacer.x
        y:      loginSpacer.y + loginServerPlacer.y
        width:  loginServerPlacer.width
        height: loginServerPlacer.height

        DapQmlButton {
            id: btnChooseServer
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("CHOOSING SERVER") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: true
            onClicked: root.sigChooseServer()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: loginServerPlacer
            qss: "login-btn-server-container"
        }
    }

    /****************************************//**
     * Enter serial
     ********************************************/

    DapQmlRectangle {
        x:      loginSerialPlacer.x
        y:      loginSpacer.y + loginSerialPlacer.y
        width:  loginSerialPlacer.width
        height: loginSerialPlacer.height
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
            subText: qsTr("SERIAL KEY") + lang.notifier
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
        DapQmlDummy {
            id: loginSerialPlacer
            qss: "login-btn-serial-container"
        }
    }

    /****************************************//**
     * Enter e-mail & password
     ********************************************/

    DapQmlRectangle {
        x:      loginEmailPlacer.x
        y:      loginSpacer.y + loginEmailPlacer.y
        width:  loginEmailPlacer.width
        height: loginEmailPlacer.height
        visible: internal.mode === QuiLoginForm.Mode.M_PASSWORD

        DapQmlButton {
            id: btnEnterEmail
            objectName: "btnEnterEmail"
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
        DapQmlDummy {
            id: loginEmailPlacer
            qss: "login-btn-email-container"
        }
    }

    DapQmlRectangle {
        x:      loginPasswordPlacer.x
        y:      loginSpacer.y + loginPasswordPlacer.y
        width:  loginPasswordPlacer.width
        height: loginPasswordPlacer.height
        visible: internal.mode === QuiLoginForm.Mode.M_PASSWORD

        DapQmlButton {
            id: btnEnterPassword
            objectName: "btnEnterPassword"
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
                        ? "qrc:/nonthemed/password-show.png"
                        : "qrc:/nonthemed/password-hide.png"
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
        DapQmlDummy {
            id: loginPasswordPlacer
            qss: "login-btn-password-container"
        }
    }

    /****************************************//**
     * Choose cert
     ********************************************/

    DapQmlRectangle {
        x:      loginChooseCertPlacer.x
        y:      loginSpacer.y + loginChooseCertPlacer.y
        width:  loginChooseCertPlacer.width
        height: loginChooseCertPlacer.height
        visible: internal.mode !== QuiLoginForm.Mode.M_PASSWORD && Brand.isEnterprise() === true

        DapQmlButton {
            id: btnChooseCert
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultCertName: "Certificate 5" // qsTr() + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedCert) ? (defaultCertName) : (internal.certName)
            subText: qsTr("CHOOSING CERTIFICATE") + lang.notifier
            qss: "login-btn-cert"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: true
            onClicked: root.sigChooseCert()

            function updateCertName() {
                mainText = (!internal.changedCert)
                        ? (defaultCertName)
                        : (internal.certName)
            }

            onDefaultCertNameChanged: updateCertName()
        }
        DapQmlDummy {
            id: loginChooseCertPlacer
            qss: (internal.mode === QuiLoginForm.Mode.M_CERT)
                ? "login-btn-cert-container2"
                : "login-btn-cert-container3"
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

        text: qsTr("CONNECT") + lang.notifier
        onClicked: {
            if (internal.mode === QuiLoginForm.Mode.M_SERIAL)
                root.sigConnectBySerial();
            else
            if (internal.mode === QuiLoginForm.Mode.M_PASSWORD)
                root.sigConnectByPassword();
            else
            if (internal.mode === QuiLoginForm.Mode.M_CERT)
                root.sigConnectByCert();
        }
    }

    /****************************************//**
     * Obtain key link
     ********************************************/

    DapQmlRectangle {
        qss: "login-obtain-container"

        DapQmlLabel {
            id: obtainLabel
            text: internal.forgotLabel() + lang.notifier
            color: "#5C5B74"
            width: parent.width / 2 - 2
            height: parent.height
            horizontalAlign: Text.AlignRight
            qss: "login-obtain-font c-label"
            mipmap: false
            visible: Brand.name() !== "RiseVPN"
//          font.family: "Lato"
//          font.pixelSize: 16
//          font.weight: Font.Normal
        }

        DapQmlLabel {
            id: obtainLinkLabel
//     First variant for Rise
//            x: parent.width / 2 + 2
            x: Brand.name() !== "RiseVPN" ? parent.width / 2 + 2 : 0
            text: internal.tapHereLabel() + lang.notifier
            color: "#DA0B82"
//     First variant for Rise
//            width: parent.width / 2
            width: Brand.name() !== "RiseVPN" ? parent.width / 2 : parent.width
            height: parent.height
//     First variant for Rise
//            horizontalAlign: Text.AlignLeft
            horizontalAlign:  Brand.name() !== "RiseVPN" ? Text.AlignLeft : Text.AlignCenter
            qss: Brand.name() !== "RiseVPN"
                     ? "login-obtain-font c-brand"
                     : "login-obtain-font c-brand"
            onClicked: {
//     First variant for Rise
//                (internal.mode === QuiLoginForm.Mode.M_SERIAL)
//                       ? root.sigObtainNewKey()
//                       : root.sigRecoverPassword()
                Brand.name() !== "RiseVPN"
                       ? root.sigObtainNewKey()
                       : root.sigShowCdbManager()
            }
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
