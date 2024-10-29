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

    /// @brief item clicked
    signal sigSelect(int index, string name);

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
            height: (descriptionLabel.contentHeight < defaultHeight)
                    ? itemFrameSize.height
                    : descriptionLabel.contentHeight + extraHeight

            property real defaultHeight: (itemFrameSize.height / 2)
            property real extraHeight: (itemFrameSize.height / 2)

            /* title mode */

            DapQmlLabel {
                anchors.fill: parent
                anchors.leftMargin: spacings.x
                anchors.rightMargin: spacings.x
                anchors.topMargin: spacings.y
                anchors.bottomMargin: spacings.y
                visible: model.isTitle === true
                horizontalAlign: Text.AlignLeft
                text: model.titleDate
                disableClicking: true
                qss: "notification-item-timestamp-title"
            }

            /* notification */

            Item {
                anchors.fill: parent
                anchors.leftMargin: spacings.x
                anchors.rightMargin: spacings.x
                anchors.topMargin: spacings.y
                anchors.bottomMargin: spacings.y
                visible: model.isTitle === false

                /* time */

                DapQmlLabel {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: contentWidth
                    height: contentHeight
                    disableClicking: true
                    qss: "notification-item-time"
                    text: model.createdTime
                }

                /* title */

                DapQmlLabel {
                    id: itemTitle
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: contentWidth
                    height: contentHeight
                    disableClicking: true
                    qss: "notification-item-title"
                    text: model.typeString
                }

                /* description */

                DapQmlLabel {
                    id: descriptionLabel
                    anchors.left: parent.left
                    anchors.top: itemTitle.bottom
                    anchors.topMargin: spacings.y * 0.625
                    verticalAlign: Text.AlignTop
                    horizontalAlign: Text.AlignLeft
                    width: parent.width
                    height: contentHeight
                    lineHeight: 1.1
                    wrapMode: Text.WordWrap
                    disableClicking: true
                    qss: "notification-item-description"
                    text: model.message
                }

                /* mouse area */

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.sigSelect (model.index, model.message);
                }
            }

            /* separator */

            DapQmlSeparator {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: height + spacings.y
                visible: model.isTitle === false
                width: parent.width
            }
        }
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlDummy {
        id: spacings
        qss: "notification-item-frame-spacings"
    }

    DapQmlDummy {
        id: itemFrameSize
        qss: "notification-item-frame"
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
        spacing: 0
        clip: true

        delegate: compDelegate
    }
}

/*-----------------------------------------*/
