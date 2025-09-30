/* INCLUDES */

import QtQuick 2.11
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Shapes 1.4
import DapQmlStyle 1.0
import Brand 1.0
import PageCtl 1.0
import NoCdbCtl 1.0
import DapQmlModelNodeOrderList 1.0
import DapQmlModelNodeProxyBase 1.0
import StyleDebugTree 1.0
import com.DapQmlWidgets 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Order Form
 * @ingroup groupDapQmlForms
 * @date 20.07.23
 * @author Mikhail Shilenko
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
        MaxUnit,
        MinUnit,
        MaxPrice,
        MinPrice,

        TokenValue
    }

    enum EditFieldType
    {
      EdtMaxUnit,
      EdtMinUnit,
      EdtMaxPrice,
      EdtMinPrice
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "NodeOrderList"

    property var interfaceObject

    property QtObject internal: QtObject {
        /* VARIABLES */
        property bool popup: false

        property bool showConfirmButton: false  /// show bottom confirm button
        property bool showValueEdit: false      /// shows line edit
        property bool isEditingUnit: false      /// shows unit edit bottom tabs
        property bool isTime: false             /// tab "TIME" is selected
        property bool isPrice: false            /// price editing
        property bool isSearch: false           /// show search order options instead of choose wallet options
        property string listTitle: "Orders"     /// title text on top of list
        property string editValue: "0"          /// edit field value string
        property bool isFilterSetup: false      /// light up "confirm" button

        /* MODES */
        property var mode: QuiNodeOrderList.Invalid

        /// list format:
        /// showConfirmButton,showValueEdit,isEditingUnit,listviewMode
        property var modeSettings: [
            0,0,0,  // Invalid

            0,0,0,  // Orders

            0,0,0,  // Networks
            0,0,0,  // Wallets
            0,0,0,  // Tokens

            0,0,0,  // Units
            1,1,0,  // Max Unit
            1,1,0,  // Min Unit
            1,1,0,  // Max Price
            1,1,0,  // Min Price

            1,1,0,  // Token Value
        ]

        /* LABELS */
        property string network:    "TestNetworkName"
        property string wallet:     "TestWalletName"
        property string server:     "TestServerName"
        property string token:      "TestTokenName"
        property string tokenValue: "TestTokenValue"
        property string unit:       "TestUnitName"
        property string price:      "TestPriceName"
        property string priceShort: "TESTC"
        property string maxPrice:   "TestMaxPriceName"
        property string minPrice:   "TestMinPriceName"
        property string maxUnit:    "TestMaxUnitName"
        property string minUnit:    "TestMinUnitName"
        property int    chosenOrderIndex: -1
        property string chosenOrderHash

        /* INTERNAL SIGNALS */
        onModeChanged: {
            console.log (`internal mode: ${mode}`)
            showConfirmButton       = modeSettings[mode*3+0];
            showValueEdit           = modeSettings[mode*3+1];
            isEditingUnit           = modeSettings[mode*3+2];
//            let listviewMode        = modeSettings[mode*4+3];
            isPrice                 = mode === QuiNodeOrderList.MaxPrice || mode === QuiNodeOrderList.MinPrice;

//            if (csListView.model)
//                csListView.model.setMode (listviewMode);
            valueEditInput.text     = "0";

            /* change title */
            switch(mode)
            {
            case QuiNodeOrderList.Invalid:  listTitle   = qsTr(""); break;

            case QuiNodeOrderList.Orders:   listTitle   = qsTr("Orders"); break;

            case QuiNodeOrderList.Networks: listTitle   = qsTr("Network"); break;
            case QuiNodeOrderList.Wallets:  listTitle   = qsTr("Wallet"); break;
            case QuiNodeOrderList.Tokens:   listTitle   = qsTr("Token"); break;

            case QuiNodeOrderList.Units:    listTitle   = qsTr("Unit"); break;
            case QuiNodeOrderList.MaxUnit:  listTitle   = qsTr("Max Unit"); break;
            case QuiNodeOrderList.MinUnit:  listTitle   = qsTr("Min Unit"); break;
            case QuiNodeOrderList.MaxPrice: listTitle   = qsTr("Max Price"); break;
            case QuiNodeOrderList.MinPrice: listTitle   = qsTr("Min Price"); break;

            case QuiNodeOrderList.TokenValue: listTitle = qsTr("Max amount to pay"); break;
            }

            /* set value */
            if (interfaceObject !== undefined)
            {
                switch (mode)
                {
                case QuiNodeOrderList.MaxUnit:  editValue   = maxUnit; break;
                case QuiNodeOrderList.MinUnit:  editValue   = minUnit; break;
                case QuiNodeOrderList.MaxPrice: editValue   = maxPrice; break;
                case QuiNodeOrderList.MinPrice: editValue   = minPrice; break;

                case QuiNodeOrderList.TokenValue: editValue = tokenValue; break;
                }
                valueEditInput.text = (editValue === "") ? "0" : editValue;
            }
            _updateFilterSetupValue();
        }
    }

    Timer {
        id: errorMessageTimer
        interval: 7000
        running: false
        repeat: false
        onTriggered: hideErrorMessage()
    }

    Timer {
        id: orderListWaitTimer
        interval: 10000
        running: false
        repeat: false
        onTriggered: hideSpinner()
    }

    Timer {
        id: updateFilterSetupTimer
        interval: 250
        running: false
        repeat: false
        onTriggered: _updateFilterSetupValue()
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief order selected
    signal sigOrderSelect(int a_index, string a_hash);

    signal sigNetworkClicked();
    signal sigWalletClicked();
    signal sigTokenClicked();
    signal sigTokenValueClicked();
    signal sigUnitClicked();
    signal sigMaxUnitClicked();
    signal sigMinUnitClicked();
    signal sigMaxPriceClicked();
    signal sigMinPriceClicked();

    signal sigRefreshDataClicked();
    signal sigSearchClicked();
    signal sigWalletConfirmClicked();

    signal sigUpdateValuesFromModel();

    /* CHOOSE WALLET */

    onSigNetworkClicked: {
        setInternalMode(QuiNodeOrderList.Networks);
        swipe.incrementCurrentIndex();
    }

    onSigWalletClicked: {
        setInternalMode(QuiNodeOrderList.Wallets);
        swipe.incrementCurrentIndex();
    }

    onSigTokenClicked: {
        setInternalMode(QuiNodeOrderList.Tokens);
        swipe.incrementCurrentIndex();
    }

    onSigTokenValueClicked: {
        setInternalMode(QuiNodeOrderList.TokenValue);
        swipe.incrementCurrentIndex();
    }

    onSigSearchClicked: {
        setInternalMode(QuiNodeOrderList.Orders);
        swipe.incrementCurrentIndex();
    }

    /* SEARCH ORDER */

    onSigUnitClicked: {
        setInternalMode(QuiNodeOrderList.Units);
        swipe.incrementCurrentIndex();
    }

    onSigMaxUnitClicked: {
        setInternalMode(QuiNodeOrderList.MaxUnit);
        swipe.incrementCurrentIndex();
    }

    onSigMinUnitClicked: {
        setInternalMode(QuiNodeOrderList.MinUnit);
        swipe.incrementCurrentIndex();
    }

    onSigMaxPriceClicked: {
        setInternalMode(QuiNodeOrderList.MaxPrice);
        swipe.incrementCurrentIndex();
    }

    onSigMinPriceClicked: {
        setInternalMode(QuiNodeOrderList.MinPrice);
        swipe.incrementCurrentIndex();
    }

    onSigWalletConfirmClicked: {
        PageCtl.slotBackwardAuto();
    }

    /* get variables from interface object */
    onInterfaceObjectChanged: {
        if (interfaceObject === undefined)
            return;

        getDataFromInterface();
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setInternalMode(a_mode) {
        root.internal.mode  = a_mode;
        console.log("Internal mode set to:", root.internal.mode);
    }

    function tabPressed(a_tabIndex) {
        root.internal.isTime = a_tabIndex;
    }

    function filterItemSelected (a_index, a_name) {
        let mode    = internal.mode;

        switch(mode)
        {

        case QuiNodeOrderList.Networks:
            root.internal.network                   = a_name;
            csListViewNetworks.model.currentIndex   = a_index;
            break;

        case QuiNodeOrderList.Wallets:
            if (csListViewWallets.model) {
                /* Show all wallets in the model */
            }
            root.internal.wallet = a_name;
            csListViewWallets.model.currentIndex = a_index;
            break;

        case QuiNodeOrderList.Tokens:
            root.internal.token                     = a_name;
            csListViewTokens.model.currentIndex     = a_index;
            break;

        case QuiNodeOrderList.Units:
            root.internal.unit                      = a_name;
            csListView.model.currentIndex           = a_index;
            break;

        }

        /* clean child fields */
        switch(mode)
        {

        case QuiNodeOrderList.Networks:
            root.internal.wallet    = " ";
            root.internal.token     = " ";
            //csListView.model.onNetworkChange();
            csListViewWallets.model.currentIndex    = -1;
            csListViewTokens.model.currentIndex     = -1;
            interfaceObject.setData(
            {
                network     : root.internal.network,
                //wallet      : root.internal.wallet,
                token       : root.internal.token,
            });
            break;

        case QuiNodeOrderList.Wallets:
            // Check if network is empty - if so, don't clear fields to allow auto-selection
            if (root.internal.network === "" || root.internal.network === " ") {
                // Network is empty, let C++ auto-select it
                interfaceObject.setData({
                    wallet: root.internal.wallet,
                });
                getDataFromInterface();
            } else {
                // Network is already selected, clear child fields
                root.internal.token = " ";
                interfaceObject.setData({
                    wallet: root.internal.wallet,
                });
                getDataFromInterface();
            }
            break;

        }

        _updateFilterSetupValue();

        timerFilterItemSelected.start();
    }

    function filterValueSet (a_value) {
        /* get field type */
        let ft = QuiNodeOrderList.EdtMaxUnit;
        let minValue = 0; let maxValue = 0;
        switch(root.internal.mode)
        {
        case QuiNodeOrderList.MaxUnit:
            ft = QuiNodeOrderList.EdtMaxUnit;
            minValue = root.internal.minUnit;
            maxValue = root.internal.maxUnit;
            break;

        case QuiNodeOrderList.MinUnit:
            ft = QuiNodeOrderList.EdtMinUnit;
            minValue = root.internal.minUnit;
            maxValue = root.internal.maxUnit;
            break;

        case QuiNodeOrderList.MaxPrice:
            ft = QuiNodeOrderList.EdtMaxPrice;
            minValue = root.internal.minPrice;
            maxValue = root.internal.maxPrice;
            break;

        case QuiNodeOrderList.MinPrice:
            ft = QuiNodeOrderList.EdtMinPrice;
            minValue = root.internal.minPrice;
            maxValue = root.internal.maxPrice;
            break;
        }

        /* check if value is valid */
        let message = interfaceObject.isEditFieldValueValid (ft, a_value, minValue, maxValue);
        if (message !== "")
            return message;

        /* store */
        switch (root.internal.mode)
        {
        case QuiNodeOrderList.MaxUnit:  root.internal.maxUnit   = a_value; break;
        case QuiNodeOrderList.MinUnit:  root.internal.minUnit   = a_value; break;
        case QuiNodeOrderList.MaxPrice: root.internal.maxPrice  = a_value; break;
        case QuiNodeOrderList.MinPrice: root.internal.minPrice  = a_value; break;
        case QuiNodeOrderList.TokenValue: root.internal.tokenValue = a_value; break;
        }

        _updateFilterSetupValue();

        return "";
    }

    function storeFilterData() {
        if (!root.internal.isSearch)
        {
            interfaceObject.setData(
            {
                network     : root.internal.network,
                wallet      : root.internal.wallet,
                token       : root.internal.token,
                tokenValue  : root.internal.tokenValue,
            }
            );
        }
        else
        {
            interfaceObject.setData(
            {
                unit        : root.internal.unit,
                maxUnit     : root.internal.maxUnit,
                minUnit     : root.internal.minUnit,
                maxPrice    : root.internal.maxPrice,
                minPrice    : root.internal.minPrice,
            }
            );
        }
        _updateFilterSetupValue();
    }

    function showChooseWallet() {
        root.internal.isSearch  = false;
        _updateFilterSetupValue();
    }

    function showSearchOrder() {
        root.internal.isSearch  = true;
        _updateFilterSetupValue();
    }

    function hideSpinner() {
        root.internal.popup = false;
    }

    function clearEditField() {
        /* clear edit value result */
        switch (root.internal.mode)
        {
        case QuiNodeOrderList.MaxUnit:
        case QuiNodeOrderList.MinUnit:
        case QuiNodeOrderList.MaxPrice:
        case QuiNodeOrderList.MinPrice:
        case QuiNodeOrderList.TokenValue:
            filterValueSet ("");
            break;
        default:
            break;
        }

        /* navigate back */
        //swipe.decrementCurrentIndex();

        /* clear edit field */
        valueEditInput.text         = "";
    }

    function showErrorMessage(a_message) {
        errorMessageItem.y              = root.width * 0.05;
        errorMessageItem.errorMessage   = a_message;
        errorMessageTimer.restart();
    }

    function hideErrorMessage() {
        errorMessageTimer.stop();
        errorMessageItem.y  = 0 - root.width * 0.1;
    }

    function _updateFilterSetupValue() {
        root.internal.isFilterSetup =
            (root.internal.network       !== "" && root.internal.network     !== " ")
            && (root.internal.wallet     !== "" && root.internal.wallet      !== " ")
            && (root.internal.token      !== "" && root.internal.token       !== " ")
            && (root.internal.tokenValue !== "" && root.internal.tokenValue  !== " ")
    }

    function getDataFromInterface() {
        root.internal.network       = interfaceObject.network();
        root.internal.wallet        = interfaceObject.wallet();
        root.internal.token         = interfaceObject.token();
        root.internal.unit          = interfaceObject.unit();
        root.internal.price         = interfaceObject.price();
        root.internal.priceShort    = interfaceObject.priceShort();
        root.internal.maxPrice      = interfaceObject.maxPrice();
        root.internal.minPrice      = interfaceObject.minPrice();
        root.internal.maxUnit       = interfaceObject.maxUnit();
        root.internal.minUnit       = interfaceObject.minUnit();
        root.internal.tokenValue    = interfaceObject.tokenValue();

        _updateFilterSetupValue();
    }

    /// @}
    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlLabel {
        id: dummyLabel
        visible: false
    }

    DapQmlStyle {
        id: dummyStyle
    }

    DapQmlDummy {
        id: pushButtonSizer
        qss: "nodeorlist-overview-btn-sizer"
        property int fontSize
    }

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
        id: delegateOrderLabelSize
        qss: "nodeorlist-label-size-14"
        property int fontSize
    }

    DapQmlDummy {
        id: colorLabel
        qss: "c-label"
        property color color
    }

    DapQmlDummy {
        id: colorLabelGray
        qss: "c-grey"
        property color color
    }

    DapQmlDummy {
        id: colorBackground
        qss: "c-background"
        property color color
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

    DapQmlDummy {
        id: closeButtonSizer
        qss: "nodeorderlist-edit-btn-close"
        property string scaledPixmap
    }

    /****************************************//**
     * Timers
     ********************************************/

    Timer {
        id: timerFilterItemSelected
        interval: 250; running: false; repeat: false
        onTriggered: swipe.decrementCurrentIndex()
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
        id: pushButton

        // property string text
        // property color color
        // property color fill
        // property var cbClicked

        Rectangle {
            anchors.fill: parent
            color: parent.fill
            radius: height * 0.485
            opacity: 0.5 + enabled * 0.5

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                fontFamiliy: Brand.fontName()
                color: parent.parent.color
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

    Component {
        id: compButton

        //property string first
        //property string second
        //property bool swap
        //property var cbOnClicked
        //property string labelTopQss
        //property string labelBottomQss
        //property QtObject tooltip

        DapQmlRectangle {
            id: itemRoot
            width: resizer.width// - 100
            height: resizer.height
            opacity: 0.5 + enabled * 0.5

//            Component.onCompleted: StyleDebugTree.describe (
//               "listviewDelegate" + model.index,
//                ["x", "y", "width", "height"],
//               this);

            property bool loadTooltip: itemRoot.parent.hasOwnProperty("tooltip")
                                       ? itemRoot.parent.tooltip.hasOwnProperty("text")
                                       : false

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
                opacity: 0.25
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
                        //elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "nodeorlist-item-label-top " + itemRoot.parent.labelTopQss
                        text: itemRoot.parent.first // `${model.price} per ${model.units}`

                        MouseArea {
                            anchors.fill: parent
                            onClicked:
                                if (itemRoot.parent.cbOnClicked)
                                    itemRoot.parent.cbOnClicked()
                            hoverEnabled: itemRoot.loadTooltip
                            onEntered: itemPopup.visible = true
                            onExited: itemPopup.visible  = false
                        }

                        ToolTip {
                            id: itemPopup
                            anchors.centerIn: parent
                            width: popupContent.width + delegateOrderLabelSize.fontSize
                            height: popupContent.height + delegateOrderLabelSize.fontSize
                            delay: 600
                            timeout: 15000
                            visible: false

                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0
                            padding: 0
                            margins: 0

                            background: Item {}

                            text:   itemRoot.loadTooltip ? itemRoot.parent.tooltip.text    : ""
                            //property string topText:    parent.loadTooltip ? itemRoot.parent.tooltip.topText    : ""
                            //property string bottomText: parent.loadTooltip ? itemRoot.parent.tooltip.bottomText : ""

                            contentItem: Rectangle {
                                anchors.bottom: parent.top
                                color: colorBackground.color
                                border.color: colorLabel.color

                                ColumnLayout {
                                    id: popupContent
                                    anchors.centerIn: parent
                                    width: popupContentText.contentWidth
                                    //height: popupContentItemLabelTop.contentHeight + popupContentItemLabelBottom.contentHeight
                                    height: popupContentText.contentHeight

                                    DapQmlLabel {
                                        id: popupContentText
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        horizontalAlign: Text.AlignHCenter
                                        verticalAlign: Text.AlignTop
                                        //elide: Text.ElideMiddle
                                        disableClicking: true
                                        qss: "nodeorlist-item-label-top nodeorlist-label-size-14"
                                        text: itemPopup.text
                                    }

            //                        DapQmlLabel {
            //                            id: popupContentItemLabelTop
            //                            Layout.fillWidth: true
            //                            Layout.fillHeight: true
            //                            horizontalAlign: Text.AlignLeft
            //                            verticalAlign: Text.AlignTop
            //                            //elide: Text.ElideMiddle
            //                            disableClicking: true
            //                            qss: "nodeorlist-item-label-top nodeorlist-label-size-14"
            //                            text: itemPopup.topText
            //                        }

            //                        DapQmlLabel {
            //                            id: popupContentItemLabelBottom
            //                            Layout.fillWidth: true
            //                            Layout.fillHeight: true
            //                            horizontalAlign: Text.AlignLeft
            //                            verticalAlign: Text.AlignBottom
            //                            //elide: Text.ElideMiddle
            //                            disableClicking: true
            //                            qss: "nodeorlist-item-label-bottom nodeorlist-label-size-14"
            //                            text: itemPopup.bottomText
            //                        }
                                }
                            }
                        }
                    }

                    DapQmlLabel {
                        id: itemLabelBottom
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: !parent.swap ? Text.AlignTop : Text.AlignBottom
                        //elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "nodeorlist-item-label-bottom " + itemRoot.parent.labelBottomQss
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
            height: enabled ? resizer.height : 0
            //height: resizer.height
            sourceComponent: compButton
            enabled: model.ipAddress !== ""
                     && model.ipAddress !== "0.0.0.0"
                     && model.units_value !== "0"
            clip: true
            property string first:      {
                if (model.units)
                    return dummyStyle.elideOrderPriceText(
                          dummyLabel.fontFamiliy,
                          delegateOrderLabelSize.fontSize,
                          `${model.price} per ${model.units_value} ${model.units}`,
                          resizer.width
                        );
                else
                    return `${model.price}`;
            }
            property string second:      `${model.server} - ${addressValue}`
            property string addressValue: {
                if (model.ipAddress && model.ipAddress !== "") {
                    return model.ipAddress;
                } else {
                    // More informative message instead of generic "unknown ip"
                    return "IP resolving...";
                }
            }
            property string labelTopQss:    "nodeorlist-label-size-14"
            property string labelBottomQss: "nodeorlist-label-size-14"
            property QtObject tooltip: QtObject {
                //property string topText:    model.units ? `${model.price}\nper ${model.units_value} ${model.units}` : `${model.price}`;
                //property string bottomText: `${model.server} \n ${model.ipAddress}`
                property string text: model.price
            }

            property var cbOnClicked: function() {
                root.internal.network       = NoCdbCtl.network;
                root.internal.wallet        = NoCdbCtl.wallet;
                root.internal.server        = model.server;
                //root.internal.unit          = model.units;
                root.internal.price         = model.price;
                root.internal.priceShort    = model.priceShort;

//                root.internal.chosenOrderIndex  = model.index;
//                root.internal.chosenOrderHash   = model.hash;
//                swipe.incrementCurrentIndex();
////                root.sigOrderSelect (root.internal.chosenOrderIndex, root.internal.chosenOrderHash);

                csListViewOrders.model.currentIndex = model.index;
                //csListView.model.currentIndex = model.index;
                root.sigOrderSelect (model.index, model.hash);
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
            checked: {
                switch(root.internal.mode)
                {
                case QuiNodeOrderList.Networks:
                    return csListViewNetworks.model.currentIndex === model.index;
                case QuiNodeOrderList.Wallets:
                    return csListViewWallets.model.currentIndex === model.index;
                case QuiNodeOrderList.Tokens:
                    return csListViewTokens.model.currentIndex === model.index;
//                case QuiNodeOrderList.Units:
//                    return csListView.model.currentIndex === model.index;
                default:    return csListView.model.currentIndex === model.index;
                }
            }
            text: (model.name !== undefined) ? model.name : ""

            DapQmlLabel {
                id: amountLabel
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: marginValue
                width: parent.width - parent.textLabel.contentWidth - marginValue - parent.iconSize * 1.25
                height: contentHeight
                horizontalAlign: Text.AlignRight
                disableClicking: true
                elide: Text.ElideMiddle
                text: (model.value !== undefined)
                      ? model.value
                      : ""
                qss: "nodeorlist-name-value c-grey"

                property real marginValue: parent.iconSize * 0.325

                MouseArea {
                    anchors.fill: parent
                    z: 10
                    hoverEnabled: true
                    onEntered: itemPopup.open()
                    onExited:  itemPopup.close()
                    //onHeightChanged: csListViewItem.collectLogMessage (`mousearea ${x.toFixed(2)},${y.toFixed(2)},${z.toFixed(2)} ${width.toFixed(2)}x${height.toFixed(2)}`)
                }

                Popup {
                    id: itemPopup
                    x: parent.width - width
                    y: 0 - height
                    width: popupLabel.contentWidth * 1.25
                    height: popupLabel.contentHeight * 1.5
                    topInset: 0
                    bottomInset: 0
                    leftInset: 0
                    rightInset: 0
                    padding: 0
                    margins: 0

                    background: Item {}

                    contentItem: Rectangle {
                        anchors.fill: parent
                        color: "#e0e0e0"
                        border.color: "#404040"

                        Text {
                            id: popupLabel
                            anchors.fill: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "#404040"

                            font {
                                family: amountLabel.fontFamiliy
                                pixelSize: amountLabel.fontSize
                            }

                            text: amountLabel.text
                        }
                    }
                }
            }

            onClicked: {
                //csListView.model.currentIndex = model.index;
                root.filterItemSelected (model.index, text);
                //timerFilterItemSelected.start(); // swipe.decrementCurrentIndex();
            }
        }
    }

    /****************************************//**
     * Fast Blur and Loading Animation
     ********************************************/

    /* popup dialog blur effect */
    FastBlur {
        anchors.fill: content
        source: content
        radius: 40
        cached: true
        z: 50
        opacity: 1.0 * root.internal.popup
        Behavior on opacity { PropertyAnimation { duration: 250 } }
    }

    /* loading animation */
    DapQmlRectangle {
        anchors.centerIn: parent
        z: 60
        qss: "nodeorlist-spinner-bg"
        opacity: 1.0 * root.internal.popup
        Behavior on opacity { PropertyAnimation { duration: 250 } }

        DapQmlArcAnimation {
            anchors.centerIn: parent
            strokeWidth: 10
            qss: "nodeorlist-spinner-arc"
        }
    }

    /****************************************//**
     * Error message
     ********************************************/

    Rectangle {
        id: errorMessageItem
        x: (parent.width - width) / 2
        y: 0 - root.width * 0.1
        z: 30
        width: parent.width * 0.85
        height: errorText.contentHeight + parent.width * 0.025
        color: colorBackground.color
        border.color: colorLabelGray.color
        radius: 12

        property string errorMessage

        Behavior on y { PropertyAnimation { duration: 250 } }

        DapQmlLabel {
            id: errorText
            anchors.fill: parent
            horizontalAlign: Text.AlignHCenter
            verticalAlign: Text.AlignVCenter
            //elide: Text.ElideMiddle
            disableClicking: true
            qss: "nodeorlist-item-label-top nodeorlist-label-size-14"
            text: errorMessageItem.errorMessage
        }
    }

    /****************************************//**
     * Content
     ********************************************/

    Item {
        id: content
        anchors.fill: parent
        opacity: 1.0 * (!root.internal.popup)
        Behavior on opacity { PropertyAnimation { duration: 250 } }

        /****************************************//**
         * Popup dialog darkness
         ********************************************/

        Rectangle {
            width: content.width
            height: content.height
            z: 55
            color: "#80000000"
            opacity: 1.0 * root.internal.popup
            Behavior on opacity { PropertyAnimation { duration: 250 } }
        }

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            id: title
            text: {
                switch(swipe.currentIndex)
                {
                case QuiNodeOrderList.Tab.Search:   return qsTr("Wallet Parameters");
                case QuiNodeOrderList.Tab.List:     return root.internal.listTitle; //qsTr("Orders");
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
         * Swipe View
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
                        visible: !root.internal.isSearch
                        property string first:      root.internal.network
                        property string second:     qsTr("Network*")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigNetworkClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: !root.internal.isSearch
                        property string first:      root.internal.wallet
                        property string second:     qsTr("Wallet*")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigWalletClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: false // !root.internal.isSearch
                        property string first:      root.internal.token
                        property string second:     qsTr("Token*")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigTokenClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: !root.internal.isSearch
                        property string first:      root.internal.tokenValue
                        property string second:     qsTr("Max amount to pay*")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigTokenValueClicked(); }

                        Image {
                            x: parent.width - parent.height * 1.325
                            y: Math.floor ((parent.height - height) / 2)
                            width: Math.floor (parent.height * 0.4)
                            height: width
                            mipmap: true
                            smooth: true
                            antialiasing: false
                            source: "qrc:/nonthemed/tooltip-icon.png"

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: tooltipIcnPopup.visible = true
                                onExited: tooltipIcnPopup.visible  = false
                            }

                            ToolTip {
                                id: tooltipIcnPopup
                                width: dummyText.contentWidth + dummyText.contentHeight * 0.625
                                height: dummyText.contentHeight * 1.625
                                delay: 600
                                timeout: 15000
                                visible: false

                                topInset: 0
                                bottomInset: 0
                                leftInset: 0
                                rightInset: 0
                                padding: 0
                                margins: 0

                                background: Item {}

                                contentItem: Rectangle {
                                    anchors.bottom: parent.top
                                    width: parent.width
                                    height: parent.height
                                    color: colorLabel.color
                                    border.color: colorLabelGray.color
                                    radius: height * 0.125

                                    property string tooltipText: qsTr ("Specify how many tokens\nyou are willing to spend to\npay for VPN service")

                                    Text {
                                        id: dummyText
                                        visible: false
                                        font.pixelSize: delegateOrderLabelSize.fontSize
                                        text: parent.tooltipText
                                    }

                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: delegateOrderLabelSize.fontSize
                                        text: parent.tooltipText
                                        color: colorBackground.color
                                    }
                                }
                            }
                        }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.unit
                        property string second:     qsTr("Unit")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigUnitClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.maxUnit
                        property string second:     qsTr("Max Unit")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMaxUnitClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.minUnit
                        property string second:     qsTr("Min Unit")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMinUnitClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.maxPrice
                        property string second:     qsTr("Max Price")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMaxPriceClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.minPrice
                        property string second:     qsTr("Min Price")
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMinPriceClicked(); }
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }

                Loader {
                    id: btnRefreshWallets
                    DapQmlStyle { item: btnRefreshWallets; qss: "nodeorlist-overview-refresh-btn" }
                    visible: !root.internal.isSearch
                    sourceComponent: pushButton
                    property string text: qsTr("REFRESH DATA")
                    property color color
                    property color fill
                    property var cbClicked: function() {
                        root.internal.network       = "";
                        root.internal.wallet        = "";
                        root.internal.token         = "";
                        root.internal.tokenValue    = "";
                        storeFilterData();
                        root.sigRefreshDataClicked();
                        // console.log("refresh clicked");
                        btnRefreshWallets.enabled = false;
                        searchFilterContent.enabled = false;
                        btnRefreshWalletsDisabledTimer.start();
                    }

                    Timer {
                        id: btnRefreshWalletsDisabledTimer
                        running: false
                        repeat: false
                        interval: 2000
                        onTriggered: {
                            root.sigUpdateValuesFromModel();
                            btnRefreshWallets.enabled = true;
                            searchFilterContent.enabled = true;
                            root.getDataFromInterface();
                        }
                    }
                }

                Loader {
                    id: btnConfirmSearch
                    DapQmlStyle { item: btnConfirmSearch; qss: "nodeorlist-overview-confirm-btn" }
                    sourceComponent: pushButton
                    enabled: root.internal.isSearch ? true : root.internal.isFilterSetup
                    property string text: root.internal.isSearch ? qsTr("APPLY PARAMETERS") : qsTr("CONFIRM")
                    property color color
                    property color fill
                    property var cbClicked: function() {
                        storeFilterData();
                        if (root.internal.isSearch)
                        {
                            orderListWaitTimer.restart();
                            root.internal.popup = true;
                            root.sigSearchClicked();
                        }
                        else
                            root.sigWalletConfirmClicked();
                    }
                }

//                DapQmlPushButton {
//                    qss: "nodeorlist-overview-confirm-btn"
//                    text: root.internal.isSearch ? qsTr("SEARCH ORDER") : qsTr("CONFIRM")
//                    enabled: root.internal.isSearch ? true : root.internal.isFilterSetup
//                    onClicked: {
//                        storeFilterData();
//                        if (root.internal.isSearch)
//                        {
//                            orderListWaitTimer.restart();
//                            root.internal.popup = true;
//                            root.sigSearchClicked();
//                        }
//                        else
//                            root.sigWalletConfirmClicked();
//                    }
//                }
            } // Search Filter

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
                        x: valueEditRectScaler.x
                        y: valueEditRectScaler.y
                        width: valueEditRectScaler.width
                        height: valueEditRectScaler.height
                        radius: valueEditRectScaler.radius
                        borderColor: valueEditRectScaler.color
                        borderWidth: valueEditRectScaler.borderWidth
                        property real fontSize
                        property string color

                        DapQmlRectangle {
                            id: valueEditRectScaler
                            visible: false
                            qss: "nodeorlist-edit"
                            onHeightChanged: valueEditRect.autoResize(0)
                        }

                        function autoResize(a_value) {
                            let compareVal  = valueEditRectScaler.width * 0.056179775;
                            let minContent  = (a_value < compareVal) ? compareVal : a_value;
                            height  = valueEditRectScaler.height + minContent;
                        }

                        Component.onCompleted: autoResize(0)

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: parent.height * 0.05

                            TextInput {
                                id: valueEditInput
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                horizontalAlignment: TextInput.AlignRight
                                verticalAlignment: TextInput.AlignVCenter
                                font.pixelSize: valueEditRect.fontSize
                                font.family: Brand.fontName()
                                font.weight: Font.Bold
                                selectByMouse: true
                                text: root.internal.editValue // "1234"
                                wrapMode: Text.Wrap
                                clip: true
                                maximumLength: 77

                                onTextEdited: {
                                    if (interfaceObject === undefined)
                                        return;

                                    text    = root.internal.isPrice
                                            ? interfaceObject.fixPriceString(text)
                                            : interfaceObject.fixNumberString(text);

                                    valueEditRect.autoResize(contentHeight);
                                }

                                onContentHeightChanged: valueEditRect.autoResize(contentHeight);

                                onActiveFocusChanged: {
                                    if (activeFocus)
                                        if (text === "0")
                                            selectAll();
                                }

                                DapQmlStyle { item: valueEditInput; qss: "c-label" }
                            }

                            Item {
                                Layout.preferredWidth: closeButtonSizer.width
                                Layout.fillHeight: true

                                DapQmlLabel {
                                    anchors.centerIn: parent
                                    width: closeButtonSizer.width
                                    height: closeButtonSizer.height
                                    scaledPixmap: closeButtonSizer.scaledPixmap
                                    onClicked: clearEditField()
                                }
                            }
                        }
                    }

                    /* RESULT */
                    DapQmlLabel {
                        id: valueEditRectResult
                        anchors.top: valueEditRect.bottom
                        anchors.horizontalCenter: valueEditRect.horizontalCenter
                        anchors.margins: valueEditRect.height * 0.115
                        width: valueEditRect.width
                        height: {
                            if (root.internal.mode === QuiNodeOrderList.TokenValue)
                                return valueEditRect.height * 0.5
                            return valueEditRect.height * 0.23
                        }
                        horizontalAlign: root.internal.isPrice ? Text.AlignRight : Text.AlignHCenter
                        verticalAlign: Text.AlignVCenter
                        elide: Text.ElideMiddle
                        qss: "c-grey"

//                        text: root.internal.isPrice
//                              ? `Balance: ${valueEditInput.text} ${root.internal.token}`
//                              //: `${valueEditInput.text} ${root.internal.isTime ? "days" : "MB"}`
//                              : `${valueEditInput.text} ${root.internal.unit}`
                        text: {
                            if (root.internal.mode === QuiNodeOrderList.TokenValue)
                            {
                                let value   = interfaceObject.balanceToCoins (valueEditInput.text);
                                if (root.internal.token === "")
                                    return value;
                                return `${value} ${root.internal.token}`;
                            }

                            if (root.internal.isPrice)
                                return `${valueEditInput.text} ${root.internal.token}`;

                            if (root.internal.unit === "All")
                                return "";

                            return `${valueEditInput.text} ${root.internal.unit}`
                        }

                        MouseArea {
                            anchors.fill: parent
                            z: 10
                            hoverEnabled: root.internal.token !== ""
                            onEntered: valueEditRectResultPopup.open()
                            onExited:  valueEditRectResultPopup.close()
                        }

                        Popup {
                            id: valueEditRectResultPopup
                            x: parent.width - width
                            y: 0 - height
                            width: valueEditRectResultPopupText.contentWidth + height
                            height: valueEditRectResultPopupText.contentHeight * 1.5
                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0
                            padding: 0
                            margins: 0

                            background: Item {}

                            contentItem: Rectangle {
                                anchors.fill: parent
                                color: "#e0e0e0"
                                border.color: "#404040"

                                Text {
                                    id: valueEditRectResultPopupText
                                    anchors.fill: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#404040"

                                    font {
                                        family: amountLabel.fontFamiliy
                                        pixelSize: valueEditRectResult.fontSize * 0.85
                                    }

                                    text: valueEditRectResult.text
                                }
                            }
                        }
                    }

                    /* Balance */
                    DapQmlLabel {
                        id: valueEditRectBalance
                        anchors.top: valueEditRectResult.bottom
                        anchors.horizontalCenter: valueEditRectResult.horizontalCenter
                        anchors.margins: valueEditRectResult.height * (0 - 0.125)
                        //anchors.margins: valueEditRectResult.height * 0.115
                        width: valueEditRect.width
                        height: valueEditRect.height * 0.23
                        horizontalAlign: Text.AlignHCenter
                        verticalAlign: Text.AlignVCenter
                        elide: Text.ElideMiddle
                        visible: root.internal.mode === QuiNodeOrderList.TokenValue
                        qss: "c-grey"
                        text: `Balance: ${NoCdbCtl.balance} ${root.internal.token}`

                        MouseArea {
                            anchors.fill: parent
                            z: 10
                            hoverEnabled: true
                            onEntered: valueEditRectBalancePopup.open()
                            onExited:  valueEditRectBalancePopup.close()
                        }

                        Popup {
                            id: valueEditRectBalancePopup
                            x: parent.width - width
                            y: 0 - height
                            width: valueEditRectBalancePopupText.contentWidth + height
                            height: valueEditRectBalancePopupText.contentHeight * 1.5
                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0
                            padding: 0
                            margins: 0

                            background: Item {}

                            contentItem: Rectangle {
                                anchors.fill: parent
                                color: "#e0e0e0"
                                border.color: "#404040"

                                Text {
                                    id: valueEditRectBalancePopupText
                                    anchors.fill: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#404040"

                                    font {
                                        family: valueEditRectBalance.fontFamiliy
                                        pixelSize: valueEditRectBalance.fontSize * 0.85
                                    }

                                    text: valueEditRectBalance.text
                                }
                            }
                        }}

                    /* TABS */
                    RowLayout {
                        id: tabsLayout
                        spacing: tabsLayoutSpacing.width
                        visible: false // !root.internal.isPrice

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

//                DapQmlPushButton {
//                    visible: root.internal.showConfirmButton
//                    qss: "nodeorlist-overview-clear-btn"
//                    text: qsTr("CLEAR")
//                    onClicked: clearEditField()
//                }

                Loader {
                    id: btnConfirm
                    DapQmlStyle { item: btnConfirm; qss: "nodeorlist-overview-confirm-btn" }
                    sourceComponent: pushButton
                    visible: {
                        switch (root.internal.mode)
                        {
                        case QuiNodeOrderList.MaxUnit:
                        case QuiNodeOrderList.MinUnit:
                        case QuiNodeOrderList.MaxPrice:
                        case QuiNodeOrderList.MinPrice:
                        case QuiNodeOrderList.TokenValue:
                            return true;
                        }
                        return false;
                    }

                    property string text: qsTr("CONFIRM")
                    property color color
                    property color fill
                    property var cbClicked: function() {
                        /* store edit value result */
                        let result = "";
                        switch (root.internal.mode)
                        {
                        case QuiNodeOrderList.MaxUnit:
                        case QuiNodeOrderList.MinUnit:
                        case QuiNodeOrderList.MaxPrice:
                        case QuiNodeOrderList.MinPrice:
                        case QuiNodeOrderList.TokenValue:
                            result  = filterValueSet (valueEditInput.text);
                            break;
                        default:
                            break;
                        }

                        /* navigate based on mode */
                        if (result === "")
                        {
                            swipe.decrementCurrentIndex();
                            hideErrorMessage();
                        }
                        else
                        {
                            //valueEditInput.text = root.internal.editValue
                            showErrorMessage (result);
                        }

                        updateFilterSetupTimer.start();
                    }
                }

//                DapQmlPushButton {
//                    visible: root.internal.showConfirmButton
//                    qss: "nodeorlist-overview-confirm-btn"
//                    text: qsTr("CONFIRM")
//                    onClicked: {
//                        /* store edit value result */
//                        let result = "";
//                        switch (root.internal.mode)
//                        {
//                        case QuiNodeOrderList.MaxUnit:
//                        case QuiNodeOrderList.MinUnit:
//                        case QuiNodeOrderList.MaxPrice:
//                        case QuiNodeOrderList.MinPrice:
//                        case QuiNodeOrderList.TokenValue:
//                            result  = filterValueSet (valueEditInput.text);
//                            break;
//                        default:
//                            break;
//                        }

//                        /* navigate based on mode */
//                        if (result === "")
//                        {
//                            swipe.decrementCurrentIndex();
//                            hideErrorMessage();
//                        }
//                        else
//                        {
//                            //valueEditInput.text = root.internal.editValue
//                            showErrorMessage (result);
//                        }

//                        updateFilterSetupTimer.start();
//                    }
//                }

                /****************************************//**
                 * Listview
                 ********************************************/

                DapQmlRectangle {
                    x: (root.width - width) / 2
                    qss: "nodeorlist-listview"
                    visible: !root.internal.showValueEdit

//                        Component.onCompleted: StyleDebugTree.describe (
//                           "csListView",
//                            ["x", "y", "width", "height"],
//                           this);

                    ListView {
                        id: csListView
                        objectName: "listview"
                        anchors.fill: parent
                        spacing: spacer.height
                        delegate: listviewDelegateNameValue
                        visible: root.internal.mode !== QuiNodeOrderList.Orders
                                 && root.internal.mode !== QuiNodeOrderList.Networks
                                 && root.internal.mode !== QuiNodeOrderList.Wallets
                                 && root.internal.mode !== QuiNodeOrderList.Tokens
                        clip: true

                    } // Listview

                    ListView {
                        id: csListViewOrders
                        objectName: "listviewOrders"
                        anchors.fill: parent
                        //spacing: spacer.height
                        delegate: listviewDelegateOrder
                        visible: root.internal.mode === QuiNodeOrderList.Orders
                        clip: true
                    }

                    ListView {
                        id: csListViewNetworks
                        objectName: "listviewNetworks"
                        anchors.fill: parent
                        spacing: spacer.height
                        delegate: listviewDelegateNameValue
                        visible: root.internal.mode === QuiNodeOrderList.Networks
                        clip: true
                    }

                    ListView {
                        id: csListViewWallets
                        objectName: "listviewWallets"
                        anchors.fill: parent
                        spacing: spacer.height
                        delegate: listviewDelegateNameValue
                        visible: root.internal.mode === QuiNodeOrderList.Wallets
                        clip: true
                    }

                    ListView {
                        id: csListViewTokens
                        objectName: "listviewTokens"
                        anchors.fill: parent
                        spacing: spacer.height
                        delegate: listviewDelegateNameValue
                        visible: root.internal.mode === QuiNodeOrderList.Tokens
                        clip: true
                    }
                }
            } // Middle Page
        } // Swipe View
    } // Content
}

/*-----------------------------------------*/
