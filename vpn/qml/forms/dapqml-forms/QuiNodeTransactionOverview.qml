/* INCLUDES */

import QtQuick 2.11
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.4
import DapQmlStyle 1.0
import Brand 1.0
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

        property int mode: 0
        property string txtOverview:    qsTr("Transaction overview")
        property string txtProcessing:  qsTr("Transaction processing...")
        property string txtReciept:     qsTr("Transaction reciept")

        /* mode map */
        property var modeSettings: [
            txtOverview,
            txtProcessing,
            txtReciept,
        ]

        onModeChanged: {
            labelTitle.text  = modeSettings[mode * 1];
        }
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigConfirm();
    signal sigCancel();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setData(a_data) {
        root.internal.network       = a_data.network;
        root.internal.wallet        = a_data.wallet;
        root.internal.server        = a_data.server;
        root.internal.unit          = a_data.unit;
        root.internal.price         = a_data.price;
        root.internal.priceShort    = a_data.priceShort;
    }

    function showOverview() {
        root.internal.mode  = 0;
    }

    function showProcessing() {
        root.internal.mode  = 1;
    }

    function showReciept() {
        root.internal.mode  = 2;
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

    DapQmlDummy {
        id: overviewSizer
        qss: "nodeorlist-overview-item-left-size"
        property int fontSize
    }

    DapQmlDummy {
        id: pushButtonSizer
        qss: "nodeorlist-overview-btn-sizer"
        property int fontSize
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
    Component {
        id: pushButton

        // property string text
        // property color color
        // property var cbClicked

        DapQmlRectangle {
            anchors.fill: parent
            color: parent.color
            radius: 400
            opacity: 0.5 + enabled * 0.5

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                fontFamiliy: Brand.fontName()
                color: "#ffffff"
                fontSize: pushButtonSizer.fontSize
                fontWeight: Font.Bold
                text: parent.parent.text
            }

            MouseArea {
                anchors.fill: parent
                enabled: parent.enabled
                onClicked: parent.parent.cbClicked()
            }
        }
    }

    /****************************************//**
     * Content
     ********************************************/

    DapQmlRectangle {
        anchors.bottom: parent.bottom
        radius: 25
        qss: "nodeorlist-overview-frame c-background"

        /* title */

        DapQmlLabel {
            id: labelTitle
            text: root.internal.txtOverview
            qss: "nodeorderlist-agreement-title"
        }

        /* overview */

        DapQmlRectangle {
            id: overview
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
                    text: root.internal.priceShort
                }
            } // ColumnLayout
        } // Container

        /* loading animation */

        DapQmlRectangle {
            anchors.top: overview.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            z: 60
            qss: "nodeorlist-spinner-bg"
            visible: root.internal.mode === 1

            DapQmlRectangle {
                id: progressCircle
                anchors.centerIn: parent
                qss: "nodeorlist-spinner-arc"

                property string color
                property int strokeWidth: 10

                Shape {
                    id: nodeorInfoArcAnim
                    anchors.fill: parent
                    layer.enabled: true
                    layer.samples: 6

                    ShapePath {
                        fillColor: "transparent"
                        strokeColor: progressCircle.color
                        strokeWidth: progressCircle.strokeWidth
                        capStyle: ShapePath.FlatCap

                        PathAngleArc {
                            id: loginInfoArcPath
                            centerX: nodeorInfoArcAnim.width / 2
                            centerY: nodeorInfoArcAnim.height / 2
                            radiusX: nodeorInfoArcAnim.width / 2 - progressCircle.strokeWidth / 2
                            radiusY: nodeorInfoArcAnim.height / 2 - progressCircle.strokeWidth / 2
                            startAngle: 90
                            sweepAngle: 180

                            NumberAnimation on startAngle {
                                from: 0
                                to: 360
                                running: true
                                loops: Animation.Infinite
                                duration: 2000
                            }
                        }
                    }
                }
            }
        }

        /* user interact */

        ColumnLayout {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: pushButtonSizer.height * 1.25
            width: pushButtonSizer.width
            height: pushButtonSizer.height * 2.125

            Loader {
                enabled: root.internal.mode !== 1
                Layout.fillWidth: true
                Layout.preferredHeight: pushButtonSizer.height
                sourceComponent: pushButton
                property string text:
                    root.internal.mode === 0
                    ? qsTr("CONFIRM PURCHASE")
                    : qsTr("SIGN PURCHASE")
                property color color: "#F45480"
                property var cbClicked: function() {
                    root.sigConfirm();
                    // console.log("confirm clicked");
                }
            }

            Loader {
                //enabled: root.internal.mode !== 1
                Layout.fillWidth: true
                Layout.preferredHeight: pushButtonSizer.height
                sourceComponent: pushButton
                property string text: qsTr("CANCEL")
                property color color: "#A9A9B0"
                property var cbClicked: function() {
                    root.sigCancel();
                    // console.log("cancel clicked");
                }
            }
        } // ColumnLayout
    } // Transaction Overview
}

/*-----------------------------------------*/
