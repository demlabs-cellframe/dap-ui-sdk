/* INCLUDES */

import QtQuick 2.11
import QtQuick.Shapes 1.4
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Qt5Compat.GraphicalEffects
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0
import Brand 1.0
import DapQmlModelRoutingExceptions 1.0

/****************************************//**
 * @brief Routing Exceptions Form
 * @ingroup groupDapQmlForms
 *
 * @date 12.04.23
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum TabType
    {
        APPS,
        ROUTES
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "RoutingExceptions"

    property real loadingListPercent: 0

    property QtObject internal: QtObject {
        property int type: QuiRoutingExceptions.APPS
        property bool popup: false
        property bool spinner: false
        property bool inc: false
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigTabChanged(int a_tabIndex);
    signal sigModeChanged(bool a_includedList);
    signal sigPopupOpen();
    signal sigPopupAppSearchFilterEdited(string a_filter);

    signal sigPopupCancel();
    signal sigPopupAppSave();
    signal sigPopupRouteAdd(string a_ip, string a_description);

    signal sigAppRemove(int a_index, string a_name);
    signal sigPopupAppCheckboxClicked(int a_index, string a_name);
    signal sigRouteRemove(int a_index, string a_ip);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function showSpinner(a_show) {
        root.internal.popup     = a_show;
        root.internal.spinner   = a_show;
    }

    function onlyHideSpinner() {
        root.internal.spinner   = false;
    }

    function enableRoutes(a_enable) {
        tabsContainer.visible   = a_enable;
    }

    function setMode(a_includedMode) {
        root.internal.inc = a_includedMode;
    }

    function _switchAppsMode() {
        root.internal.inc = !root.internal.inc;
        root.sigModeChanged (root.internal.inc);
    }

    function _popupBottomButtonClicked(isSecond) {
        if(root.internal.type === QuiRoutingExceptions.APPS)
        {
            if (!isSecond)
                root.sigPopupCancel(); // console.log("_popupBottomButtonClicked > apps > first");
            else
                root.sigPopupAppSave(); // console.log("_popupBottomButtonClicked > apps > second");
        }
        else
            if(root.internal.type === QuiRoutingExceptions.ROUTES)
            {
                if (!isSecond)
                    root.sigPopupCancel(); // console.log("_popupBottomButtonClicked > routes > first");
                else
                    root.sigPopupRouteAdd(rouexcAddressInput.mainText, rouexcDescriptionInput.mainText); // console.log("_popupBottomButtonClicked > routes > second");
            }

        root.internal.popup         = false;
        popupAppsSearch.mainText    = "";
    }

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/

    DapQmlDummy { id: rouexcTabPos;     qss: "rouexc-tab-postion" }
    DapQmlDummy { id: rouexcTabSize;    qss: "rouexc-tab-idle" }
    DapQmlDummy { id: rouexcTabSpace;   qss: "rouexc-tab-spacing" }
    DapQmlDummy { id: rouexcItemIcon;   qss: "rouexc-content-item-icon" }
    DapQmlDummy { id: rouexcPopupTitle; qss: "rouexc-title-container" }
    DapQmlDummy { id: rouexcAppModeSw;  qss: "rouexc-tab-content-app-mode-switch-container" }

    DapQmlDummy {
        id: popupDialogTitleCloseButtonDummy

        property string inactive
        property string active

        qss: "rouexc-content-item-btn-close"
    }

    DapQmlDummy { id: listviewPopupAppsSizer;           qss: "rouexc-popup-apps-content"; }
    DapQmlDummy { id: rouexcContentItem;                qss: "rouexc-content-item" }
    DapQmlDummy { id: rouexcContentItemIcon;            qss: "rouexc-content-item-icon" }
    DapQmlDummy { id: rouexcPopupAppCheckbox;           qss: "rouexc-popup-app-checkbox";               property string scaledPixmap }
    DapQmlDummy { id: rouexcPopupAppCheckboxChecked;    qss: "rouexc-popup-app-checkbox-checked";       property string scaledPixmap }
    DapQmlDummy { id: rouexcContentItemMain;            qss: "rouexc-popup-app-item-label-pos c-label"; property color color; property int fontSize }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: tabButton

        // property int tabType
        // property string name

        DapQmlRectangle {
            anchors.fill: parent

            qss: root.internal.type === parent.tabType //QuiRoutingExceptions.TapType.APPS
                 ? "rouexc-tab-idle rouexc-tab-selected"
                 : "rouexc-tab-idle"

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                qss: root.internal.type === parent.parent.tabType // QuiRoutingExceptions.TapType.APPS
                     ? "rouexc-tab-idle-label rouexc-tab-selected-label"
                     : "rouexc-tab-idle-label"
                disableClicking: true
                text: parent.parent.name // "APPS"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.internal.type = parent.parent.tabType;
                    root.sigTabChanged (parent.parent.tabType);
                }
            }
        }
    }

    Component {
        id: pushButton

        // property bool isSecond
        // property string name
        // property var callback

        DapQmlRectangle {
            anchors.fill: parent
            qss: (!parent.isSecond) ? "rouexc-popup-btn" : "rouexc-popup-btn-second"

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                qss: (!parent.parent.isSecond) ? "rouexc-popup-btn-label" : "rouexc-popup-btn-label-second"
                disableClicking: true
                text: parent.parent.name // "APPS"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: parent.parent.callback()
            }
        }
    }

    Component {
        id: switchModeButton

        // property string name
        // property var callback

        DapQmlRectangle {
            qss: "rouexc-tab-content-app-mode-switch"

            DapQmlRectangle {
                anchors.fill: parent
                color: "#F45480"
                opacity: 0.05
                radius: height / 8
            }

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                color: "#F45480"
                disableClicking: true
                text: parent.parent.name // "APPS"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: parent.parent.callback()
            }
        }

    }

    Component {
        id: delegateApp

        Item {
            width: listviewAppsExc.width
            height: rouexcContentItem.height

            /* icon */
            DapQmlLabel {
                x: width * 0.4375
                y: (parent.height - height) / 2
                scaledPixmap: "image://DapQmlModelRoutingExceptionsImageProvider/" + model.packageName + ".png"
                disableClicking: true
                qss: "rouexc-content-item-icon"
            }

            /* label */
            Text {
                x: rouexcContentItemMain.x
                y: (parent.height - height) / 2
                width: parent.width - x * 1.25 - delegateAppCloseBtn.width * 1.25
                height: contentHeight
                color: rouexcContentItemMain.color
                text: model.appName
                clip: true
                elide: Text.ElideRight

                font {
                    family: Brand.fontName()
                    pixelSize: rouexcContentItemMain.fontSize
                    weight: Font.Normal
                }
            }

            /* close button */
            DapQmlLabel {
                id: delegateAppCloseBtn
                x: parent.width - (width * 1.4375)
                y: (parent.height - height) / 2
                qss: "rouexc-content-app-btn-close"

                onClicked: root.sigAppRemove (model.index, model.packageName); //console.log (`clicked ${model.index}`);
                //onHeightChanged: console.log(`delegateApp ${x},${y} ${width}x${height} :${scaledPixmap}`);
            }

            /* separator */
            DapQmlSeparator {
                y: parent.height - height
                width: parent.width
            }
        }
    }

    Component {
        id: delegateAppCheck

        Item {
            width: listviewPopupAppsSizer.width
            height: rouexcContentItem.height

            /* icon */
            DapQmlLabel {
                x: width * 0.4375
                y: (parent.height - height) / 2
                width: rouexcContentItemIcon.width
                height: rouexcContentItemIcon.height
                scaledPixmap: "image://DapQmlModelRoutingExceptionsImageProvider/" + model.packageName + ".png"
                disableClicking: true
            }

            /* label */
            Text {
                x: rouexcContentItemMain.x
                y: (parent.height - height) / 2
                width: parent.width - x - delegateAppCheckCheckBox.width
                height: contentHeight
                color: rouexcContentItemMain.color
                text: model.appName
                clip: true
                elide: Text.ElideRight

                font {
                    family: Brand.fontName()
                    pixelSize: rouexcContentItemMain.fontSize
                    weight: Font.Normal
                }
            }

            /* checkbox */
            DapQmlLabel {
                id: delegateAppCheckCheckBox
                x: parent.width - (width * 0.675 * 1.4375)
                y: (parent.height - height) / 2
                width: rouexcPopupAppCheckbox.width
                height: rouexcPopupAppCheckbox.height
                property bool checked: model.checked
                scaledPixmap: !checked ? rouexcPopupAppCheckbox.scaledPixmap : rouexcPopupAppCheckboxChecked.scaledPixmap
                function clickEd() {
                    root.sigPopupAppCheckboxClicked (model.index, model.packageName);
                    checked = !checked;
                }
            }

            /* separator */
            DapQmlSeparator {
                y: parent.height - height
                width: parent.width
            }

            MouseArea {
                anchors.fill: parent
                onClicked: delegateAppCheckCheckBox.clickEd()
            }
        }

//        DapQmlButton {
//            width: listviewPopupAppsSizer.width
//            mainQss: "c-label"
//            qss: "rouexc-content-item"
//            buttonStyle: DapQmlButton.Style.IconMainSubIcon
//            mainText: model.appName
//            subText: ""

//            /* icon */
//            DapQmlLabel {
//                x: width * 0.4375
//                y: (parent.height - height) / 2
//                scaledPixmap: "image://DapQmlModelRoutingExceptionsImageProvider/" + model.packageName + ".png"
//                disableClicking: true
//                qss: "rouexc-content-item-icon"
//            }

//            /* checkbox button */
//            DapQmlLabel {
//                x: parent.width - (width * 0.675 * 1.4375)
//                y: (parent.height - height) / 2
//                qss: "rouexc-popup-app-checkbox" + (model.checked ? " rouexc-popup-app-checkbox-checked" : "")

//                onClicked: root.sigPopupAppCheckboxClicked (model.index, model.packageName); //console.log (`clicked ${model.index}`);
//            }

//            /* separator */
//            DapQmlSeparator {
//                y: parent.height - height
//                width: parent.width
//            }
//        }
    }

    Component {
        id: delegateRoute

        DapQmlRectangle {
            width: listviewRoutes.width
            qss: "rouexc-content-item"

            DapQmlButton {
                x: rouexcItemIcon.width * 0.4375
                width: parent.width - x
                height: parent.height
                buttonStyle: DapQmlButton.Style.TopMainBottomSub
                mainText: model.address
                subText: model.description
                mainQss: "text-align-left rouexc-content-item-main"
                subQss: "text-align-left rouexc-content-item-sub"

                /* close button */
                DapQmlLabel {
                    x: parent.width - (width * 1.4375)
                    y: (parent.height - height) / 2
                    qss: "rouexc-content-app-btn-close"

                    onClicked: root.sigRouteRemove(model.index, model.address); // console.log (`clicked ${model.index}`);
                }
            }

            /* separator */
            DapQmlSeparator {
                y: parent.height - height
                width: parent.width
            }
        }
    }

    Component {
        id: popupDialogTitle

        Item {
            anchors.fill: parent

            RowLayout {
                anchors.fill: parent

                /* title */
                DapQmlLabel {
                    Layout.leftMargin: rouexcPopupTitle.x
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlign: Qt.AlignLeft
                    qss: "rouexc-title-label"
                    text: parent.parent.parent.name
                    clip: true
                }

                /* close button */
                DapQmlPushButton {
                    Layout.rightMargin: rouexcPopupTitle.x * 0.875
                    Layout.preferredWidth: popupDialogTitleCloseButtonDummy.width
                    Layout.preferredHeight: popupDialogTitleCloseButtonDummy.height
                    Layout.alignment: Qt.AlignVCenter

                    inactive: popupDialogTitleCloseButtonDummy.inactive
                    active: popupDialogTitleCloseButtonDummy.active

                    onClicked: {
                        root.sigPopupCancel();
                        root.internal.popup = false;
                    }
                }
            }


            /* bottom */
            DapQmlSeparator {
                id: popupDialogTitleBottom
                anchors.bottom: parent.bottom
                qss: "rouexc-title-separator"
            }
        }
    }

    Component {
        id: popupBottomButtons

        // property string firstName
        // property string secondName

        Item {
            width: parent.width
            height: popupItemSizer.height

            //onHeightChanged: console.log(`popup bottom ${width}x${height} # ${parent.width}x${parent.height}`)

            /* top */
            DapQmlSeparator {
                id: popupBottomButtonsSeparator
                qss: "rouexc-popup-bottom-separator"
            }

            /* sizer */
            DapQmlDummy {
                id: popupButtonSizer
                qss: "rouexc-popup-btn"
            }
            DapQmlDummy {
                id: popupItemSizer
                qss: "rouexc-popup-bottom-size"
            }

            /* buttons */
            RowLayout {
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                width: popupButtonSizer.width * 2 + spacing
                height: popupButtonSizer.height
                spacing: rouexcTabSpace.width

                /* left button */
                Loader {
                    Layout.preferredWidth: popupButtonSizer.width
                    Layout.preferredHeight: popupButtonSizer.height
                    property string name: parent.parent.parent.firstName
                    property bool isSecond: false
                    property var callback: function() {_popupBottomButtonClicked(isSecond)}
                    sourceComponent: pushButton
                }

                /* right button */
                Loader {
                    Layout.preferredWidth: popupButtonSizer.width
                    Layout.preferredHeight: popupButtonSizer.height
                    property string name: parent.parent.parent.secondName
                    property bool isSecond: true
                    property var callback: function() {_popupBottomButtonClicked(isSecond)}
                    sourceComponent: pushButton
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
        visible: !root.internal.popup

        /* popup dialog darkness */
        Rectangle {
            width: window.width
            height: window.height
            z: 55
            color: "#80000000"
            visible: root.internal.popup
        }

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            id: title
            text: root.internal.inc
                  ? "Inclusion in routing"
                  : "Routing Exceptions"
            qss: "dialog-title"
        }

        /****************************************//**
         * Add button
         ********************************************/

        DapQmlPushButton {
            id: rouexcBtnAdd
            qss: "rouexc-btn-add"

            onClicked: {
                rouexcAddressInput.mainText     = "";
                rouexcDescriptionInput.mainText = "";
                root.internal.popup             = true
                root.sigPopupOpen();
            }
        }

        /****************************************//**
         * Tab buttons
         ********************************************/

        DapQmlDummy {
            id: separatorPos
            qss: "rouexc-tab-separator"

            property real calcPos: tabsContainer.visible
                                   ? y
                                   : y + tabsContainer.height
        }

        DapQmlSeparator {
            x: (parent.width - width) / 2
            y: separatorPos.calcPos - rouexcAppModeSw.height
            width: rouexcAppModeSw.width
            height: separatorPos.height
            visible: root.internal.type === QuiRoutingExceptions.APPS
            //qss: "rouexc-tab-content-app-mode-switch-container"
        }

        Item {
            y: separatorPos.calcPos - rouexcAppModeSw.height + 1
            width: rouexcAppModeSw.width
            height: rouexcAppModeSw.height - 1
            visible: root.internal.type === QuiRoutingExceptions.APPS

            Loader {
                anchors.centerIn: parent
                property string name: !root.internal.inc
                                      ? qsTr("SWITCH TO INCLUSION IN ROUTING")
                                      : qsTr("SWITCH TO ROUTING EXCEPTIONS")
                property var callback: function() {_switchAppsMode();}
                sourceComponent: switchModeButton
            }
        }

        DapQmlSeparator {
            x: (parent.width - width) / 2
            y: separatorPos.calcPos
            width: separatorPos.width
            height: separatorPos.height
            //qss: "rouexc-tab-separator"
        }

        RowLayout {
            id: tabsContainer
            x: (parent.width - width) / 2
            y: rouexcTabPos.y
            width: rouexcTabSize.width * 2 + spacing
            height: rouexcTabSize.height
            spacing: rouexcTabSpace.width

            Loader {
                Layout.preferredWidth: rouexcTabSize.width
                Layout.preferredHeight: rouexcTabSize.height
                property int tabType: QuiRoutingExceptions.APPS
                property string name: "APPS"
                sourceComponent: tabButton
            }

            Loader {
                Layout.preferredWidth: rouexcTabSize.width
                Layout.preferredHeight: rouexcTabSize.height
                property int tabType: QuiRoutingExceptions.ROUTES
                property string name: "ROUTES"
                sourceComponent: tabButton
            }
        }

        /****************************************//**
         * Tab content
         ********************************************/

        DapQmlDummy {
            id: listviewSizer
            qss: "rouexc-tab-content"

            property real calcHeight: tabsContainer.visible
                                      ? height
                                      : height + tabsContainer.height
        }

        Item {
            id: listviewAppsContainer
            x: (parent.width - width) / 2
            y: listviewSizer.y
            width: listviewSizer.width
            height: listviewSizer.calcHeight - rouexcAppModeSw.height
            visible: root.internal.type === QuiRoutingExceptions.APPS

            ListView {
                id: listviewAppsExc
                anchors.fill: parent
                objectName: "listviewAppsExc"
                visible: root.internal.inc === false
                clip: true

                delegate: delegateApp
            }

            ListView {
                id: listviewAppsInc
                objectName: "listviewAppsInc"
                anchors.fill: parent
                visible: root.internal.inc === true
                clip: true

                delegate: delegateApp
            }
        }

        ListView {
            id: listviewRoutes
            objectName: "listviewRoutes"
            x: (parent.width - width) / 2
            y: listviewSizer.y
            width: listviewSizer.width
            height: listviewSizer.calcHeight
            visible: root.internal.type === QuiRoutingExceptions.ROUTES
            clip: true

            //DapQmlStyle { item: listviewRoutes; qss: "rouexc-tab-content"; }

            delegate: delegateRoute
            //model: modelRoutes
        }

        /*-----------------------------------------*/
    }

    /****************************************//**
     * Popup content
     ********************************************/

    /* popup dialog blur effect */
    FastBlur {
        anchors.fill: content
        source: content
        radius: 40
        cached: true
        z: 50
        visible: root.internal.popup
    }

    /* popup spinner */
    DapQmlRectangle {
        anchors.centerIn: parent
        z: 60
        qss: "rouexc-spinner-bg"
        visible: root.internal.popup & root.internal.spinner

//        AnimatedImage {
//            id: spinnerAnim
//            anchors.centerIn: parent
//            source: scaledPixmap

//            property string scaledPixmap

//            DapQmlStyle { item: spinnerAnim; qss: "rouexc-spinner ic_spinner" }
//        }

        DapQmlLabel {
            x: (parent.width - width) / 2
            qss: "rouexc-spinner-label"
            text: qsTr("Loading apps list...") + lang.notifier
        }

        DapQmlRectangle {
            anchors.top: parent.top
            qss: "rouexc-spinner-arc-container"

            DapQmlDummy {
                id: progressCircle
                qss: "rouexc-spinner-arc"
                property string outer
                property string inner
                property int strokeWidth: 10
            }

            DapQmlDummy {
                id: rouexcSpinner
                qss: "rouexc-spinner"

//                property real start
//                property real sweep

//                PropertyAnimation on start {
//                    loops: Animation.Infinite
//                    from: 0
//                    to: 360
//                    duration: 1250
//                }

//                PropertyAnimation on sweep {
//                    loops: Animation.Infinite
//                    from: 0
//                    to: 360
//                    duration: 1250
//                }
            }

            DapQmlLabel {
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                qss: "rouexc-spinner-percentage c-label"
                text: `${Math.floor(root.loadingListPercent)}%`
            }

            Shape {
                id: outerLine
                anchors.centerIn: parent
                z: 50
                width: progressCircle.width
                height: progressCircle.height
                // multisample, decide based on your scene settings
                layer.enabled: true
                layer.samples: 6


                ShapePath {
                    fillColor: "transparent"
                    //strokeColor: progressCircle.color
                    strokeColor: progressCircle.outer
                    strokeWidth: progressCircle.strokeWidth
                    capStyle: ShapePath.FlatCap

                    PathAngleArc {
                        centerX: progressCircle.width/2
                        centerY: progressCircle.height/2
                        radiusX: progressCircle.width/2 - progressCircle.strokeWidth
                        radiusY: progressCircle.height/2 - progressCircle.strokeWidth
                        startAngle: 0
                        sweepAngle: 360
                    }
                }
            }

            Shape {
                id: innerLine
                anchors.centerIn: parent
                z: 60
                width: progressCircle.width
                height: progressCircle.height
                // multisample, decide based on your scene settings
                layer.enabled: true
                layer.samples: 6

                ShapePath {
                    fillColor: "transparent"
                    //strokeColor: progressCircle.color
                    strokeColor: progressCircle.inner
                    strokeWidth: progressCircle.strokeWidth
                    capStyle: ShapePath.RoundCap

                    PathAngleArc {
                        centerX: progressCircle.width/2
                        centerY: progressCircle.height/2
                        radiusX: progressCircle.width/2 - progressCircle.strokeWidth
                        radiusY: progressCircle.height/2 - progressCircle.strokeWidth
                        startAngle: 0 - 90 //root.loadingListPercent === 0 ? rouexcSpinner.start - 90 : 0 - 90
                        sweepAngle: root.loadingListPercent * 360 / 100 //root.loadingListPercent === 0 ? rouexcSpinner.start : root.loadingListPercent * 360 / 100
                    }
                }
            }
        }

    }

    /* popup dialog content */
    DapQmlRectangle {
        id: popupDialogContentBG
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        z: 60
        visible: root.internal.popup & !root.internal.spinner
        qss: root.internal.type === QuiRoutingExceptions.APPS
             ? "c-background rouexc-popup-content-apps"
             : "c-background rouexc-popup-content-routes"

        /****************************************//**
         * Apps Popup
         ********************************************/

        Item {
            anchors.fill: parent
            visible: root.internal.type === QuiRoutingExceptions.APPS
            clip: true

            /* title */
            Loader {
                width: parent.width
                height: rouexcPopupTitle.height
                property string name: "Exception management"
                sourceComponent: popupDialogTitle
            }

            /* search */
            DapQmlRectangle {
                id: popupAppsSearchContainer
                x: (parent.width - width) / 2
                qss: "rouexc-popup-app-search"

                DapQmlLineEdit {
                    id: popupAppsSearch
                    objectName: "popupAppsSearch"
                    anchors.fill: parent
                    focus: true

                    mainQss: "rouexc-popup-app-search-input c-label"
                    placeholderQss: "rouexc-popup-app-search-input"
                    iconQss: "ic_country_filter"

                    mainText: ""
                    placeHolderText: "Search application"

                    onTextEdited: root.sigPopupAppSearchFilterEdited(mainText)
                }
            }

            /* list exc */
            ListView {
                id: listviewPopupAppsExc
                objectName: "listviewPopupAppsExc"
                x: (parent.width - width) / 2
                y: listviewPopupAppsSizer.y
                width: listviewPopupAppsSizer.width
                height: popupAppsBottom.y - y
                visible: root.internal.inc === false
                clip: true

                delegate: delegateAppCheck
            }

            /* list inc */
            ListView {
                id: listviewPopupAppsInc
                objectName: "listviewPopupAppsInc"
                x: (parent.width - width) / 2
                y: listviewPopupAppsSizer.y
                width: listviewPopupAppsSizer.width
                height: popupAppsBottom.y - y
                visible: root.internal.inc === true
                clip: true

                delegate: delegateAppCheck
            }

            /* bottom buttons */
            Loader {
                id: popupAppsBottom
                y: parent.height - height
                width: parent.width
                property string firstName:  "Cancel"
                property string secondName: "Save"
                sourceComponent: popupBottomButtons
            }
        }

        /****************************************//**
         * Routes Popup
         ********************************************/

        Item {
            anchors.fill: parent
            visible: root.internal.type === QuiRoutingExceptions.ROUTES
            clip: true

            /* title */
            Loader {
                width: parent.width
                height: rouexcPopupTitle.height
                property string name: "Exception management"
                sourceComponent: popupDialogTitle
            }

            /* address */
            DapQmlRectangle {
                x: (parent.width - width) / 2
                qss: "rouexc-route-address rouexc-route-input-field"

                DapQmlLineEdit {
                    id: rouexcAddressInput
                    qss: "rouexc-route-input-field-size"
                    mainQss: "rouexc-route-input-field-font c-label"
                    iconQss: "null-size null-pos rouexc-route-input-margin"
                    placeholderQss: "rouexc-route-input-field-font rouexc-route-input-field-placeholder"
                    mainText: ""
                    placeHolderText: "Enter ip address/host"
                }
            }


            /* description */
            DapQmlRectangle {
                x: (parent.width - width) / 2
                qss: "rouexc-route-description rouexc-route-input-field"

                DapQmlLineEdit {
                    id: rouexcDescriptionInput
                    qss: "rouexc-route-input-field-size"
                    mainQss: "rouexc-route-input-field-font c-label"
                    iconQss: "null-size null-pos rouexc-route-input-margin"
                    placeholderQss: "rouexc-route-input-field-font rouexc-route-input-field-placeholder"
                    mainText: ""
                    placeHolderText: "Enter description"
                }
            }

            /* bottom buttons */
            Loader {
                y: parent.height - height
                width: parent.width
                property string firstName:  "Cancel"
                property string secondName: "Add"
                sourceComponent: popupBottomButtons
            }
        }
    }
}

/*-----------------------------------------*/
