import QtQuick 2.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {

    id: root

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeConfirm"
    property string titleString: qsTr("Node")
    property bool buttonActive: true

    /// @brief item clicked
    signal walletClicked()
    signal networkClicked()
    signal tokenClicked()
    signal buttonClicked()


    function setWallet(walletName)
    {
        overview.setWallet(walletName)
    }

    function setNetwork(networkName)
    {
        overview.setNetwork(networkName)
    }

    function setToken(tokenName)
    {
        overview.setToken(tokenName)
    }

    function setSetConfirmButtonActive(active)
    {
        overview.buttonActive = active
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: root.titleString
        qss: "dialog-title"
    }

    QuiNodeOverviewBase {
        id: overview
        // form name
        formName: root.formName
        // buttons clicked
        onWalletClicked: root.walletClicked()
        onNetworkClicked: root.networkClicked()
        onTokenClicked: root.tokenClicked()
        onButtonClicked: root.buttonClicked()
        // button enable
        buttonActive: false//root.buttonActive
        // arrows on buttons
        link: true
    }

}
