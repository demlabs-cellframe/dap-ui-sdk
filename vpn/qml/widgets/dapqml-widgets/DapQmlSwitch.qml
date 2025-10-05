/* INCLUDES */

import QtQuick 2.11
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.4
import DapQmlStyle 1.0
import DapQmlSwitchCtl 1.0
import com.DapQmlWidgets 1.0
import StyleDebugTree 1.0
import Qt5Compat.GraphicalEffects

/****************************************//**
 * @brief Dap QML Switch Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Checkbox-like widget presented as a
 * turn on / turn off switch
 *
 * Built using two images:
 * - Background frame image
 * - Foreground toggle image
 *
 * @note changes style based on "checked" state
 *
 * @date 07.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    width: 270
    height: 174

    //opacity: 0.5
    enabled: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief checkbox state
    /// @note changes hooked to _setStyle()
    property bool checked: false

    /// @brief show loading animation
    property bool loading: !enabled

    /// @brief widget qss style
    property string qss

    property alias ctl: ctl

    DapQmlSwitchCtl {
        id: ctl
        draggingStartDistance: 8
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief switch is clicked (toggled)
    signal clicked();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change switch state
    function setState(value) {
        checked = value;
    }

    /// @brief toggle switch
    function toggle() {
        setState(!checked);
        root.clicked();
    }

    function setEnable(value) {
        root.enabled = value;
        //desaturateEffect.desaturation = 0.5 * (!value);
    }

    /// @brief change style based on checkbox state
    function _setStyle() {
        tglUpdateTimer.stop();
        tglUpdateTimer.start();
        bg.qss  = (!checked) ? "switch-bg-off" : "switch-bg-on";
        tgl.qss = (!checked) ? "switch-toggle-off" : "switch-toggle-on";
    }

    /// @brief calc horizontal centering
    function _centerHor(item) {
        return root.width / 2 - item.width / 2;
    }

    /// @brief calc vertical centering
    function _centerVer(item) {
        return root.height / 2 - item.height / 2;
    }

    onCheckedChanged: _setStyle()
    Component.onCompleted: ctl.setRoot(this)

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    Item {
        id: content
        anchors.fill: parent
        //visible: false

        /****************************************//**
         * Background frame
         ********************************************/

        DapQmlLabel {
            id: bg
            x: _centerHor(this)
            y: _centerVer(this)
            z: 0
            width: root.width - 24 * (root.width / 270)
            height: root.height - 36 * (root.height / 174)
            qss: "switch-bg-off"

            Component.onCompleted: ctl.setBackground(this)

            DapQmlLabel {
                id: bgConn
                anchors.fill: parent
                z: 1
                qss: "switch-bg-conn"
                visible: root.loading && root.checked
            }
        }

        /****************************************//**
         * Toggle
         ********************************************/

        DapQmlLabel {
            id: tgl
            y: 0
            z: 5
            width: root.height
            height: root.height
            qss: "switch-toggle-off"

            Behavior on x {
                PropertyAnimation {
                    duration: ctl.draggingAnim ? 0 : 125
                    easing.type: Easing.InQuad
                    Component.onCompleted: ctl.setToggleAnimation(this)
                }
            }

            Component.onCompleted: ctl.setToggle(this)

            Timer {
                id: tglUpdateTimer
                running: true
                repeat: true
                interval: 2500
                onTriggered: ctl.updateTogglePos()
            }

            Rectangle {
                id: loadingFrame
                anchors.centerIn: parent
                visible: root.loading
                width: size
                height: size
                radius: size
                color: "white"

                property real size: Math.min(parent.width, parent.height) * 0.75

                //DapQmlStyle { item: loadingFrame; qss: "c-background" }

                DapQmlArcAnimation {
                    anchors.fill: parent
                    anchors.margins: parent.width * 0.275
                    z: 200
                    qss: "c-dashboard-accent"
                }
            }
        }
    }

    /****************************************//**
     * Saturation effect
     ********************************************/

//    Desaturate {
//        id: desaturateEffect
//        anchors.fill: content
//        source: content
//        desaturation: root.enable ? 0 : 0.4
//    }

    /****************************************//**
     * Mouse area
     ********************************************/

    MultiPointTouchArea {
        id: draggingSpace
        anchors.fill: parent
        z: 10
        touchPoints: [
            TouchPoint {
                id: point
                //onXChanged: root.internal._move()
                Component.onCompleted: ctl.setTouchingPoint(this)
            }
        ]
        Component.onCompleted: ctl.setTouchArea(this)
        //onPressed:  internal._begin() // console.log(`switch pressed`);
        //onReleased: internal._end()   // console.log(`switch released`);

//        Rectangle {
//            width: 30; height: 30
//            color: "green"
//            x: point.x
//            y: point.y
//        }
    }
}

/*-----------------------------------------*/
