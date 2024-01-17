/* INCLUDES */
import QtQuick 2.0
import QtQuick.Layouts 1.0
import PageCtl 1.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Transaction History Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * -Resizer for items
 * -Listview with custom delegate
 *
 * @date 11.09.23
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
    property string formName: "TransactionHistory"

    Timer {
        id: backTimer
        interval: 350
        running: false
        repeat: false
        onTriggered: PageCtl.slotBackwardAuto()
    }

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
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlDummy {
        id: itemSizer
        visible: false
        qss: "trhistory-item"
        property real margin: width * 0.035
    }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: compDelegate

        Item {
            width: itemSizer.width
            height: itemSizer.height

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: itemSizer.margin
                anchors.rightMargin: itemSizer.margin

                /* left vertical items */
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    /* left top */
                    DapQmlLabel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: Text.AlignBottom
                        elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "trhistory-item-left-top"
                        text: `${model.units} ${model.priceUnit}`
                    }

                    /* left bottom */
                    DapQmlLabel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlign: Text.AlignLeft
                        verticalAlign: Text.AlignTop
                        elide: Text.ElideMiddle
                        disableClicking: true
                        qss: "trhistory-item-left-bottom"
                        text: model.nodeLocation
                    }
                }

                /* right item */
                DapQmlLabel {
                    Layout.preferredWidth: contentWidth
                    Layout.fillHeight: true
                    horizontalAlign: Text.AlignRight
                    disableClicking: true
                    qss: "trhistory-item-right"
                    text: model.created // "placeholder text"
                }
            }

            DapQmlSeparator {
                anchors.bottom: parent.bottom
                width: parent.width
            }

            MouseArea {
                anchors.fill: parent
                onClicked: root.sigSelect (model.index, model.nodeLocation);
            }
        }
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Transaction history") + lang.notifier
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
