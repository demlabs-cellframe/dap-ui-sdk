import QtQuick 2.0
import "qrc:/dapqml-widgets"

Item {

    id: root
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "SearchOrders"
    property bool link: true
    property bool buttonActive: true

    signal unitClicked()
    signal maxUnitClicked()
    signal minUnitClicked()
    signal minPriceClicked()
    signal maxPriceClicked()
    signal searchClicked()

    function setUnit(unit)
    {
        btnChooseUnit.mainText = unit
    }

    function setMaxUnit(unitCount)
    {
        btnMaxUnit.mainText = unitCount
    }

    function setMinUnit(unitCount)
    {
        btnMinUnit.mainText = unitCount
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
        text: "Search orders"
        qss: "dialog-title"
    }

    /****************************************//**
     * Choose network
     ********************************************/

//    DapQmlRectangle {
//        x:      networkPlacer.x
//        y:      networkPlacer.y
//        width:  networkPlacer.width
//        height: networkPlacer.height

//        DapQmlButton {
//            id: btnChooseNetwork
//            x: (parent.width - width) / 2
//            z: 15
//            width: parent.width - 74

//            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
//            mainText: qsTr("ALL") + lang.notifier
//            subText: qsTr("NETWORK") + lang.notifier
//            qss: "searchorders-btn-item"
//            mainQss: "searchorders-btn-main"
//            subQss: "searchorders-btn-sub"
//            separator: true
//            link: root.link
//            onClicked: root.networkClicked()
//        }
//        DapQmlDummy {
//            id: networkPlacer
//            qss: "searchorders-btn-network-container"
//        }
//    }

    /****************************************//**
     * Choose cell
     ********************************************/

//    DapQmlRectangle {
//        x:      cellPlacer.x
//        y:      cellPlacer.y
//        width:  cellPlacer.width
//        height: cellPlacer.height

//        DapQmlButton {
//            id: btnChooseCell
//            x: (parent.width - width) / 2
//            z: 15
//            width: parent.width - 74

//            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
//            mainText: qsTr("ALL") + lang.notifier
//            subText: qsTr("TOKEN") + lang.notifier
//            qss: "searchorders-btn-item"
//            mainQss: "searchorders-btn-main"
//            subQss: "searchorders-btn-sub"
//            separator: true
//            link: root.link
//            onClicked: root.tokenClicked()
//        }
//        DapQmlDummy {
//            id: cellPlacer
//            qss: "searchorders-btn-token-container"
//        }
//    }

    /****************************************//**
     * Choose unit
     ********************************************/

    DapQmlRectangle {
        x:      unitPlacer.x
        y:      unitPlacer.y
        width:  unitPlacer.width
        height: unitPlacer.height

        DapQmlButton {
            id: btnChooseUnit
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
            mainText: qsTr("ALL") + lang.notifier
            subText: qsTr("UNIT") + lang.notifier
            qss: "searchorders-btn-item"
            mainQss: "searchorders-btn-main"
            subQss: "searchorders-btn-sub"
            separator: true
            link: root.link
            onClicked: root.unitClicked()
        }
        DapQmlDummy {
            id: unitPlacer
            qss: "searchorders-btn-unit-container"
        }
    }

    /****************************************//**
     * max unit
     ********************************************/

    DapQmlRectangle {
        x:      maxunitPlacer.x
        y:      maxunitPlacer.y
        width:  maxunitPlacer.width
        height: maxunitPlacer.height

        DapQmlButton {
            id: btnMaxUnit
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
            mainText: qsTr("-") + lang.notifier
            subText: qsTr("MAX UNIT") + lang.notifier
            qss: "searchorders-btn-item"
            mainQss: "searchorders-btn-main"
            subQss: "searchorders-btn-sub"
            separator: true
            link: root.link
            onClicked: root.maxUnitClicked()

        }
        DapQmlDummy {
            id: maxunitPlacer
            qss: "searchorders-btn-maxunit-container"
        }
    }

    /****************************************//**
     * min unit
     ********************************************/

    DapQmlRectangle {
        x:      minunitPlacer.x
        y:      minunitPlacer.y
        width:  minunitPlacer.width
        height: minunitPlacer.height

        DapQmlButton {
            id: btnMinUnit
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
            mainText: qsTr("-") + lang.notifier
            subText: qsTr("MIN UNIT") + lang.notifier
            qss: "searchorders-btn-item"
            mainQss: "searchorders-btn-main"
            subQss: "searchorders-btn-sub"
            separator: true
            link: root.link
            onClicked: root.minUnitClicked()
        }
        DapQmlDummy {
            id: minunitPlacer
            qss: "searchorders-btn-minunit-container"
        }
    }

    /****************************************//**
     * max price cell
     ********************************************/

    DapQmlRectangle {
        x:      maxpricePlacer.x
        y:      maxpricePlacer.y
        width:  maxpricePlacer.width
        height: maxpricePlacer.height

        DapQmlButton {
            id: btnMaxPrice
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
            mainText: qsTr("-") + lang.notifier
            subText: qsTr("MAX PRICE TOKEN") + lang.notifier
            qss: "searchorders-btn-item"
            mainQss: "searchorders-btn-main"
            subQss: "searchorders-btn-sub"
            separator: true
            link: root.link
            onClicked: root.maxPriceClicked()

        }
        DapQmlDummy {
            id: maxpricePlacer
            qss: "searchorders-btn-maxprice-container"
        }
    }

    /****************************************//**
     * min price cell
     ********************************************/

    DapQmlRectangle {
        x:      minpricePlacer.x
        y:      minpricePlacer.y
        width:  minpricePlacer.width
        height: minpricePlacer.height

        DapQmlButton {
            id: btnMinPrice
            x: (parent.width - width) / 2
            z: 15
            width: parent.width - 74

            buttonStyle: DapQmlButton.Style.LeftTopSubBottomMain
            mainText: qsTr("-") + lang.notifier
            subText: qsTr("MIN PRICE TOKEN") + lang.notifier
            qss: "searchorders-btn-item"
            mainQss: "searchorders-btn-main"
            subQss: "searchorders-btn-sub"
            separator: true
            link: root.link
            onClicked: root.minPriceClicked()
        }
        DapQmlDummy {
            id: minpricePlacer
            qss: "searchorders-btn-minprice-container"
        }
    }


    /****************************************//**
     * Connect button
     ********************************************/

    DapQmlPushButton {
        id: btnConfirn
        x: (parent.width - width) / 2
        z: 15
        qss: "overview-btn-confirm push-button"
        text: qsTr("CONFIRM") + lang.notifier
        onClicked: root.searchClicked()
        enabled: buttonActive
    }

}
