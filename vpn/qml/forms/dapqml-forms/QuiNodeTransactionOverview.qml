import QtQuick 2.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {

    id: root
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeTransactionOverview"
    property string titleOverview: qsTr("Transaction owerview")
    property string titleProcessing: qsTr("Transaction processing")

    /// @brief confirm clicked
    signal sigConfirm();


    function setMode(mode)
    {
        if (mode === 1)
        {
            title.text = titleOverview
            processingItem.visible = false
            overview.visible = true
            overview.buttonActive = true
        }
        else
        {
            title.text = titleProcessing
            processingItem.visible = true
            overview.visible = false
        }
    }

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

    function setTokenCount(tokenCount)
    {
        overview.setTokenCount(tokenCount)
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: titleOverview
        qss: "dialog-title"
    }

    QuiNodeOverviewBase {
        id: overview
        // form name
        formName: root.formName
        // button enable
        buttonActive: true
        //
        cellEnabled: true
        // arrows on buttons
        link: false
        onButtonClicked: { root.sigConfirm() }
    }


    /****************************************//**
     * Mode Sending Item
     ********************************************/

    Item {
        id: processingItem
        anchors.fill: parent
        visible: false

        /* info */
        DapQmlLabel {
            qss: "bugrep-sending"
            text: qsTr("Sending...") + lang.notifier
        }

        /* animated spinner */
        AnimatedImage {
            id: animation
            source: "qrc:/dapqml-forms-asset/Spinner.gif"
            DapQmlStyle { qss: "bugrep-animation"; item: animation }
        }

        /* cancel */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: qsTr("CANCEL") + lang.notifier
            onClicked: {
//                root.sigConfirm();
            }
        }
    }

}
