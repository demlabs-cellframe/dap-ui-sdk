/* INCLUDES */

import QtQuick 2.11
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.4
import DapQmlSerialKeyInput 1.0
import StyleDebugTree 1.0
import DapQmlStyle 1.0
import Brand 1.0
import PageCtl 1.0
import com.DapQmlWidgets 1.0
import "qrc:/dapqml-widgets"
import "qrc:/dapqml-forms/tools"

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
//        use in RiseVPN
        M_PASSWORD,
        M_CERT,
//        use with 'cellfarameDetected'
        M_WALLET
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

        property bool changedCert:  false
        property string certName:   ""
        property bool legacyStyle: Brand.name() !== "KelVPN"

        /// @brief login mode
        property int mode: QuiLoginForm.Mode.M_SERIAL

        /// @brief kel cellframe dashdoard detected
        property bool cellfarameDetected: false

        /// @brief kel transaction processing for NoCBD
        property bool transactionProcessing: false
        property bool waitingForApproval: false

        /// @brief show password contents
        property bool showPassword: false

        property bool showConnectionOverlay: false

        property bool tokenIsSet: false

        property bool gotOrdersInsideHistory: false

        function forgotLabel() {
//   First variant for Rise
//            return mode === QuiLoginForm.Mode.M_SERIAL
//                ? qsTr("Don't have a serial key?")
//                : qsTr("Forgot your password?")
            return mode === QuiLoginForm.Mode.M_SERIAL
                ? qsTr("Don't have a serial key?")
                : qsTr("Don't have a serial key?")

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

    /// @brief search orders button clicked
    signal sigSearchOrders()

    /// @brief choose certificate button clicked
    signal sigChooseCert();

    /// @brief enter serial key clicked
    signal sigChooseSerial();

    /// @brief enter wallet key clicked
    signal sigChooseWallet();

    /// @brief enter maxprice key clicked
    signal sigChooseMaxPrice();

    /// @brief choose order from history
    signal sigChooseOrderHistory();

    /// @brief connection by serial requested
    signal sigConnectBySerial();

    /// @brief connection by login & password requested
    signal sigConnectByPassword();

    /// @brief connection by certificate requested
    signal sigConnectByCert();

    /// @brief connection by order requested
    signal sigConnectByOrder();

    signal sigStartCondTransation();

    /// @brief buy serial clicked
    signal sigObtainNewKey();

    /// @brief start recovering password
    signal sigRecoverPassword();

    /// @brief entered serial is incorerct
    signal sigSerialFillingIncorrect();

    /// @brief show server manager
    signal sigShowCdbManager();

    /// @brief waiting approval retry button clicked
    signal sigRetryButtonClicked();

    /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /// nocdb debug func, this feature should be removed
    property bool debugNoCDB: true
    signal sigDebugNoCDBMode();

    signal textEditedAndCleaned();
    signal textEditedAndFilledOut (string serial);

    signal textChangedAndCleaned();
    signal textChangedAndFilledOut (string serial);

    signal walletSelected(bool selected);
    signal sigStartUpdate();

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
        //btnChooseServer.mainText    = a_name;
        btnChooseServer.label    = a_name;
    }

    function setOrderLocation(location ) {
        btnChooseOrder.mainText    = location;
    }
    function setOrderAddr(addr) {
        btnChooseOrder.subText     = addr;
    }


    /// @brief set wallet for noCBD
    function setWallet(a_wallet) {
        btnChooseWallet.mainText = a_wallet;
        //internal.waitingForApproval = false
    }

    /// @brief set network|token for noCBD
    function setNetworkAndToken(a_data){
        btnChooseWallet.subText     = a_data;
        root.internal.tokenIsSet    = a_data.includes("|");
    }

    /// @brief set transaction processing flag for noCBD
    function setTransactionProcessing(a_data){
        console.log(`setTransactionProcessing ${a_data}`);
        internal.transactionProcessing = true;
        loginInfoLabel.text = "Transaction is in progress"
    }

    function deactivateTransactionProcessing() {
        internal.transactionProcessing = false;
        loginInfoLabel.text = ""
    }

    /// @brief set input mask for serial input
    function setupInputMask() {
        //btnEnterSerial.inputMask    = ">NNNN-NNNN-NNNN-NNNN;_"
    }

    /// @briefset found cellframe dashboard
    function cellfarameDashboardDetected(detected) {
        console.log(`cellfarameDashboardDetected ${detected}`);
        internal.cellfarameDetected = detected && Brand.name() === "KelVPN";
        if (internal.waitingForApproval)
            loginInfoLabel.text = qsTr("Waiting for approval\n\nCheck the Cellframe Dashboard");
        loginTypeKelContainer.update();
    }

    function setWaitingForApproval(approval) {
        console.log(`setWaitingForApproval ${approval}`);
        internal.waitingForApproval = approval
        if (internal.waitingForApproval)
            loginInfoLabel.text = qsTr("Waiting for approval\n\nCheck the Cellframe Dashboard")
    }

    function setWalletSeleted(selected)
    {
        if (!selected)
            return
        internal.mode = QuiLoginForm.Mode.M_WALLET;
        loginTypeKelContainer.update();
    }

    function beginConnection() {
        if (btnConnect.enabled === false)
            return;
        if (internal.mode === QuiLoginForm.Mode.M_SERIAL)
            root.sigConnectBySerial();
        else
        if (internal.mode === QuiLoginForm.Mode.M_PASSWORD)
            root.sigConnectByPassword();
        else
        if (internal.mode === QuiLoginForm.Mode.M_CERT)
            root.sigConnectByCert();
    }

    function showConnectionOverlay(a_show) {
        root.internal.showConnectionOverlay = a_show;
    }

    function setOrdersInsideHistoryFlag(a_value) {
        root.internal.gotOrdersInsideHistory    = a_value;
    }

    /// @}
    /****************************************//**
     * Ticker & Update tools
     ********************************************/

    QuiToolTicker {}
    QuiToolUpdateNotification {}

    /****************************************//**
     * Server connecting overlay
     ********************************************/

    DapQmlRectangle {
        anchors.fill: parent
        z: 300
        qss: "c-background"
        visible: root.internal.showConnectionOverlay

        ColumnLayout {
            id: connOverlay
            anchors.centerIn: parent

            DapQmlStyle { item: connOverlay; qss: "login-connection-container" }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: root.width * 0.14953271

                DapQmlArcAnimation {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.height
                    height: parent.height
                    qss: "c-brand"
                }
            }

            DapQmlLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                qss: "login-connecting-label"
                text: qsTr("Connecting...")
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: root.internal.showConnectionOverlay
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
     * Login type select for RiseVPN
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
            onClicked:  {
                internal.mode   = QuiLoginForm.Mode.M_SERIAL;
                root.walletSelected(internal.mode === QuiLoginForm.Mode.M_WALLET);
                loginTypeContainer.update();
            }
        }

        DapQmlTabButton {
            id: tabLoginPass
            qss: "login-mode-btn-loginpass"
            checked:    internal.mode === QuiLoginForm.Mode.M_PASSWORD
            onClicked:  { internal.mode   = QuiLoginForm.Mode.M_PASSWORD; loginTypeContainer.update(); }
        }
    }

    /****************************************//**
     * KelVPN login type select for NoCBD (SERIAL, WALLET)
     ********************************************/

    Rectangle {
        id: loginTypeKelContainer
        width: root.width
        color: "transparent"
        clip: true
        visible: Brand.name() === "KelVPN" && internal.cellfarameDetected
        DapQmlStyle { item: loginTypeKelContainer; qss: "login-type-container" }
//        Rectangle {
//            color: "yellow"
//            anchors.fill: parent
//        }

        function update() {
            tabSerial1.checked = internal.mode === QuiLoginForm.Mode.M_SERIAL;
            tabCell.checked    = internal.mode === QuiLoginForm.Mode.M_WALLET;
        }

        DapQmlTabButton {
            id: tabSerial1
            qss: "login-mode-btn-serial-nocbd"
            checked:    internal.mode === QuiLoginForm.Mode.M_SERIAL
            onClicked:  {
                internal.mode = QuiLoginForm.Mode.M_SERIAL;
                loginTypeKelContainer.update();
                root.walletSelected(internal.mode === QuiLoginForm.Mode.M_WALLET);
            }
        }

        DapQmlTabButton {
//            Rectangle {
//                color: "blue"
//                anchors.fill: parent
//            }
            id: tabCell
            qss: "login-mode-btn-nocbd"
            checked:    internal.mode === QuiLoginForm.Mode.M_WALLET
            onClicked:  { internal.mode = QuiLoginForm.Mode.M_WALLET;
                loginTypeKelContainer.update();
                root.walletSelected(internal.mode === QuiLoginForm.Mode.M_WALLET);
            }
        }
    }

    /****************************************//**
     * Top spacer
     ********************************************/

    DapQmlDummy {
        id: loginSpacer
        qss: (internal.mode === QuiLoginForm.Mode.M_CERT) ? "login-space-for2rows" : "login-space-for3rows"
//        Component.onCompleted: StyleDebugTree.describe (
//           "loginSpacer",
//            ["x", "y", "width", "height"],
//           this);
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
            property string fontFamiliy
            property int fontSize
            property int fontWeight
            property color color
            id: loginTypeNamePlacer
            qss: "login-typename-label font-brand c-grey"
        }
    }

//    /****************************************//**
//     * Top separator
//     ********************************************/

//    DapQmlRectangle {
//        x: loginSepsPlacer.x
//        y: loginSpacer.y + loginSepsPlacer.y
//        width: loginSepsPlacer.width
//        height: loginSepsPlacer.height
//        DapQmlSeparator {
//            x: (parent.width - width) / 2
//            z: 15
//            width: parent.width - 74
//            qss: "login-separator"
//        }
//        DapQmlDummy {
//            id: loginSepsPlacer
//            qss:Brand.name() === "KelVPN" && internal.cellfarameDetected
////               NoCBD mode
//                 ? internal.mode === QuiLoginForm.Mode.M_WALLET
////               wallet
////                 ? "login-nocbd-wallet-separator-container"
//                 ? "login-nocbd-skey-separator-container"
////               serial login
//                 : "login-nocbd-skey-separator-container"
////               other
//                 : "login-separator-container"
//        }
//    }

    /****************************************//**
     * Choose wallet for NoCBD
     ********************************************/

//=======
//        x: loginSepsPlacer.x
//        y: loginSpacer.y + loginSepsPlacer.y
//        width: loginSepsPlacer.width
//        height: loginSepsPlacer.height
//        visible: root.internal.legacyStyle

//        DapQmlSeparator {
//            x: (parent.width - width) / 2
//            z: 15
//            width: parent.width - 74
//            qss: "login-separator"
//        }

//        DapQmlDummy {
//            id: loginSepsPlacer
//            qss: "login-separator-container"
//        }
//    }

//    /****************************************//**
//     * Choose server
//     ********************************************/

//    DapQmlRectangle {
//        x:      loginServerPlacer.x
//        y:      loginSpacer.y + loginServerPlacer.y
//        width:  loginServerPlacer.width
//        height: loginServerPlacer.height
//>>>>>>> refs/heads/release-7.7

    DapQmlRectangle {
        x:      loginWalletPlacer.x
        y: {
            let pos = loginSpacer.y + loginWalletPlacer.y;
            if (root.internal.gotOrdersInsideHistory
                && internal.mode === QuiLoginForm.Mode.M_WALLET)
                return pos - btnOrderHistory.height;
            else
                return pos;
        }
        width:  loginWalletPlacer.width
        height: loginWalletPlacer.height
        visible: internal.cellfarameDetected && internal.mode === QuiLoginForm.Mode.M_WALLET
                 //&& !(internal.transactionProcessing || internal.waitingForApproval)
                 && !internal.waitingForApproval

        DapQmlButton {
            id: btnChooseWallet
            x: (parent.width - width) / 2
            z: 15
            width: parent.width

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: root.internal.legacyStyle
            frame: !root.internal.legacyStyle // true
            link: true
            onClicked: root.sigChooseWallet()
        }

        DapQmlDummy {
            id: loginWalletPlacer
            qss: "login-btn-wallet-container"
        }
    }

    /****************************************//**
     * Choose server
     ********************************************/

    DapQmlRectangle {
        x:      loginServerPlacer.x
        y: {
            let pos = loginSpacer.y + loginServerPlacer.y;
            if (root.internal.gotOrdersInsideHistory
                && internal.mode === QuiLoginForm.Mode.M_WALLET)
                return pos - btnOrderHistory.height;
            else
                return pos;
        }
        width:  loginServerPlacer.width
        height: loginServerPlacer.height
        visible: {
            if (Brand.name() === "KelVPN" && internal.mode === QuiLoginForm.Mode.M_WALLET) {
                return internal.cellfarameDetected
                        //&& (!internal.transactionProcessing && !internal.waitingForApproval)
                       && !internal.waitingForApproval
            }
            else {
                return true;
            }
        }

        DapQmlButton {
            id: btnChooseServer
            x: (parent.width - width) / 2
            z: 15
            width: parent.width
            visible: internal.mode !== QuiLoginForm.Mode.M_WALLET
            buttonStyle: DapQmlButton.Style.TopMainBottomSub

            mainText: "" // qsTr("Auto select") + lang.notifier
            subText: qsTr("CHOOSING SERVER") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            frame: true
            link: true

            /* signals */

            onClicked: root.sigChooseServer()

            /* server label */

            property string label: qsTr("Auto select") + lang.notifier

            DapQmlLabel {
                anchors.fill: parent
                anchors.bottomMargin: parent.height / 2
                anchors.leftMargin:   parent.width * 0.15
                anchors.rightMargin:  parent.width * 0.15

                fontSize: btnChooseServer.labelMain.fontSize
                fontWeight: btnChooseServer.labelMain.fontWeight
                color: btnChooseServer.labelMain.color
                verticalAlign: Text.AlignBottom
                disableClicking: true
                text: parent.label

                onTextChanged: rescaleLabel()
                onWidthChanged: rescaleLabel()

                function rescaleLabel() {
                    /* check error */
                    if (btnChooseServer.labelMain === undefined)
                        return;

                    /* copy correct font size */
                    fontSize = btnChooseServer.labelMain.fontSize;
                    label.text = text;
                    label.font.pixelSize = fontSize;

                    /* variables */
                    let ww = width;
                    //let cw = label.contentWidth;
                    //console.log(`LoginForm: contentWidth ${cw}, width ${ww}`);

                    /* scale down when required */
                    while (label.contentWidth > ww && fontSize > 2)
                    {
                        fontSize -= 1;
                        label.font.pixelSize = fontSize;
                    }

                    /* if scaling failed, print error */
                    if (fontSize <= 2)
                    {
                        fontSize = btnChooseServer.labelMain.fontSize
                        label.font.pixelSize = fontSize;
                        console.log(`LoginForm: Unable to scale server name label: ${text}`);
                    }

                    /* successful scaling */
                    else
                        console.log(`LoginForm: Scaled server name label: ${text} to ${fontSize} (${label.contentWidth.toFixed(2)}:${ww.toFixed(2)})`);
                }
            }
        }

        Item {
            anchors.fill: parent
            opacity: root.internal.tokenIsSet ? 1.0 : 0.5
            enabled: root.internal.tokenIsSet

            property real decreaseHeight: {
                if (root.internal.gotOrdersInsideHistory
                    && internal.mode === QuiLoginForm.Mode.M_WALLET)
                    return btnChooseOrder.height * 0.2;
                else
                    return 0;
            }

            MouseArea {
                anchors.fill: parent
                onClicked: root.sigSearchOrders()
            }

            Item {
                width: parent.width
                height: parent.height - parent.decreaseHeight
                clip: true

                DapQmlButton {
                    id: btnChooseOrder
                    x: (parent.width - width) / 2
                    z: 15
                    width: parent.width
                    visible: internal.mode === QuiLoginForm.Mode.M_WALLET
                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    enabled: root.internal.tokenIsSet

                    mainText: qsTr("Order") + lang.notifier
                    subText: qsTr("SEARCH ORDERS") + lang.notifier
                    qss: "login-btn-server"
                    mainQss: "login-btn-main"
                    subQss: "login-btn-sub"
                    frame: true
                    link: true
                    onClicked: root.sigSearchOrders()
                }
            }

            /* bottom rectangle */

            DapQmlRectangle {
                x: btnChooseOrder.x
                y: btnChooseOrder.y + btnChooseOrder.height - height
                width: btnChooseOrder.width
                height: btnChooseOrder.height * 0.2
                visible: root.internal.gotOrdersInsideHistory
                         && internal.mode === QuiLoginForm.Mode.M_WALLET
                color: btnOrderHistory.color
            }
        }

        DapQmlRectangle {
            id: btnOrderHistory
            x: btnChooseOrder.x
            y: btnChooseOrder.y + btnChooseOrder.height
            z: 14
            visible: root.internal.gotOrdersInsideHistory
                     && internal.mode === QuiLoginForm.Mode.M_WALLET
            radius: btnChooseOrder.height * 0.1
            qss: "login-btn-orders-history-frame"

            property int fontSize
            property int fontWeight

            /* top rectangle */

            DapQmlRectangle {
                width: parent.width
                height: parent.radius * 2
                color: parent.color

                /* separator */

                DapQmlSeparator {
                    width: parent.width
                }
            }

            /* label */

            DapQmlLabel {
                anchors.fill: parent
                anchors.bottomMargin: parent.height * 0.075
                z: 0
                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignVCenter
                text: qsTr("My orders")
                fontSize: parent.fontSize
                fontWeight: parent.fontWeight
                qss: "c-label"
            }

            /* link icon */

            DapQmlImage {
                id: linkImage
                x: parent.width - width * 2.4
                y: (parent.height - height) / 2
                width: btnChooseOrder.height / 5
                height: width
                DapQmlStyle { qss: "btn-arrow"; item: linkImage }
            }

            /* clickable */

            MouseArea {
                anchors.fill: parent
                onClicked: root.sigChooseOrderHistory()
            }
        }

        DapQmlDummy {
            id: loginServerPlacer
            qss: {
                if (Brand.name() === "KelVPN"
                    && internal.cellfarameDetected)
                {
                    //if (internal.mode === QuiLoginForm.Mode.M_WALLET)
                    //    return "login-btn-nocbd-wallet-server-container"
                    return "login-btn-nocbd-skey-server-container"
                }
                else
                    return "login-btn-server-container"
            }

//            qss: Brand.name() === "KelVPN" && internal.cellfarameDetected
////               NoCBD mode
//                 ? internal.mode === QuiLoginForm.Mode.M_WALLET
////               wallet
//                 //? "login-btn-nocbd-wallet-server-container"
//                 ? "login-btn-nocbd-skey-server-container"
////               serial login
//                 : "login-btn-nocbd-skey-server-container"
////               other
//                 : "login-btn-server-container"

        }
    }
    /****************************************//**
     * Transaction processing label
     ********************************************/

    Item {
        anchors.fill: parent
        visible: Brand.name() === "KelVPN" && internal.mode === QuiLoginForm.Mode.M_WALLET

        /* arc animation */
        DapQmlRectangle {
            id: progressCircle
            visible: internal.cellfarameDetected
                     //&& (internal.transactionProcessing || internal.waitingForApproval)
                    && internal.waitingForApproval
            qss: "login-transaction-processing-arc-animation"

            DapQmlArcAnimation {
                anchors.fill: parent
                anchors.margins: parent.width * 0.275
                strokeWidth: 7
                z: 200
                qss: "c-brand"
            }
        }

//        /* retry button */
//        DapQmlRectangle {
//            id: retryBtn
//            visible: internal.cellfarameDetected
//                     && (internal.transactionProcessing || internal.waitingForApproval)
//            qss: "login-transaction-processing-retry-button"

//            QtObject {
//                id: animationCtl
//                property Timer timer: Timer {
//                    interval: 500
//                    running: false
//                    repeat: false
//                    onTriggered: animationCtl.finish()
//                }

//                function start() {
//                    retryBtn.qss    = "c-brand";
//                    timer.start();
//                }

//                function finish() {
//                    retryBtn.qss    = "login-transaction-processing-retry-button";
//                }
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    animationCtl.start();
//                    root.sigRetryButtonClicked();
//                }
//            }

//            DapQmlLabel {
//                anchors.centerIn: parent
//                disableClicking: true
//                width: contentWidth
//                height: contentHeight
//                qss: "c-label"
//                text: "Retry"
//            }
//        }

        DapQmlLabel {
            id: loginInfoLabel
            qss: "login-transaction-processing-label-nocbd"
            text: "<<< Message >>>"
            visible: internal.cellfarameDetected
                     //&& (internal.transactionProcessing || internal.waitingForApproval)
                     && internal.waitingForApproval
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

//        Component.onCompleted: StyleDebugTree.describe (
//           "btnEnterSerialBox",
//            ["x", "y", "width", "height", "visible"],
//           this);

        DapQmlButton {
            id: btnEnterSerial
            objectName: "btnEnterSerial"
            property int maxCountChar: 19
            x: (parent.width - width) / 2
            z: 15
            width: parent.width
            //height: parent.height

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            mainText: ""
            subText: qsTr("SERIAL KEY") + lang.notifier
            qss: "login-btn-serial"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            placeHolderText: "____ ____ ____ ____"
            placeHolderQss: "login-btn-main"
            //inputMask: ">NNNN-NNNN-NNNN-NNNN;_"
            separator: root.internal.legacyStyle
            frame: !root.internal.legacyStyle // true

            onClicked: root.sigChooseSerial()
            onTextAccepted: root.beginConnection()
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

//            Component.onCompleted: StyleDebugTree.describe (
//               "btnEnterSerial",
//                ["x", "y", "width", "height"],
//               this);
        }
        DapQmlDummy {
            id: loginSerialPlacer
            qss: Brand.name() === "KelVPN" && internal.cellfarameDetected
//                 NoCBD mode
                 ? "login-btn-serial-container-nocbd"
//                 serial login
                 : "login-btn-serial-container"

//            Component.onCompleted: StyleDebugTree.describe (
//               "loginSerialPlacer",
//                ["x", "y", "width", "height", "qss"],
//               this);
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
            width: parent.width

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            mainText: ""
            subText: "EMAIL"
            qss: "login-btn-email"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: root.internal.legacyStyle
            frame: !root.internal.legacyStyle // true
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
            width: parent.width

            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            mainText: ""
            subText: "PASSWORD"
            qss: "login-btn-password"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            editEchoMode: (internal.showPassword)
                          ? TextInput.Normal
                          : TextInput.Password
            separator: root.internal.legacyStyle
            frame: !root.internal.legacyStyle // true
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
            width: parent.width
            property string defaultCertName: "Certificate 5" // qsTr() + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedCert) ? (defaultCertName) : (internal.certName)
            subText: qsTr("CHOOSING CERTIFICATE") + lang.notifier
            qss: "login-btn-cert"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: root.internal.legacyStyle
            frame: !root.internal.legacyStyle // true
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
        qss: Brand.name() === "KelVPN" && internal.cellfarameDetected
//                 NoCBD mode
             ? "login-connect-nocbd-mode push-button"
//                 serial login
             : "login-connect push-button"
        text: Brand.name() === "KelVPN" && internal.cellfarameDetected
        //                 NoCBD mode
                     ? qsTr("CONTINUE") + lang.notifier
        //                 serial login
                     : qsTr("CONNECT") + lang.notifier
        onClicked: {
            if (internal.mode === QuiLoginForm.Mode.M_SERIAL)
                root.sigConnectBySerial();
            else
            if (internal.mode === QuiLoginForm.Mode.M_PASSWORD)
                root.sigConnectByPassword();
            else
            if (internal.mode === QuiLoginForm.Mode.M_CERT)
                root.sigConnectByCert();
            else
            if (internal.mode === QuiLoginForm.Mode.M_WALLET) {
//                root.internal.transactionOverviewShow = true
//                PageCtl.showTransaction();
                if (internal.transactionProcessing === false)
                    root.sigStartCondTransation()
                else
                    root.sigConnectByOrder();
            }
        }
    }

    /****************************************//**
     * Obtain key link
     ********************************************/

    DapQmlRectangle {
        qss: Brand.name() === "KelVPN" && internal.cellfarameDetected
//                 NoCBD mode
             ? "login-obtain-container-mocbd-mode"
//                 serial login
             : "login-obtain-container"

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
