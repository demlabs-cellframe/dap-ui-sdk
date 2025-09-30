/* INCLUDES */

import QtQuick 2.11
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
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
 * @date 15.04.24
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

        /// @brief login mode
        property int mode: QuiLoginForm.Mode.M_SERIAL
        property bool cdbDetected: false
    }

    /// @brief NoCDB variables
    property QtObject noCdb : QtObject {

        /// @brief flag tells when cellframe is detected or not
        property bool cellframeDetected: false

        /// @brief kel transaction processing for NoCBD
        property bool transactionProcessing: false

        /// @brief waiting for cellframe dashboard approval
        property bool waitingForApproval: false

        /// @brief true if network and token is set
        property bool tokenIsSet: false

        /// @brief true if history contain any orders
        property bool gotOrdersInsideHistory: false

        property real decreaseHeight: {
            if (gotOrdersInsideHistory)
                return btnChooseOrder.height * 0.125;
            else
                return 0;
        }
    }

    /// @brief Interface::Login class instance pointer
    property var interfaceObject

    // Activated serial state (to hide input and show banner)
    property bool serialActivated: false
    property string activatedSerial: ""

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigSerialFilled (string a_serial);
    signal sigSerialCleaned();
    signal sigSerialNotFinished();

    signal sigChooseServer();
    signal sigModeSerialSelected();
    signal sigModeNoCdbSelected();
    signal sigConnectBySerial();
    signal sigStartCondTransation();
    signal sigConnectByOrder();
    signal sigObtainNewKey();
    signal sigChooseWallet();
    signal sigSearchOrders();
    signal sigChooseOrderHistory();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function getAllWidgets() {
        var result = {
            "enterKeyField"     : enterKeyField,
            "connectionOverlay" : connectionOverlay,
            "btnChooseServer"   : btnChooseServer,
            "btnChooseWallet"   : btnChooseWallet,
            "btnChooseOrder"    : btnChooseOrder,
            "btnConnect"        : btnConnect,
            "btnContinue"       : btnContinue,
        };
        return result
    }

    function cdbDetected(a_value) {
        internal.cdbDetected    = a_value;
    }

    function setCellframeDetected(a_value) {
        noCdb.cellframeDetected = a_value;
    }

    function setTransactionProcessing(a_value) {
        noCdb.transactionProcessing = a_value;
        if (internal.cdbDetected === false)
        {
            internal.mode = QuiLoginForm.Mode.M_WALLET;
            root.walletSelected(internal.mode === QuiLoginForm.Mode.M_WALLET);
        }
    }

    function setWaitingForApproval(a_value) {
        noCdb.waitingForApproval = a_value;
    }

    function setTokenSet(a_value) {
        noCdb.tokenIsSet = a_value;
    }

    function setOrdersInsideHistoryFlag(a_value) {
        noCdb.gotOrdersInsideHistory = a_value;
    }

    function switchTab(a_index) {
        internal.mode   = a_index;
        loginTypeKelContainer.update();
    }

    // Format serial as XXXX-XXXX-XXXX-XXXX (remove non-alphanumerics first)
    function formatSerialForDisplay(raw) {
        if (raw === undefined || raw === null)
            return "____ ____ ____ ____";
        var s = ("" + raw).replace(/[^A-Za-z0-9]/g, "");
        if (s.length === 0)
            return "____ ____ ____ ____";
        var out = "";
        for (var i = 0; i < s.length; i++) {
            if (i > 0 && (i % 4) === 0)
                out += "-";
            out += s[i];
        }
        return out;
    }

    /// @}

    MouseArea {
        id: dummyFocus
        anchors.fill: parent
        z: 0
        hoverEnabled: false
        propagateComposedEvents: true
        onClicked: {
            Qt.inputMethod.hide();
            connectionOverlay.forceActiveFocus();
        }
    }

    /****************************************//**
     * Ticker & Update tools
     ********************************************/

    QuiToolTicker {}
    QuiToolUpdateNotification {}

    /****************************************//**
     * Server connecting overlay
     ********************************************/

    DapQmlRectangle {
        id: connectionOverlay
        anchors.fill: parent
        z: 300
        visible: false
        qss: "c-background"

        ColumnLayout {
            id: connOverlayLayout
            anchors.centerIn: parent

            DapQmlStyle { item: connOverlayLayout; qss: "login-connection-container" }

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
            enabled: connectionOverlay.visible
        }
    }

    /****************************************//**
     * Logo
     ********************************************/

    DapQmlLabel {
        z: 15
        qss: "login-logo"
    }

    /****************************************//**
     * Login by Serial
     ********************************************/

    Item {
        anchors.fill: parent
        visible: internal.mode === QuiLoginForm.Mode.M_SERIAL

        // Debug line to verify property values during integration
        DapQmlLabel {
            id: dbgBannerInfo
            z: 200
            anchors.horizontalCenter: parent.horizontalCenter
            text: "[DBG] serialActivated=" + serialActivated + ", serial=" + (activatedSerial ? activatedSerial : "")
            qss: "c-label"
        }

        /****************************************//**
         * Activated banner (shown instead of input)
         ********************************************/

        Item {
            id: activatedBanner
            // Place banner exactly over the serial input area
            anchors.top: enterKeyField.top
            anchors.left: enterKeyField.left
            width: enterKeyField.width
            height: enterKeyField.height
            visible: serialActivated === true
            z: 1000

            Column {
                id: bannerColumn
                anchors.centerIn: parent
                width: parent.width * 0.9
                spacing: 6
                z: 1

                DapQmlLabel {
                    width: parent.width
                    text: qsTr("Your serial key is activated")
                    horizontalAlign: Text.AlignHCenter
                    qss: "c-label"
                    // Keep readable size; theme may override if defined
                    fontSize: 18
                    fontWeight: 700
                    z: 2
                    implicitHeight: contentHeight
                }

                DapQmlLabel {
                    width: parent.width
                    text: activatedSerial && activatedSerial.length > 0 ? formatSerialForDisplay(activatedSerial) : "____ ____ ____ ____"
                    horizontalAlign: Text.AlignHCenter
                    qss: "c-brand"
                    fontSize: 16
                    fontWeight: 600
                    z: 2
                    implicitHeight: contentHeight
                }
            }

            // Debug: log when banner visibility or data changes
            onVisibleChanged: {
                console.log("[Login Banner] visible=", visible, "serialActivated=", serialActivated, "serial=", activatedSerial)
            }
        }

        /****************************************//**
         * Enter serial
         ********************************************/

        DapQmlButton {
            id: enterKeyField
            z: 15

            mainText: ""
            subText: qsTr("SERIAL KEY") + lang.notifier
            buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
            frame: true

            qss: noCdb.cellframeDetected ? "login-btn-serial-nocdb" :  "login-btn-serial-cdb"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"

            placeHolderText: "____ ____ ____ ____"
            placeHolderQss: "login-btn-main"

            /* signals */

            onTextAccepted: root.beginConnection()
            onTextChanged: checkSerialKeyData()
            onTextEdited: checkSerialKeyData()

            /* variables */

            readonly property int maxCountChar: 19

            /* functions */

            function checkSerialKeyData() {
                var text    = mainText;

                if (text.length == maxCountChar)
                    root.sigSerialFilled (mainText);
                else if (text.length === 0)
                    root.sigSerialCleaned();
                else
                    root.sigSerialNotFinished();
            }

            Keys.onLeftPressed: {
                event.accepted = true
            }

            Keys.onRightPressed: {
                event.accepted = true
            }
            // Hide input when serial is already activated
            visible: internal.mode === QuiLoginForm.Mode.M_SERIAL && serialActivated === false
        }

        /****************************************//**
         * Choose server
         ********************************************/

        DapQmlButton {
            id: btnChooseServer
            x: (parent.width - width) / 2
            z: 15

            mainText: ""
            subText: qsTr("CHOOSING SERVER") + lang.notifier
            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            frame: true
            link: true

            qss: noCdb.cellframeDetected ? "login-btn-choose-order" : "login-btn-choose-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"

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

                    if (interfaceObject !== undefined)
                        label.font.pixelSize    = interfaceObject.scaleServerLabelFont (text, fontSize, width * 0.85);
                }
            }
        }
    }

    /****************************************//**
     * NoCDB
     ********************************************/

    Item {
        anchors.fill: parent
        visible: internal.mode === QuiLoginForm.Mode.M_WALLET
                 && noCdb.waitingForApproval === false

        /****************************************//**
         * Choose Wallet
         ********************************************/

        DapQmlButton {
            id: btnChooseWallet
            z: 15
            x: (parent.width - width) / 2
            width: parent.width

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            frame: true
            link: true

            qss: noCdb.cellframeDetected ? "login-btn-serial-nocdb" :  "login-btn-serial-cdb"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"

            /* signals */

            onClicked: root.sigChooseWallet()
        }

        /****************************************//**
         * Choose Order
         ********************************************/

        DapQmlRectangle {
            id: btnChooseOrderContainer
            qss: noCdb.cellframeDetected ? "login-btn-choose-order" : "login-btn-choose-server"
            opacity: noCdb.tokenIsSet ? 1.0 : 0.5
            enabled: noCdb.tokenIsSet

            MouseArea {
                anchors.fill: parent
                onClicked: root.sigSearchOrders()
            }

            Item {
                width: btnChooseOrderContainer.width
                height: btnChooseOrderContainer.height// - noCdb.decreaseHeight
                clip: true

                DapQmlButton {
                    id: btnChooseOrder
                    anchors.fill: parent
                    z: 15

                    mainText: qsTr("Order") + lang.notifier
                    subText: qsTr("SEARCH ORDERS") + lang.notifier
                    buttonStyle: DapQmlButton.Style.TopMainBottomSub
                    visible: internal.mode === QuiLoginForm.Mode.M_WALLET
                    enabled: noCdb.tokenIsSet
                    frame: true
                    link: true

                    mainQss: "login-btn-main"
                    subQss: "login-btn-sub"

                    /* signals */

                    onClicked: root.sigSearchOrders()
                }
            }

            /* bottom rectangle */

            DapQmlRectangle {
                x: btnChooseOrderContainer.x
                y: btnChooseOrderContainer.y + btnChooseOrderContainer.height - height
                width: btnChooseOrderContainer.width
                height: btnChooseOrderContainer.height * 0.2
                visible: noCdb.gotOrdersInsideHistory
                         && internal.mode === QuiLoginForm.Mode.M_WALLET
                color: btnOrderHistory.color
            }
        }

        /****************************************//**
         * Order History
         ********************************************/

        DapQmlRectangle {
            id: btnOrderHistory
            x: btnChooseOrderContainer.x
            y: btnChooseOrderContainer.y + btnChooseOrderContainer.height - noCdb.decreaseHeight
            z: 14
            visible: noCdb.gotOrdersInsideHistory
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
    }

    /****************************************//**
     * NoCDB Waiting for Approval
     ********************************************/

    Item {
        anchors.fill: parent
        visible: internal.mode === QuiLoginForm.Mode.M_WALLET
                 && noCdb.waitingForApproval === true

        /****************************************//**
         * Arc Animation
         ********************************************/

        DapQmlRectangle {
            id: progressCircle
            qss: "login-transaction-processing-arc-animation"

            DapQmlArcAnimation {
                anchors.fill: parent
                anchors.margins: parent.width * 0.275
                strokeWidth: 7
                z: 200
                qss: "c-brand"
            }
        }

        DapQmlLabel {
            id: loginInfoLabel
            qss: "login-transaction-processing-label-nocbd"
            text: (noCdb.transactionProcessing
                  ? qsTr("Transaction is in progress")
                  : qsTr("Waiting for approval\n\nCheck the Cellframe Dashboard"))
                  + lang.notifier
        }
    }

    /****************************************//**
     * CDB & NoCBD mode select (SERIAL <-> WALLET)
     ********************************************/

    DapQmlRectangle {
        id: loginTypeKelContainer
        color: "transparent"
        clip: true
        visible: noCdb.cellframeDetected
        qss: "login-mode-container"

        function update() {
            tabSerial.checked   = internal.mode === QuiLoginForm.Mode.M_SERIAL;
            tabNoCdb.checked    = internal.mode === QuiLoginForm.Mode.M_WALLET;
        }

        DapQmlDummy {
            id: tabButtonSize
            visible: false
            qss: "login-mode-button-size"
        }

        RowLayout {
            anchors.fill: parent

            DapQmlTabButton {
                id: tabSerial
                Layout.preferredWidth: tabButtonSize.width
                Layout.preferredHeight: tabButtonSize.height
                qss: "login-mode-serial"
                checked:    internal.mode === QuiLoginForm.Mode.M_SERIAL
                onClicked:  {
                    internal.mode = QuiLoginForm.Mode.M_SERIAL;
                    loginTypeKelContainer.update();
                    root.sigModeSerialSelected();
                }
            }

            DapQmlTabButton {
                id: tabNoCdb
                Layout.preferredWidth: tabButtonSize.width
                Layout.preferredHeight: tabButtonSize.height
                qss: "login-mode-nocdb"
                checked:    internal.mode === QuiLoginForm.Mode.M_WALLET
                onClicked:  {
                    internal.mode = QuiLoginForm.Mode.M_WALLET;
                    loginTypeKelContainer.update();
                    root.sigModeNoCdbSelected();
                }
            }
        }
    }

    /****************************************//**
     * Connect button
     ********************************************/

    DapQmlPushButton {
        id: btnConnect
        x: (parent.width - width) / 2
        z: 15
        visible: internal.mode === QuiLoginForm.Mode.M_SERIAL
        qss: noCdb.cellframeDetected
             ? "login-btn-connect-by-nocbd push-button"
             : "login-btn-connect-by-serial push-button"
        text: (noCdb.cellframeDetected ? qsTr("CONTINUE") : qsTr("CONNECT")) + lang.notifier
        onClicked: root.sigConnectBySerial()
    }

    /****************************************//**
     * Continue button (NoCDB)
     ********************************************/

    DapQmlPushButton {
        id: btnContinue
        x: (parent.width - width) / 2
        z: 15
        visible: internal.mode === QuiLoginForm.Mode.M_WALLET
                 && noCdb.waitingForApproval === false
        qss: "login-btn-connect-by-nocbd push-button"
        text: (noCdb.cellframeDetected ? qsTr("CONTINUE") : qsTr("CONNECT")) + lang.notifier
        onClicked: {
            if (noCdb.transactionProcessing === false)
                root.sigStartCondTransation();
            else
                root.sigConnectByOrder();
        }
    }

    /****************************************//**
     * Obtain key link
     ********************************************/

    DapQmlRectangle {
        qss: noCdb.cellframeDetected
             ? "login-obtain-container-mocbd-mode"
             : "login-obtain-container-serial-mode"

        RowLayout {
            anchors.fill: parent

            DapQmlLabel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                text: qsTr("Don't have a serial key?") + lang.notifier
                horizontalAlign: Text.AlignRight
                qss: "login-obtain-font c-label"
            }

            DapQmlLabel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                text: qsTr("Tap here to obtain one") + lang.notifier
                horizontalAlign: Text.AlignLeft
                qss: "login-obtain-font c-brand"
                onClicked: root.sigObtainNewKey()

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

/*-----------------------------------------*/
