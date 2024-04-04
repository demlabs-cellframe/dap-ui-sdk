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
     * @name COMPONENTS
     ********************************************/
    /// @{

    Component {
        id: compRadio

//        property int index
//        property int quality
//        property int ping
//        property string name

        DapQmlRadioButton {
            id: itemRoot

            property int quality: parent.quality + csListView.model.hookInt

            text: parent.name + csListView.model.hook
            separator: true
            iconSize: resizer.height
            y: spacer.height / 2
            width: resizer.width
            height: resizer.height + spacer.height
            checked: csListView.model.current === parent.index
            opacity: 0

            Behavior on opacity { PropertyAnimation { duration: 250 } }

            Component.onCompleted: opacity = 1

            DapQmlLabel {
                id: itemPing
                x: parent.width - (width * 1.35)
                y: (parent.height - height) / 2
                width: resizer.height * 0.5
                height: resizer.height * 0.5
                qss: `ic_conn-${quality}` + csListView.model.hook

                property int quality: (itemRoot.quality === 0) ? (0) : (6 - itemRoot.quality)

                MouseArea {
                    anchors.fill: parent
                    z: 10
                    hoverEnabled: true
                    onEntered: {
                        var point   = mapToItem(null, 0, 0);
                        itemPopup.show(point.x, point.y, itemRoot.parent.ping);
                    }
                    onExited:  itemPopup.hide()
                }
            }

            onClicked: root.sigSelect (parent.index, parent.name)
        }
    }

    Component {
        id: compUnknownYet

        Item {
            y: spacer.height / 2
            width: resizer.width
            height: resizer.height + spacer.height

//            DapQmlArcAnimation {
//                x: parent.height * 0.125
//                y: (parent.height - height) / 2
//                width: parent.height * 0.75
//                height: parent.height * 0.75
//                strokeWidth: 6
//                qss: "c-brand"
//            }

//            DapQmlSeparator {
//                x: parent.height * 1.125
//                y: (parent.height - height) / 2
//                width: parent.width - parent.height * 1.25
//            }

            DapQmlRectangle {
                id: containerBox
                anchors.centerIn: parent
                width: parent.width * 0.75
                height: parent.height * 0.125
                color: "#aaa"
                radius: height
                clip: true

                DapQmlRectangle {
                    id: childBox
                    width: parent.width * 0.325
                    height: parent. height
                    qss: "c-brand"
                    radius: height

                    property real leftSide: 0 - childBox.width / 2
                    property real rightSide: containerBox.width - childBox.width / 2
                    readonly property real animDuration: 750

                    Behavior on x { PropertyAnimation { duration: childBox.animDuration; easing.type: Easing.InOutQuad } }

                    onXChanged: {
                        if (x == leftSide)
                            x = rightSide;
                        else
                            if (x == rightSide)
                                x = leftSide
                    }

                    Component.onCompleted: {
                        x = rightSide;
                    }
                }
            }

            DapQmlSeparator {
                y: parent.height - height
                width: parent.width
            }
        }
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

            delegate: Item {
                id: csListViewItem
                width: resizer.width
                height: resizer.height + spacer.height

                Loader {
                    anchors.fill: parent
                    sourceComponent: model.connectionQuality === -1 + csListView.model.hookInt
                                     ? compUnknownYet
                                     : compRadio
                    property int index: model.index
                    property int quality: model.connectionQuality + csListView.model.hookInt
                    property int ping: model.ping + csListView.model.hookInt
                    property string name: model.name
                }
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
