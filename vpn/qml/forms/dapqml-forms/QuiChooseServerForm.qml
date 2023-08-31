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
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseServer"

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
     * @name FUNCTIONS
     ********************************************/
    /// @{

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
            id: csListViewItem
            width: resizer.width
            height: resizer.height + spacer.height
            property bool checked: false

//            property string logMessage
//            property int logMessageCounter: 0

//            function collectLogMessage(a_msg) {
//                logMessage += a_msg + " : ";
//                logMessageCounter++;
//                if (logMessageCounter >= 4)
//                    console.log(logMessage + model.name);
//            }

            //onHeightChanged: csListViewItem.collectLogMessage (`item ${x.toFixed(2)},${y.toFixed(2)},${z.toFixed(2)} ${width.toFixed(2)}x${height.toFixed(2)}`)

            DapQmlRadioButton {
                property int quality: model.connectionQuality + csListView.model.hookInt

                text: model.name + csListView.model.hook
                checked: csListView.model.current === model.index //model.checked + csListView.model.hookInt
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height + spacer.height
                y: spacer.height / 2

                //onHeightChanged: csListViewItem.collectLogMessage (`radio ${x.toFixed(2)},${y.toFixed(2)},${z.toFixed(2)} ${width.toFixed(2)}x${height.toFixed(2)}`)

                DapQmlLabel {
                    property int quality: (parent.quality === 0) ? (0) : (6 - parent.quality)
                    x: parent.width - (width * 1.35)
                    y: (parent.height - height) / 2
                    width: resizer.height * 0.5
                    height: resizer.height * 0.5
                    qss: `ic_conn-${quality}` + csListView.model.hook

                    //onHeightChanged: csListViewItem.collectLogMessage (`icn ${x.toFixed(2)},${y.toFixed(2)},${z.toFixed(2)} ${width.toFixed(2)}x${height.toFixed(2)}`)

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

                                property int ping: model.ping + csListView.model.hookInt

                                text: (ping > -1)
                                      ? (`ping ${ping} ms` + csListView.model.hook)
                                      : ("unavailable" + csListView.model.hook)
                            }
                        }
                    }
                }

                onClicked: root.sigSelect (model.index, model.name)
            }
        }
    }
}

/*-----------------------------------------*/
