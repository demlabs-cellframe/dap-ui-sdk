/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import DapQmlStyle 1.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Order Form
 * @ingroup groupDapQmlForms
 * @date 20.07.23
 * @author Stanislav
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
    property string formName: "NodeOrderList"

    property QtObject internal: QtObject {
        property string network
        property string wallet
        property string server
        property string unit
        property string price
        property string priceShort
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    DapQmlDummy {
        id: linkImageSizer
        qss: "btn-arrow nodeorlist-item-link"
        property string scaledPixmap
    }

    DapQmlDummy {
        id: listviewItemSizer
        qss: "nodeorlist-item-label"
    }

    DapQmlDummy {
        id: overviewSizer
        qss: "nodeorlist-overview-item-left-size"
        property int fontSize
    }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: listviewDelegate

        DapQmlRectangle {
            width: resizer.width// - 100
            height: resizer.height + spacer.height

            Component.onCompleted: StyleDebugTree.describe (
               "listviewDelegate" + model.index,
                ["x", "y", "width", "height"],
               this);

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.internal.network       = model.network;
                    root.internal.wallet        = model.wallet;
                    root.internal.server        = model.server;
                    root.internal.unit          = model.units;
                    root.internal.price         = model.price;
                    root.internal.priceShort    = model.priceShort;

                    swipe.incrementCurrentIndex();
                }
            }

            DapQmlSeparator {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 2
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: listviewItemSizer.x
                anchors.rightMargin: listviewItemSizer.x

                Component.onCompleted: StyleDebugTree.describe (
                   "listviewDelegate rowlay" + model.index,
                    ["x", "y", "width", "height"],
                   this);

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Component.onCompleted: StyleDebugTree.describe (
                       "listviewDelegate collay" + model.index,
                        ["x", "y", "width", "height"],
                       this);

                    DapQmlLabel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: Text.AlignBottom
                        elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "nodeorlist-item-label-top"
                        text: `${model.price} per ${model.units}`
                    }

                    DapQmlLabel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: Text.AlignTop
                        elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "nodeorlist-item-label-bottom"
                        text: model.server
                    }
                }

                DapQmlImage {
                    id: linkImage
                    Layout.preferredWidth: linkImageSizer.width
                    Layout.preferredHeight: linkImageSizer.height
                    scaledPixmap: linkImageSizer.scaledPixmap

                    Component.onCompleted: StyleDebugTree.describe (
                       "listviewDelegate link" + model.index,
                        ["x", "y", "width", "height"],
                       this);
                }
            }
        }
    }

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
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: swipe.currentIndex === 0 ? qsTr("Orders") : qsTr("Transaction overview")
        qss: "dialog-title"
        hideClose: swipe.currentIndex !== 0

        /****************************************//**
         * Back button
         ********************************************/

        DapQmlPushButton {
            qss: "form-title-close-btn"
            visible: swipe.currentIndex !== 0
            onClicked: swipe.decrementCurrentIndex()
        }
    }

    /****************************************//**
     * Content
     ********************************************/

    SwipeView {
        id: swipe
        anchors.fill: parent
        interactive: false

        /****************************************//**
         * Listview
         ********************************************/

        Item {
            width: root.width
            height: root.height

            ListView {
                id: csListView
                objectName: "listview"
                x: (root.width - width) / 2
                clip: true

                delegate: listviewDelegate

                DapQmlStyle { item: csListView; qss: "nodeorlist-listview" }

                Component.onCompleted: StyleDebugTree.describe (
                   "csListView",
                    ["x", "y", "width", "height"],
                   this);
            }
        }

        /****************************************//**
         * Transaction Overview
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
            }
        }
    }

}

/*-----------------------------------------*/
