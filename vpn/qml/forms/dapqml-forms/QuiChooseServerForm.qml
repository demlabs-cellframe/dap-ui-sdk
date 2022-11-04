/* INCLUDES */

import QtQuick 2.1
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Server Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using simple ListView with DapQmlRadioButton as delegate
 *
 * Form contains resizer for ListView items
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseServer"

    /// @brief items array
    ///
    /// Need to store all items
    property var items: new Array

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);
//    signal sigCurrentInexChanged();

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Choose server") + lang.notifier
        qss: "dialog-title"
    }

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

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: csListView
        objectName: "chooseServerListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height
            property bool checked: false

            DapQmlRadioButton {
                property int ping: model.ping
                property int quality: model.connectionQuality

                text: model.name + csListView.model.hook
                checked: model.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2

                // for debug purposes - uncomment 'Text' below
//                Text {
//                    text: `${model.name} >> ping [${parent.ping}] quality [${parent.quality}]` + csListView.model.hook
//                }

                DapQmlLabel {
                    property int quality: (parent.quality === 0) ? (0) : (6 - parent.quality)
                    x: parent.width - (width * 1.35)
                    y: (parent.height - height) / 2
                    width: resizer.height * 0.5
                    height: resizer.height * 0.5
                    qss: `ic_conn-${quality}` + csListView.model.hook
                    ToolTip {
                        id: id_tooltip
                        opacity : 0.70
                        contentItem: Text{
                            color: "#404040"
                            text: (ping > -1) ? "ping " + ping + " ms" : "unavailable"
                        }
                        background: Rectangle {
                            border.color: "#404040"
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: id_tooltip.visible = true
                        onExited: id_tooltip.visible = false
                    }
                }

                onClicked: root.sigSelect (model.index, model.name)
                Component.onCompleted: { items.push(this); }
            }
        }
    }
}

/*-----------------------------------------*/
