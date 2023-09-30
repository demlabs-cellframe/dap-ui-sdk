/* INCLUDES */

import QtQuick 2.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Notifications History Form
 * @ingroup groupDapQmlForms
 *
 * @date 30.09.23
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
    property string formName: "NotificationsHistory"

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: compDelegate

        DapQmlRectangle {
            width: csListView.width
            qss: "notification-item-frame"

            /* title mode */

            DapQmlLabel {
                anchors.fill: parent
                visible: model.isTitle === true
                horizontalAlign: Text.AlignLeft
                text: model.text
                qss: "notification-item-timestamp-title"
            }

            /* notification */

            Item {
                anchors.fill: parent
                visible: model.isTitle === false

                /* time */

                DapQmlLabel {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: contentWidth
                    height: contentHeight
                    qss: "notification-item-time"
                    text: model.createdTime
                }

                /* title */

                DapQmlLabel {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: contentWidth
                    height: contentHeight
                    qss: "notification-item-title"
                    text: model.typeString
                }

                /* description */

                DapQmlLabel {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: contentWidth
                    height: contentHeight
                    qss: "notification-item-description"
                    text: model.message
                }
            }
        }
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Notifications") + lang.notifier
        qss: "dialog-title"
    }

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: csListView
        objectName: "listview"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        delegate: compDelegate
    }
}

/*-----------------------------------------*/
