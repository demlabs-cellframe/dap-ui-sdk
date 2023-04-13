/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0

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
            anchors.fill: parent
            mainText: "app"
        }
    }

    Component {
        id: delegateRoute

        DapQmlButton {
            anchors.fill: parent
            mainText: "route"
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
        model: [1,2,3,4]
    }

    ListView {
        id: listviewRoutes
        objectName: "listviewRoutes"
        visible: root.internal.type === QuiRoutingExceptions.ROUTES
        clip: true

        DapQmlStyle { item: listviewRoutes; qss: "rouexc-tab-content"; }

        delegate: delegateRoute
        model: [1,2,3,4,5,6,7]
    }
}

/*-----------------------------------------*/
