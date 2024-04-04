/* INCLUDES */

import QtQuick 2.1
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import com.DapQmlWidgets 1.0
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

    property QtObject internal: QtObject {
        property bool spinner: true

        onSpinnerChanged: {
            if (spinner)
                spinnerWatchdogTimer.start();
        }
    }

    Timer {
        id: spinnerWatchdogTimer
        interval: 5000
        running: true
        repeat: false
        onTriggered: root.internal.spinner = false
    }

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

    function onListLoaded() {
        spinnerWatchdogTimer.stop();
        root.internal.spinner = false;
    }

    /// @}
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
     * Content
     ********************************************/

    Item {
        id: content
        anchors.fill: parent
        visible: !root.internal.spinner

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            id: title
            text: qsTr("Choose server") + lang.notifier
            qss: "dialog-title"
        }

        /****************************************//**
         * Popup
         ********************************************/

        Rectangle {
            id: itemPopup
            x: parent.width - width
            y: 0 - height
            z: 200
            width: popupLabel.contentWidth * 1.25
            height: popupLabel.contentHeight * 1.5
            visible: false
            color: "#e0e0e0"
            border.color: "#404040"

            function show(a_x, a_y, a_ping) {
                //console.log(`popup show: ${a_x},${a_y},${a_ping}`);
                x   = a_x - width;
                y   = a_y - height;
                popupLabel.setPing (a_ping);
                visible = true;
                hideTimer.restart();
            }

            function hide() {
                //console.log(`popup hide`);
                visible = false;
            }

            Timer {
                id: hideTimer
                interval: 5000
                repeat: false
                running: false
                onTriggered: itemPopup.hide()
            }

            Text {
                id: popupLabel
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "#404040"

    //            property int ping: model.ping + csListView.model.hookInt

                function setPing(a_ping) {
                    text    = (a_ping > -1)
                              ? (`ping ${a_ping} ms`)
                              : ("unavailable");
                }

                text: "unavailable"
            }
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

            delegate: DapQmlRadioButton {
                property int quality: model.connectionQuality + csListView.model.hookInt

                text: model.name + csListView.model.hook
                separator: true
                iconSize: resizer.height
                y: spacer.height / 2
                width: resizer.width
                height: resizer.height + spacer.height
                checked: csListView.model.current === model.index //model.checked + csListView.model.hookInt
                opacity: 0

                Behavior on opacity { PropertyAnimation { duration: 250 } }

                Component.onCompleted: opacity = 1

                DapQmlLabel {
                    id: itemPing
                    property int quality: (parent.quality === 0) ? (0) : (6 - parent.quality)
                    x: parent.width - (width * 1.35)
                    y: (parent.height - height) / 2
                    width: resizer.height * 0.5
                    height: resizer.height * 0.5
                    qss: `ic_conn-${quality}` + csListView.model.hook

                    MouseArea {
                        anchors.fill: parent
                        z: 10
                        hoverEnabled: true
                        onEntered: {
                            var point   = mapToItem(null, 0, 0);
                            itemPopup.show(point.x, point.y, model.ping);
                        }
                        onExited:  itemPopup.hide()
                    }
                }

                onClicked: root.sigSelect (model.index, model.name)
            }
        }
    }

    /****************************************//**
     * Spinner
     ********************************************/

    /* spinner blur effect */
    FastBlur {
        anchors.fill: content
        source: content
        radius: 40
        cached: true
        z: 50
        visible: root.internal.spinner
    }

    /* spinner */
    DapQmlRectangle {
        anchors.centerIn: parent
        z: 60
        qss: "chooseserver-spinner-bg"
        visible: root.internal.spinner

        DapQmlArcAnimation {
            anchors.fill: parent
            anchors.margins: parent.width * 0.275
            strokeWidth: 7
            z: 200
            qss: "c-brand"
        }
    }
}

/*-----------------------------------------*/
