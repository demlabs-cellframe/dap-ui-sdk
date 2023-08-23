/* INCLUDES */

import QtQuick 2.11
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Shapes 1.4
import DapQmlStyle 1.0
import Brand 1.0
import PageCtl 1.0
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
        MaxUnit,
        MinUnit,
        MaxPrice,
        MinPrice,

        TokenValue
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

        /* MODES */
        property var mode: QuiNodeOrderList.Invalid

        /// list format:
        /// showConfirmButton,showValueEdit,isEditingUnit,listviewMode
        property var modeSettings: [
            0,0,0,DapQmlModelOrderList.Invalid,     // Invalid

            0,0,0,DapQmlModelOrderList.Orders,      // Orders

            0,0,0,DapQmlModelOrderList.Networks,    // Networks
            0,0,0,DapQmlModelOrderList.Wallets,     // Wallets
            0,0,0,DapQmlModelOrderList.Tokens,      // Tokens

            0,0,0,DapQmlModelOrderList.Units,       // Units
            1,1,0,DapQmlModelOrderList.Invalid,     // Max Unit
            1,1,0,DapQmlModelOrderList.Invalid,     // Min Unit
            1,1,0,DapQmlModelOrderList.Invalid,     // Max Price
            1,1,0,DapQmlModelOrderList.Invalid,     // Min Price

            1,1,0,DapQmlModelOrderList.TokenValue,  // Token Value
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
            showConfirmButton       = modeSettings[mode*4+0];
            showValueEdit           = modeSettings[mode*4+1];
            isEditingUnit           = modeSettings[mode*4+2];
            let listviewMode        = modeSettings[mode*4+3];
            isPrice                 = mode === QuiNodeOrderList.MaxPrice || mode === QuiNodeOrderList.MinPrice;

            if (csListView.model)
                csListView.model.setMode (listviewMode);
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

            case QuiNodeOrderList.TokenValue: listTitle = qsTr("Max Price"); break;
            }

            /* set value */
            switch (mode)
            {
            case QuiNodeOrderList.MaxUnit:  editValue   = interfaceObject.maxUnit(); break;
            case QuiNodeOrderList.MinUnit:  editValue   = interfaceObject.minUnit(); break;
            case QuiNodeOrderList.MaxPrice: editValue   = interfaceObject.maxPrice(); break;
            case QuiNodeOrderList.MinPrice: editValue   = interfaceObject.minPrice(); break;

            case QuiNodeOrderList.TokenValue: editValue = interfaceObject.maxUnit(); break;
            }
            valueEditInput.text = (editValue === "") ? "0" : editValue;
        }
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

    signal sigSearchClicked();
    signal sigWalletConfirmClicked();

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

    function filterItemSelected (a_index, a_name) {
        if (csListView.model === undefined)
            return;

        let mode    = csListView.model.mode;

        console.log(`filterItemSelected at ${a_index}, value "${a_name}", mode ${mode}`)

        switch(mode)
        {
        case QuiNodeOrderList.Networks: root.internal.network   = a_name; break;
        case QuiNodeOrderList.Wallets:  root.internal.wallet    = a_name; break;
        case QuiNodeOrderList.Tokens:   root.internal.token     = a_name; break;
        case QuiNodeOrderList.Units:    root.internal.unit      = a_name; break;
        }
    }

    function filterValueSet (a_value) {
        switch (root.internal.mode)
        {
        case QuiNodeOrderList.MaxUnit:  root.internal.maxUnit   = a_value; break;
        case QuiNodeOrderList.MinUnit:  root.internal.minUnit   = a_value; break;
        case QuiNodeOrderList.MaxPrice: root.internal.maxPrice  = a_value; break;
        case QuiNodeOrderList.MinPrice: root.internal.minPrice  = a_value; break;
        case QuiNodeOrderList.TokenValue: root.internal.tokenValue = a_value; break;
        }
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
    }

    function showChooseWallet() {
        root.internal.isSearch  = false;
    }

    function showSearchOrder() {
        root.internal.isSearch  = true;
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
        id: overviewSizer
        qss: "nodeorlist-overview-item-left-size"
        property int fontSize
    }

    DapQmlDummy {
        id: colorLabel
        qss: "c-label"
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
            height: resizer.height
            sourceComponent: compButton
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
            property string second:      `${model.server} - ${model.ipAddress}`
            property string labelTopQss:    "nodeorlist-label-size-14"
            property string labelBottomQss: "nodeorlist-label-size-14"
            property QtObject tooltip: QtObject {
                //property string topText:    model.units ? `${model.price}\nper ${model.units_value} ${model.units}` : `${model.price}`;
                //property string bottomText: `${model.server} \n ${model.ipAddress}`
                property string text: model.price
            }

            property var cbOnClicked: function() {
                root.internal.network       = model.network;
                root.internal.wallet        = model.wallet;
                root.internal.server        = model.server;
                //root.internal.unit          = model.units;
                root.internal.price         = model.price;
                root.internal.priceShort    = model.priceShort;
                root.internal.chosenOrderIndex  = model.index;
                root.internal.chosenOrderHash   = model.hash;

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
            text: (model.name !== undefined) ? model.name : ""

            DapQmlLabel {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: parent.iconSize * 0.325
                width: contentWidth
                height: contentHeight
                disableClicking: true
                text: (model.value !== undefined) ? model.value : ""
                qss: "nodeorlist-name-value c-grey"
            }

            onClicked: {
                csListView.model.currentIndex = model.index;
                root.filterItemSelected (model.index, text);
                timerFilterItemSelected.start(); // swipe.decrementCurrentIndex();
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
                case QuiNodeOrderList.Tab.Search:   return qsTr("Search order");
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
                        property string second:     "Network"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigNetworkClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: !root.internal.isSearch
                        property string first:      root.internal.wallet
                        property string second:     "Wallet"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigWalletClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: !root.internal.isSearch
                        property string first:      root.internal.token
                        property string second:     "Token"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigTokenClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: !root.internal.isSearch
                        property string first:      root.internal.tokenValue
                        property string second:     "Max price"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigTokenValueClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.unit
                        property string second:     "Unit"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigUnitClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.maxUnit
                        property string second:     "Max Unit"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMaxUnitClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.minUnit
                        property string second:     "Min Unit"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMinUnitClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.maxPrice
                        property string second:     "Max Price"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMaxPriceClicked(); }
                    }

                    Loader {
                        sourceComponent: compButton
                        visible: root.internal.isSearch
                        property string first:      root.internal.minPrice
                        property string second:     "Min Price"
                        property string labelTopQss
                        property string labelBottomQss
                        property bool swap:         true
                        property var cbOnClicked: function() { root.sigMinPriceClicked(); }
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }

                DapQmlPushButton {
                    qss: "nodeorlist-overview-confirm-btn"
                    text: root.internal.isSearch ? qsTr("SEARCH ORDER") : qsTr("CONFIRM")
                    onClicked: {
                        storeFilterData();
                        if (root.internal.isSearch)
                        {
                            root.internal.popup = true;
                            root.sigSearchClicked();
                        }
                        else
                            root.sigWalletConfirmClicked();
                    }
                }
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
                        qss: "nodeorlist-edit"
                        property real fontSize

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: parent.height * 0.1

                            TextInput {
                                id: valueEditInput
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                horizontalAlignment: TextInput.AlignLeft //AlignHCenter
                                verticalAlignment: TextInput.AlignVCenter
                                font.pixelSize: valueEditRect.fontSize
                                font.family: Brand.fontName()
                                font.weight: Font.Bold
                                selectByMouse: true
                                text: root.internal.editValue // "1234"
                                clip: true

                                onTextEdited: {
                                    if (interfaceObject === undefined)
                                        return;

                                    text    = root.internal.isPrice
                                            ? interfaceObject.fixPriceString(text)
                                            : interfaceObject.fixNumberString(text);
                                }
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
                        anchors.top: valueEditRect.bottom
                        anchors.horizontalCenter: valueEditRect.horizontalCenter
                        anchors.margins: height/2
                        width: valueEditRect.width
                        height: valueEditRect.height * 0.23
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
                                return `Balance: ${csListView.model.balance} ${root.internal.token}`;

                            if (root.internal.isPrice)
                                return `${valueEditInput.text} ${root.internal.token}`;

                            if (root.internal.unit === "All")
                                return "";

                            return `${valueEditInput.text} ${root.internal.unit}`
                        }
                    }

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

                DapQmlPushButton {
                    visible: root.internal.showConfirmButton
                    qss: "nodeorlist-overview-confirm-btn"
                    text: qsTr("CONFIRM")
                    onClicked: {
                        /* store edit value result */
                        switch (root.internal.mode)
                        {
                        case QuiNodeOrderList.MaxUnit:
                        case QuiNodeOrderList.MinUnit:
                        case QuiNodeOrderList.MaxPrice:
                        case QuiNodeOrderList.MinPrice:
                        case QuiNodeOrderList.TokenValue:
                            filterValueSet (valueEditInput.text);
                            break;
                        default:
                            break;
                        }

                        /* navigate based on mode */
                        if (root.internal.mode === QuiNodeOrderList.Orders)
                            swipe.incrementCurrentIndex();
                        else
                            swipe.decrementCurrentIndex();
                    }
                }

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
                        clip: true

                    } // Listview

                    ListView {
                        id: csListViewOrders
                        objectName: "listviewOrders"
                        anchors.fill: parent
                        spacing: spacer.height
                        delegate: listviewDelegateOrder
                        visible: root.internal.mode === QuiNodeOrderList.Orders
                        clip: true
                    }
                }
            } // Middle Page

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
                    onClicked: {
                        root.sigOrderSelect (root.internal.chosenOrderIndex, root.internal.chosenOrderHash);
                    }
                }
            } // Transaction Overview
        } // Swipe View
    } // Content
}

/*-----------------------------------------*/
