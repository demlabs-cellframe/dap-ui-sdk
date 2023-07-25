/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
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
     * @name DEFS
     ********************************************/
    /// @{

    enum Tab {
        Search,
        List,
        Overview
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeOrderList"

    property QtObject internal: QtObject {
        property string network:    "TestNetworkName"
        property string wallet:     "TestWalletName"
        property string server:     "TestServerName"
        property string token:      "TestTokenName"
        property string unit:       "TestUnitName"
        property string price:      "TestPriceName"
        property string priceShort: "TESTC"
        property string maxPrice:   "TestMaxPriceName"
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);

    signal sigNetworkClicked();
    signal sigWalletClicked();
    signal sigTokenClicked();
    signal sigUnitClicked();
    signal sigMaxPriceClicked();
    signal sigSearchClicked();

    onSigNetworkClicked: {
        csListView.model.setMode(DapQmlModelOrderList.Mode.Networks);
        swipe.incrementCurrentIndex();
    }

    onSigWalletClicked: {
        csListView.model.setMode(DapQmlModelOrderList.Mode.Wallets);
        swipe.incrementCurrentIndex();
    }

    onSigTokenClicked: {
        csListView.model.setMode(DapQmlModelOrderList.Mode.Tokens);
        swipe.incrementCurrentIndex();
    }

    onSigUnitClicked: {
        // TODO
        swipe.incrementCurrentIndex();
    }

    onSigMaxPriceClicked: {
        // TODO
        swipe.incrementCurrentIndex();
    }

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
        id: compButton

        //property string first
        //property string second
        //property bool swap
        //property var cbOnClicked

        DapQmlRectangle {
            id: itemRoot
            width: resizer.width// - 100
            height: resizer.height

//            Component.onCompleted: StyleDebugTree.describe (
//               "listviewDelegate" + model.index,
//                ["x", "y", "width", "height"],
//               this);

            MouseArea {
                anchors.fill: parent
                onClicked:
                    if (itemRoot.parent.cbOnClicked)
                        itemRoot.parent.cbOnClicked()
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

//                Component.onCompleted: StyleDebugTree.describe (
//                   "listviewDelegate rowlay" + model.index,
//                    ["x", "y", "width", "height"],
//                   this);

                ColumnLayout {
                    id: itemLabels
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    property bool swap: false

//                    Component.onCompleted: StyleDebugTree.describe (
//                       "listviewDelegate collay" + model.index,
//                        ["x", "y", "width", "height"],
//                       this);

                    Component.onCompleted: {
                        if (itemRoot.parent.hasOwnProperty("swap"))
                        {
                            swap    = itemRoot.parent.swap;
                            if (swap)
                            {
                                var item    = children[0];
                                item.parent = root;
                                item.parent = this;
                            }
                        }
                    }

                    DapQmlLabel {
                        id: itemLabelTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: parent.swap ? Text.AlignTop : Text.AlignBottom
                        elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "nodeorlist-item-label-top"
                        text: itemRoot.parent.first // `${model.price} per ${model.units}`
                    }

                    DapQmlLabel {
                        id: itemLabelBottom
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: !parent.swap ? Text.AlignTop : Text.AlignBottom
                        elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "nodeorlist-item-label-bottom"
                        text: itemRoot.parent.second // model.server
                    }
                }

                DapQmlImage {
                    id: linkImage
                    Layout.preferredWidth: linkImageSizer.width
                    Layout.preferredHeight: linkImageSizer.height
                    scaledPixmap: linkImageSizer.scaledPixmap

//                    Component.onCompleted: StyleDebugTree.describe (
//                       "listviewDelegate link" + model.index,
//                        ["x", "y", "width", "height"],
//                       this);
                }
            }
        }
    }

    Component {
        id: listviewDelegateNull

        Item {}
    }

    Component {
        id: listviewDelegate

        Loader {
            width: resizer.width
            height: resizer.height
            sourceComponent: compButton
            property string first:      `${model.price} per ${model.units}`
            property string second:      model.server
            property var cbOnClicked: function() {
                root.internal.network       = model.network;
                root.internal.wallet        = model.wallet;
                root.internal.server        = model.server;
                root.internal.unit          = model.units;
                root.internal.price         = model.price;
                root.internal.priceShort    = model.priceShort;

                swipe.incrementCurrentIndex();
            }

            Component.onCompleted: StyleDebugTree.describe (
               "listviewDelegate" + model.index,
                ["x", "y", "width", "height", "first", "second"],
               this);
        }

//        DapQmlRectangle {
//            width: resizer.width// - 100
//            height: resizer.height + spacer.height

////            Component.onCompleted: StyleDebugTree.describe (
////               "listviewDelegate" + model.index,
////                ["x", "y", "width", "height"],
////               this);

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    root.internal.network       = model.network;
//                    root.internal.wallet        = model.wallet;
//                    root.internal.server        = model.server;
//                    root.internal.unit          = model.units;
//                    root.internal.price         = model.price;
//                    root.internal.priceShort    = model.priceShort;

//                    swipe.incrementCurrentIndex();
//                }
//            }

//            DapQmlSeparator {
//                anchors.bottom: parent.bottom
//                width: parent.width
//                height: 2
//            }

//            RowLayout {
//                anchors.fill: parent
//                anchors.leftMargin: listviewItemSizer.x
//                anchors.rightMargin: listviewItemSizer.x

////                Component.onCompleted: StyleDebugTree.describe (
////                   "listviewDelegate rowlay" + model.index,
////                    ["x", "y", "width", "height"],
////                   this);

//                ColumnLayout {
//                    Layout.fillWidth: true
//                    Layout.fillHeight: true

////                    Component.onCompleted: StyleDebugTree.describe (
////                       "listviewDelegate collay" + model.index,
////                        ["x", "y", "width", "height"],
////                       this);

//                    DapQmlLabel {
//                        Layout.fillWidth: true
//                        Layout.fillHeight: true
//                        horizontalAlign: Text.AlignLeft
//                        verticalAlign: Text.AlignBottom
//                        elide: Text.ElideMiddle
//                        disableClicking: true
//                        qss: "nodeorlist-item-label-top"
//                        text: `${model.price} per ${model.units}`
//                    }

//                    DapQmlLabel {
//                        Layout.fillWidth: true
//                        Layout.fillHeight: true
//                        horizontalAlign: Text.AlignLeft
//                        verticalAlign: Text.AlignTop
//                        elide: Text.ElideMiddle
//                        disableClicking: true
//                        qss: "nodeorlist-item-label-bottom"
//                        text: model.server
//                    }
//                }

//                DapQmlImage {
//                    id: linkImage
//                    Layout.preferredWidth: linkImageSizer.width
//                    Layout.preferredHeight: linkImageSizer.height
//                    scaledPixmap: linkImageSizer.scaledPixmap

//                    Component.onCompleted: StyleDebugTree.describe (
//                       "listviewDelegate link" + model.index,
//                        ["x", "y", "width", "height"],
//                       this);
//                }
//            }
//        }
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
        text: {
            switch(swipe.currentIndex)
            {
            case QuiNodeOrderList.Tab.Search:   return qsTr("Search order");
            case QuiNodeOrderList.Tab.List:     return qsTr("Orders");
            case QuiNodeOrderList.Tab.Overview: return qsTr("Transaction overview");
            }
            return "";
        }
        //text: swipe.currentIndex === QuiNodeOrderList.Tab.List ? qsTr("Orders") : qsTr("Transaction overview")
        qss: "dialog-title"
        hideClose: swipe.currentIndex !== QuiNodeOrderList.Tab.Search

        /****************************************//**
         * Back button
         ********************************************/

        DapQmlPushButton {
            qss: "form-title-close-btn"
            visible: swipe.currentIndex !== QuiNodeOrderList.Tab.Search
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
         * Search Filter
         ********************************************/

        Item {
            id: searchFilter
            width: root.width
            height: root.height

            ColumnLayout {
                id: searchFilterContent
                spacing: spacer.height
                DapQmlStyle { item: searchFilterContent; qss: "nodeorlist-listview" }

                Loader {
                    sourceComponent: compButton
                    property string first:      root.internal.network
                    property string second:     "Network"
                    property bool swap:         true
                    property var cbOnClicked: function() { root.sigNetworkClicked(); }
                }

                Loader {
                    sourceComponent: compButton
                    property string first:      root.internal.wallet
                    property string second:     "Wallet"
                    property bool swap:         true
                    property var cbOnClicked: function() { root.sigWalletClicked(); }
                }

                Loader {
                    sourceComponent: compButton
                    property string first:      root.internal.token
                    property string second:     "Token"
                    property bool swap:         true
                    property var cbOnClicked: function() { root.sigTokenClicked(); }
                }

                Loader {
                    sourceComponent: compButton
                    property string first:      root.internal.unit
                    property string second:     "Unit"
                    property bool swap:         true
                    property var cbOnClicked: function() { root.sigUnitClicked(); }
                }

                Loader {
                    sourceComponent: compButton
                    property string first:      root.internal.maxPrice
                    property string second:     "Max price"
                    property bool swap:         true
                    property var cbOnClicked: function() { root.sigMaxPriceClicked(); }
                }

                Item {
                    Layout.fillHeight: true
                }
            }

            DapQmlPushButton {
                qss: "nodeorlist-overview-confirm-btn"
                text: qsTr("SEARCH ORDER")
                onClicked: {
                    root.sigSearchClicked();
                    swipe.incrementCurrentIndex();
                }
            }
        }

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
                spacing: spacer.height
                clip: true

                property var mode

                state: "idle"
                onModelChanged: state = "modelSet"
                onModeChanged: {
                    console.log (`NEW MODE: ${mode}`)
                    if (mode === undefined)
                        delegate = listviewDelegateNull;

                    switch(mode)
                    {
                    case DapQmlModelOrderList.Mode.Networks:    delegate = listviewDelegateNull;    break;
                    case DapQmlModelOrderList.Mode.Wallets:     delegate = listviewDelegateNull;    break;
                    case DapQmlModelOrderList.Mode.Tokens:      delegate = listviewDelegateNull;    break;
                    case DapQmlModelOrderList.Mode.Orders:      delegate = listviewDelegate;        break;

                    default:
                        return listviewDelegateNull;
                    }
                }

                states: [
                    State {
                        name: "modelSet"
                        PropertyChanges { target: csListView; mode: csListView.mode }
                    }
                ]

                DapQmlStyle { item: csListView; qss: "nodeorlist-listview" }

                Component.onCompleted: StyleDebugTree.describe (
                   "csListView",
                    ["x", "y", "width", "height"],
                   this);
            }

            DapQmlPushButton {
                qss: "nodeorlist-overview-confirm-btn"
                text: qsTr("SEARCH ORDER")
                onClicked: swipe.incrementCurrentIndex()
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
