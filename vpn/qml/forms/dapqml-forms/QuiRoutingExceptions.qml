/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0
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
    }

    DapQmlModelRoutingExceptions {
        id: modelApp
        mode: DapQmlModelRoutingExceptions.APPS
    }

    DapQmlModelRoutingExceptions {
        id: modelRoutes
        mode: DapQmlModelRoutingExceptions.ROUTES
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: "Routing Exceptions"
        qss: "dialog-title"
    }

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
                onClicked: root.internal.type = parent.parent.tabType
            }
        }
    }

    Component {
        id: delegateApp

        DapQmlButton {
            width: listviewApps.width
            qss: "rouexc-content-item"
            buttonStyle: DapQmlButton.Style.IconMainSubIcon
            mainText: model.name
            subText: ""
            //onHeightChanged: console.log (`index ${model.index} s ${width}*${height}`);

//            Rectangle {
//                anchors.fill: parent
//                color: "green"
//            }

            /* icon */
            DapQmlLabel {
                x: width * 0.4375
                y: (parent.height - height) / 2
                scaledPixmap: model.icon
                disableClicking: true
                qss: "rouexc-content-item-icon"
            }

            /* close button */
            DapQmlLabel {
                x: parent.width - (width * 1.4375)
                y: (parent.height - height) / 2
                qss: "rouexc-content-item-btn-close"

                onClicked: console.log (`clicked ${model.index}`);
            }
        }
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

                onHeightChanged: console.log (`index ${model.index} p ${x}x${y} s ${width}*${height}`);

                /* close button */
                DapQmlLabel {
                    x: parent.width - (width * 1.4375)
                    y: (parent.height - height) / 2
                    qss: "rouexc-content-item-btn-close"

                    onClicked: console.log (`clicked ${model.index}`);
                }
            }
        }
    }

    /****************************************//**
     * Tab buttons
     ********************************************/

    DapQmlSeparator {
        qss: "rouexc-tab-separator"
    }

    RowLayout {
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

    ListView {
        id: listviewApps
        objectName: "listviewApps"
        visible: root.internal.type === QuiRoutingExceptions.APPS
        clip: true

        DapQmlStyle { item: listviewApps; qss: "rouexc-tab-content"; }

        delegate: delegateApp
        model: modelApp
    }

    ListView {
        id: listviewRoutes
        objectName: "listviewRoutes"
        visible: root.internal.type === QuiRoutingExceptions.ROUTES
        clip: true

        DapQmlStyle { item: listviewRoutes; qss: "rouexc-tab-content"; }

        delegate: delegateRoute
        model: modelRoutes
    }
}

/*-----------------------------------------*/
