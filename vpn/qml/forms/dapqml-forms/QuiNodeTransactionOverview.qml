/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Transaction Overview Form
 * @ingroup groupDapQmlForms
 * @date 23.08.23
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeTransactionOverview"

    property QtObject internal: QtObject {
        /* VARIABLES */
        property string network:    "TestNetworkName"
        property string wallet:     "TestWalletName"
        property string server:     "TestServerName"
        property string unit:       "TestUnitName"
        property string price:      "TestPriceName"
        property string priceShort: "TESTC"
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigConfirm();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setData(a_data) {
        root.internal.network       = a_data.network;
        root.internal.wallet        = a_data.wallet;
        root.internal.server        = a_data.server;
        root.internal.unit          = a_data.units;
        root.internal.price         = a_data.price;
        root.internal.priceShort    = a_data.priceShort;
    }

    /// @}
    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlDummy {
        id: linkImageSizer
        qss: "btn-arrow nodeorlist-item-link"
        property string scaledPixmap
    }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: compOverviewItem

        //property string first
        //property string second

        RowLayout {
            anchors.fill: parent

            DapQmlLabel {
                Layout.preferredWidth: overviewSizer.width
                Layout.fillHeight: true
                horizontalAlign: Text.AlignLeft
                qss: "nodeorlist-overview-item-left"
                text: parent.parent.first
            }

            DapQmlLabel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                horizontalAlign: Text.AlignLeft
                qss: "nodeorlist-overview-item-right"
                text: parent.parent.second
            }
        }
    }

    /****************************************//**
     * Content
     ********************************************/

    Item {
        width: root.width
        height: root.height

        DapQmlRectangle {
            qss: "nodeorlist-overview-container"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: linkImageSizer.width

                Loader {
                    property string first:  qsTr("Network")
                    property string second: root.internal.network
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    sourceComponent: compOverviewItem
                }

                Loader {
                    property string first:  qsTr("Wallet")
                    property string second: root.internal.wallet
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    sourceComponent: compOverviewItem
                }

                Loader {
                    property string first:  qsTr("Server")
                    property string second: root.internal.server
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    sourceComponent: compOverviewItem
                }

                Loader {
                    property string first:  qsTr("Unit")
                    property string second: root.internal.unit
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    sourceComponent: compOverviewItem
                }

                DapQmlSeparator {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 2
                }

                DapQmlLabel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.topMargin: linkImageSizer.width * 0.05
                    qss: "nodeorlist-overview-price"
                    text: `${root.internal.priceShort} ${root.internal.unit}`
                }
            }
        }

        DapQmlPushButton {
            qss: "nodeorlist-overview-confirm-btn"
            text: qsTr("CONFIRM PURCHASE")
            onClicked: root.sigConfirm()
        }
    } // Transaction Overview
}

/*-----------------------------------------*/
