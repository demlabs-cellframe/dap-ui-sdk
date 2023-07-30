/* INCLUDES */

import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import DapQmlStyle 1.0
import Brand 1.0
import DapQmlModelOrderList 1.0
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

    enum Mode {
        Invalid,

        Orders,
        Networks,
        Wallets,
        Tokens,

        Units,
        MaxPrice
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

    property var interfaceObject

    property QtObject internal: QtObject {
        /* VARIABLES */
        property bool showConfirmButton: false  /// show bottom confirm button
        property bool showValueEdit: false      /// shows line edit
        property bool isEditingUnit: false      /// shows unit edit bottom tabs
        property bool isTime: false             /// tab "TIME" is selected
        property bool isPrice: false            /// price editing

        /* MODES */
        property var mode: QuiNodeOrderList.Invalid
        property var modeSettings: [
            0,0,0,DapQmlModelOrderList.Invalid,     // Invalid

            0,0,0,DapQmlModelOrderList.Orders,      // Orders
            1,0,0,DapQmlModelOrderList.Networks,    // Networks
            1,0,0,DapQmlModelOrderList.Wallets,     // Wallets
            1,0,0,DapQmlModelOrderList.Tokens,      // Tokens

            1,1,1,DapQmlModelOrderList.Invalid,     // Unit
            1,1,0,DapQmlModelOrderList.Invalid,     // Max Price
        ]

        /* LABELS */
        property string network:    "TestNetworkName"
        property string wallet:     "TestWalletName"
        property string server:     "TestServerName"
        property string token:      "TestTokenName"
        property string unit:       "TestUnitName"
        property string price:      "TestPriceName"
        property string priceShort: "TESTC"
        property string maxPrice:   "TestMaxPriceName"

        /* INTERNAL SIGNALS */
        onModeChanged: {
            console.log (`internal mode: ${mode}`)
            showConfirmButton       = modeSettings[mode*4+0];
            showValueEdit           = modeSettings[mode*4+1];
            isEditingUnit           = modeSettings[mode*4+2];
            let listviewMode        = modeSettings[mode*4+3];
            isPrice                 = mode === QuiNodeOrderList.MaxPrice;

            if (csListView.model)
                csListView.model.setMode (listviewMode);
            valueEditInput.text     = "0";
        }
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
        setInternalMode(QuiNodeOrderList.Networks);
        //csListView.model.setMode(DapQmlModelOrderList.Networks);
        swipe.incrementCurrentIndex();
    }

    onSigWalletClicked: {
        setInternalMode(QuiNodeOrderList.Wallets);
        //csListView.model.setMode(DapQmlModelOrderList.Wallets);
        swipe.incrementCurrentIndex();
    }

    onSigTokenClicked: {
        setInternalMode(QuiNodeOrderList.Tokens);
        //csListView.model.setMode(DapQmlModelOrderList.Tokens);
        swipe.incrementCurrentIndex();
    }

    onSigUnitClicked: {
        setInternalMode(QuiNodeOrderList.Units);
        swipe.incrementCurrentIndex();
    }

    onSigMaxPriceClicked: {
        setInternalMode(QuiNodeOrderList.MaxPrice);
        swipe.incrementCurrentIndex();
    }

    onSigSearchClicked: {
        setInternalMode(QuiNodeOrderList.Orders);
        //csListView.model.setMode(DapQmlModelOrderList.Orders);
        swipe.incrementCurrentIndex();
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setInternalMode(a_mode) {
        root.internal.mode  = a_mode;
    }

    function tabPressed(a_tabIndex) {
        root.internal.isTime = a_tabIndex;
    }

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

    /* sizer */
    DapQmlDummy {
        id: tabButtonSizer
        qss: "nodeorlist-tab-btn-size"
    }

    DapQmlDummy {
        id: nameValueRadio
        qss: "radiobtn-resizer"
    }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: tabButton

        // property bool isSecond
        // property string name
        // property var callback

        DapQmlRectangle {
            anchors.fill: parent
            qss: (parent.isSecond !== root.internal.isTime)
                 ? "nodeorlist-tab-btn"
                 : "nodeorlist-tab-btn nodeorlist-tab-btn-selected"

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                qss: (parent.parent.isSecond !== root.internal.isTime)
                     ? "nodeorlist-tab-btn-label"
                     : "nodeorlist-tab-btn-label-second"
                disableClicking: true
                text: parent.parent.name // "TRAFFIC"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: parent.parent.callback()
            }
        }
    }

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
        id: listviewDelegateOrder

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

//            Component.onCompleted: StyleDebugTree.describe (
//               "listviewDelegateOrder" + model.index,
//                ["x", "y", "width", "height", "first", "second"],
//               this);
        }
    }

    Component {
        id: listviewDelegateNameValue

        DapQmlRadioButton {
            width: nameValueRadio.width
            height: nameValueRadio.height
            iconSize: nameValueRadio.height
            separator: true
            qss: "nodeorlist-name-value"
            checked: csListView.model.currentIndex === model.index
            text: model.name

            DapQmlLabel {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: parent.iconSize * 0.325
                width: contentWidth
                height: contentHeight
                disableClicking: true
                text: model.value
                qss: "nodeorlist-name-value c-grey"
            }

            onClicked: csListView.model.currentIndex = model.index;
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
                }
            }
        }

        /****************************************//**
         * Middle Page
         ********************************************/

        Item {
            width: root.width
            height: root.height

            /****************************************//**
             * Value Edit
             ********************************************/

            Item {
                anchors.fill: parent
                visible: root.internal.showValueEdit

                /* VALUE EDIT */

                DapQmlRectangle {
                    id: valueEditRect
                    qss: "nodeorlist-edit"
                    property real fontSize

                    TextInput {
                        id: valueEditInput
                        anchors.fill: parent
                        anchors.margins: parent.height * 0.1
                        horizontalAlignment: TextInput.AlignLeft //AlignHCenter
                        verticalAlignment: TextInput.AlignVCenter
                        font.pixelSize: parent.fontSize
                        font.family: Brand.fontName()
                        font.weight: Font.Bold
                        selectByMouse: true
                        text: "1234"
                        clip: true

                        onTextEdited: {
                            if (interfaceObject === undefined)
                                return;

                            text    = root.internal.isPrice
                                    ? interfaceObject.fixPriceString(text)
                                    : interfaceObject.fixNumberString(text);
                        }
                    }
                }

                /* RESULT */
                DapQmlLabel {
                    anchors.top: valueEditRect.bottom
                    anchors.horizontalCenter: valueEditRect.horizontalCenter
                    anchors.margins: height/2
                    width: valueEditRect.width
                    height: valueEditRect.height * 0.23
                    horizontalAlign: root.internal.isPrice ? Text.AlignRight : Text.AlignHCenter
                    verticalAlign: Text.AlignVCenter
                    elide: Text.ElideMiddle
                    qss: "c-grey"

                    text: root.internal.isPrice
                          ? `Balance: ${valueEditInput.text} ${root.internal.token}`
                          : `${valueEditInput.text} ${root.internal.isTime ? "days" : "MB"}`
                }

                /* TABS */
                RowLayout {
                    id: tabsLayout
                    spacing: tabsLayoutSpacing.width
                    visible: !root.internal.isPrice

                    DapQmlStyle { item: tabsLayout; qss: "nodeorlist-tabs" }
                    DapQmlDummy { id: tabsLayoutSpacing; qss: "nodeorlist-tabs-spacing" }

//                    Component.onCompleted: StyleDebugTree.describe (
//                       "tabsLayout",
//                        ["x", "y", "width", "height", "spacing"],
//                       this);

                    /* left button */
                    Loader {
                        Layout.preferredWidth: tabButtonSizer.width
                        Layout.preferredHeight: tabButtonSizer.height
                        property string name: qsTr("TRAFFIC")
                        property bool isSecond: false
                        property var callback: function() {tabPressed(isSecond)}
                        sourceComponent: tabButton

//                        Component.onCompleted: StyleDebugTree.describe (
//                           "leftTab",
//                            ["x", "y", "width", "height", "name"],
//                           this);
                    }

                    /* right button */
                    Loader {
                        Layout.preferredWidth: tabButtonSizer.width
                        Layout.preferredHeight: tabButtonSizer.height
                        property string name: qsTr("TIME")
                        property bool isSecond: true
                        property var callback: function() {tabPressed(isSecond)}
                        sourceComponent: tabButton

//                        Component.onCompleted: StyleDebugTree.describe (
//                           "rightTab",
//                            ["x", "y", "width", "height", "name"],
//                           this);
                    }
                }
            }

            /****************************************//**
             * Listview
             ********************************************/

            ListView {
                id: csListView
                objectName: "listview"
                x: (root.width - width) / 2
                spacing: spacer.height
                visible: !root.internal.showValueEdit
                clip: true

                onModelChanged: {
                    if (model)
                        csListViewConn.target   = model;
                }

                Connections {
                    id: csListViewConn
                    //target: csListView.model
                    target: csListViewConnDummy
                    function onModeChanged() {
                        console.log (`listview mode: ${target.mode}`)
                        if (target.mode === undefined)
                        {
                            csListView.delegate = listviewDelegateNull;
                            return;
                        }

                        switch(target.mode)
                        {
                        case DapQmlModelOrderList.Networks:    csListView.delegate = listviewDelegateNameValue; break;
                        case DapQmlModelOrderList.Wallets:     csListView.delegate = listviewDelegateNameValue; break;
                        case DapQmlModelOrderList.Tokens:      csListView.delegate = listviewDelegateNameValue; break;
                        case DapQmlModelOrderList.Orders:      csListView.delegate = listviewDelegateOrder;     break;
                        }
                    }
                }

                Item {
                    id: csListViewConnDummy
                    property var mode
                }

                DapQmlStyle { item: csListView; qss: "nodeorlist-listview" }

                Component.onCompleted: StyleDebugTree.describe (
                   "csListView",
                    ["x", "y", "width", "height"],
                   this);
            }

            DapQmlPushButton {
                visible: true // root.internal.showConfirmButton
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
