/* INCLUDES */

import QtQuick 2.15
import QtQuick.Controls 2.12
import Qt5Compat.GraphicalEffects
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
        property real loadingPos: 0 - loadingSize
        property real loadingSize: csListView.width * 0.325
        readonly property real animDuration: 1000
        property real leftSide: 0 - loadingSize
        property real rightSide: csListView.width
        property bool loadingDir: false
        readonly property int popupDuration: 400

        Behavior on loadingPos {
            PropertyAnimation {
                duration: root.internal.animDuration
                easing.type: Easing.Linear // Easing.InOutQuad
            }
        }

        onLoadingPosChanged: {
            if (loadingPos == leftSide)
            {
                loadingPos = rightSide;
                loadingDir = true;
            }
            else
                if (loadingPos == rightSide)
                {
                    loadingPos = leftSide
                    loadingDir = false;
                }
        }

        onSpinnerChanged: {
            if (spinner)
                spinnerWatchdogTimer.start();
        }
    }

    Timer {
        id: spinnerWatchdogTimer
        interval: 8000
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
    signal sigUpdate();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function onListLoaded() {
        //spinnerWatchdogTimer.stop();
        //root.internal.spinner = false;
    }

    function onAppStartTimeout() {
        spinnerWatchdogTimer.stop();
        root.internal.spinner = false;
    }

    function updateLastUpdateLabel(a_text) {
        lastUpdateLabel.text = a_text;
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

            DapQmlDummy {
                id: containerContainer
                anchors.centerIn: parent
                qss: "chooseserver-loading-frame"

                property string color
                property string shine

                Rectangle {
                    id: unknownItemContainerMask
                    anchors.fill: parent
                    color: "white"
                    radius: height / 2
                    visible: false
                }

                DapQmlRectangle {
                    anchors.fill: parent
                    z: 20
                    color: containerContainer.color
                    //color: "#bcbcbc"
                    //radius: height
                    //clip: true
                    radius: height / 2
                }

                Item {
                    id: containerBox
                    anchors.fill: parent
                    visible: false

                    Rectangle {
                        id: childBox
                        x: root.internal.loadingPos
                        y: (!root.internal.loadingDir) * height * 2
                        width: root.internal.loadingSize
                        height: parent.height
                        //visible: root.internal.loadingDir
                        //qss: "c-brand"
                        radius: height
                        //opacity: 0.5

                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "transparent" }
                            GradientStop { position: 0.5; color: containerContainer.shine }
                            GradientStop { position: 1.0; color: "transparent" }

                            orientation: Gradient.Horizontal
                        }
                    }
                }

                OpacityMask {
                    id: containerBoxMask
                    anchors.fill: parent
                    z: 30
                    source: containerBox
                    maskSource: unknownItemContainerMask
                    cached: false
                }
            }

            DapQmlSeparator {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: height
                width: parent.width
            }
        }
    }

    Component {
        id: compServerUnavailable

        Item {}
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
        z: 40
        visible: !root.internal.spinner

        Behavior on opacity {
            PropertyAnimation {
                duration: root.internal.popupDuration
                easing.type: Easing.Linear
            }
        }

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            id: title
            text: qsTr("Choose server") + lang.notifier
            qss: "dialog-title"

            DapQmlPushButton {
                anchors.right: parent.right
                width: parent.height
                height: parent.height
                qss: "chooseserver-update-btn"
                onClicked: { root.sigUpdate(); root.internal.spinner = true; }
            }
        }

        /****************************************//**
         * Last update time label
         ********************************************/

        DapQmlLabel {
            id: lastUpdateLabel
            x: title.x
            y: title.y + title.height
            width: title.width
            height: title.height
            qss: "chooseserver-last-update-label"
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
                interval: 4000
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
                    text    = (a_ping > -1 && a_ping < 9999)
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
                height: (model.ping + csListView.model.hookInt === 9999)
                        ? 0
                        : resizer.height + spacer.height

                Loader {
                    anchors.fill: parent
//                    sourceComponent: model.connectionQuality === -1 + csListView.model.hookInt
//                                     ? compUnknownYet
//                                     : compRadio
                    sourceComponent: {
                        var state = (model.connectionQuality + csListView.model.hookInt === -1) * 1
                                    | (model.ping + csListView.model.hookInt === 9999) * 2;
                        switch(state)
                        {
                        case 0: return compRadio;
                        case 1: return compUnknownYet;
                        case 2: return compServerUnavailable;
                        case 3: return compServerUnavailable;
                        }

                        return compServerUnavailable;
                    }

//                    sourceComponent: compUnknownYet
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
        radius: 30
        cached: true
        z: 50
        visible: root.internal.spinner
    }

    /* spinner */
    DapQmlRectangle {
        id: spinnerContent
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

    DropShadow {
        anchors.fill: spinnerContent
        z: 58
        radius: 18//contentRoot.radius
        samples: 17
        color: `#30${contentShadowColor.color.substring(1)}`
        source: spinnerContent
        visible: root.internal.spinner

        DapQmlDummy {
            id: contentShadowColor
            property string color
            qss: "notification-shadow"
        }
    }
}

/*-----------------------------------------*/
