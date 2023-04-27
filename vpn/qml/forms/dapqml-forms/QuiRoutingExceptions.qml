/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
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

    property QtObject internal: QtObject {
        property int type: QuiRoutingExceptions.APPS
        property bool popup: false
        property bool spinner: false
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigTabChanged(int a_tabIndex);
    signal sigPopupOpen();

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
        root.internal.popup = false;
    }

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/

    DapQmlDummy { id: rouexcTabPos;    qss: "rouexc-tab-postion" }
    DapQmlDummy { id: rouexcTabSize;   qss: "rouexc-tab-idle" }
    DapQmlDummy { id: rouexcTabSpace;  qss: "rouexc-tab-spacing" }
    DapQmlDummy { id: rouexcItemIcon;  qss: "rouexc-content-item-icon" }

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
        id: delegateApp

        DapQmlButton {
            width: listviewApps.width
            mainQss: "c-label"
            qss: "rouexc-content-item"
            buttonStyle: DapQmlButton.Style.IconMainSubIcon
            mainText: model.appName
            subText: ""

            /* icon */
            DapQmlLabel {
                x: width * 0.4375
                y: (parent.height - height) / 2
                scaledPixmap: "image://DapQmlModelRoutingExceptionsImageProvider/" + model.packageName + ".png"
                disableClicking: true
                qss: "rouexc-content-item-icon"
            }

            /* close button */
            DapQmlLabel {
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
            width: listviewPopupApps.width
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
                width: rouexcContentItemMain.width - x - delegateAppCheckCheckBox.width
                height: contentHeight
                color: rouexcContentItemMain.color
                text: model.appName

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
                scaledPixmap: !model.checked ? rouexcPopupAppCheckbox.scaledPixmap : rouexcPopupAppCheckboxChecked.scaledPixmap
                onClicked: root.sigPopupAppCheckboxClicked (model.index, model.packageName);
            }

            /* separator */
            DapQmlSeparator {
                y: parent.height - height
                width: parent.width
            }
        }

//        DapQmlButton {
//            width: listviewPopupApps.width
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
            width: parent.width
            height: popupDialogTitleBottom.y

            /* title */
            DapQmlLabel {
                y: (parent.height - height) / 2
                height: contentHeight
                qss: "rouexc-title-label"
                text: parent.parent.name
                clip: false
            }

            /* close button */
            DapQmlPushButton {
                id: popupDialogTitleCloseBtn
                y: (parent.height - height) / 2
                qss: "rouexc-content-item-btn-close"
                onClicked: {
                    root.sigPopupCancel();
                    root.internal.popup = false;
                }
            }

            /* bottom */
            DapQmlSeparator {
                id: popupDialogTitleBottom
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
            text: "Routing Exceptions"
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

        ListView {
            id: listviewApps
            objectName: "listviewApps"
            x: (parent.width - width) / 2
            y: listviewSizer.y
            width: listviewSizer.width
            height: listviewSizer.calcHeight
            visible: root.internal.type === QuiRoutingExceptions.APPS
            clip: true

            //DapQmlStyle { item: listviewApps; qss: "rouexc-tab-content"; }

            delegate: delegateApp
            //model: modelApp
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

        AnimatedImage {
            id: spinnerAnim
            anchors.centerIn: parent
            source: scaledPixmap

            property string scaledPixmap

            DapQmlStyle { item: spinnerAnim; qss: "rouexc-spinner ic_spinner" }
        }
    }

    /* popup dialog content */
    DapQmlRectangle {
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
                id: popupAppsTitle
                width: parent.width
                property string name: "Exception management"
                sourceComponent: popupDialogTitle
            }

            /* list */
            ListView {
                id: listviewPopupApps
                objectName: "listviewPopupApps"
                x: (parent.width - width) / 2
                y: popupAppsTitle.height
                width: listviewPopupAppsSizer.width
                height: popupAppsBottom.y - y
                clip: true

                DapQmlDummy { id: listviewPopupAppsSizer;           qss: "rouexc-popup-apps-content"; }
                DapQmlDummy { id: rouexcContentItem;                qss: "rouexc-content-item" }
                DapQmlDummy { id: rouexcContentItemIcon;            qss: "rouexc-content-item-icon" }
                DapQmlDummy { id: rouexcPopupAppCheckbox;           qss: "rouexc-popup-app-checkbox";               property string scaledPixmap }
                DapQmlDummy { id: rouexcPopupAppCheckboxChecked;    qss: "rouexc-popup-app-checkbox-checked";       property string scaledPixmap }
                DapQmlDummy { id: rouexcContentItemMain;            qss: "rouexc-popup-app-item-label-pos c-label"; property color color; property int fontSize }

                delegate: delegateAppCheck
                //model: modelCheckedApp

//                delegate: Text {
//                    text: model.appName
//                }

//                delegate: Image {
//                    width: 64
//                    height: 64
//                    source: "image://DapQmlModelRoutingExceptionsImageProvider/" + model.packageName + ".png"
//                }
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
