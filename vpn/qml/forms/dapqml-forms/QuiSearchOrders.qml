import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {

    id: root
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "Search  order"
    property bool link: false
    property bool cellEnabled: false
    property bool buttonActive: false

    signal networkClicked()
    signal tokenClicked()
    signal cellClicked()
    signal buttonClicked()

    function setNetwork(networkName)
    {
        btnChooseNetwork.mainText = networkName
    }

    function setToken(tokenName)
    {
        btnChooseCell.mainText = tokenName
    }

    function setMaxPrice(tokenCount)
    {
        btnMaxPrice.mainText = tokenCount
    }

    function setMinPrice(tokenCount)
    {
        btnMinPrice.mainText = tokenCount
    }

    function setSetConfirmButtonActive(active)
    {
        btnConfirn.enabled = active
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: root.titleString
        qss: "dialog-title"
    }

    /****************************************//**
     * Top separator
     ********************************************/

    DapQmlRectangle {
        x: loginSepsPlacer.x
        y: loginSepsPlacer.y
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
            qss: "overview-separator-container"
        }
    }

    /****************************************//**
     * Choose wallet
     ********************************************/

    DapQmlRectangle {
        x:      walletPlacer.x
        y:      walletPlacer.y
        width:  walletPlacer.width
        height: walletPlacer.height

        DapQmlButton {
            id: btnChooseWallet
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("WALLET") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: root.link
            onClicked: root.walletClicked()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: walletPlacer
            qss: "overview-btn-wallet-container"
        }
    }


    /****************************************//**
     * Choose network
     ********************************************/

    DapQmlRectangle {
        x:      networkPlacer.x
        y:      networkPlacer.y
        width:  networkPlacer.width
        height: networkPlacer.height

        DapQmlButton {
            id: btnChooseNetwork
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("NETWORK") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: root.link
            onClicked: root.networkClicked()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: networkPlacer
            qss: "overview-btn-network-container"
        }
    }

    /****************************************//**
     * Choose token
     ********************************************/

    DapQmlRectangle {
        x:      cellPlacer.x
        y:      cellPlacer.y
        width:  cellPlacer.width
        height: cellPlacer.height

        DapQmlButton {
            id: btnChooseCell
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("TOKEN") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: root.link
            onClicked: root.tokenClicked()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: cellPlacer
            qss: "overview-btn-cell-container"
        }
    }

    /****************************************//**
     * Choose unit
     ********************************************/

    DapQmlRectangle {
        x:      minpricePlacer.x
        y:      minpricePlacer.y
        width:  minpricePlacer.width
        height: minpricePlacer.height
        visible: root.cellEnabled

        DapQmlButton {
            id: btnMinPrice
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("MIN PRICE") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: root.link
            onClicked: root.cellClicked()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: minpricePlacer
            qss: "overview-btn-maxprice-container"
        }
    }

    /****************************************//**
     * max price
     ********************************************/

    DapQmlRectangle {
        x:      maxpricePlacer.x
        y:      maxpricePlacer.y
        width:  maxpricePlacer.width
        height: maxpricePlacer.height
        visible: root.cellEnabled

        DapQmlButton {
            id: btnMaxPrice
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("MAX PRICE") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: root.link
            onClicked: root.cellClicked()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: maxpricePlacer
            qss: "overview-btn-maxprice-container"
        }
    }

    /****************************************//**
     * min price
     ********************************************/

    DapQmlRectangle {
        x:      minpricePlacer.x
        y:      minpricePlacer.y
        width:  minpricePlacer.width
        height: minpricePlacer.height
        visible: root.cellEnabled

        DapQmlButton {
            id: btnMinPrice
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74
            property string defaultServerName: qsTr("Auto select") + lang.notifier

            buttonStyle: DapQmlButton.Style.TopMainBottomSub
            mainText: (!internal.changedServer) ? (defaultServerName) : (internal.serverName)
            subText: qsTr("MIN PRICE") + lang.notifier
            qss: "login-btn-server"
            mainQss: "login-btn-main"
            subQss: "login-btn-sub"
            separator: true
            link: root.link
            onClicked: root.cellClicked()

            function updateServerName() {
                mainText = (!internal.changedServer)
                        ? (defaultServerName)
                        : (internal.serverName)
            }

            onDefaultServerNameChanged: updateServerName()
        }
        DapQmlDummy {
            id: minpricePlacer
            qss: "overview-btn-maxprice-container"
        }
    }


    /****************************************//**
     * Connect button
     ********************************************/

    DapQmlPushButton {
        id: btnConfirn
        x: (parent.width - width) / 2
        z: 15
        qss: "overview-btn-confirm"
        text: qsTr("CONFIRM") + lang.notifier
        onClicked: root.buttonClicked()
        enabled: buttonActive
    }

}
