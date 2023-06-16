/* INCLUDES */

import QtQuick 2.0
import DapQmlStyle 1.0
import QtGraphicalEffects 1.12

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

    /// @brief widget qss style
    property string qss

    property QtObject internal: QtObject {
        property bool dragging: false
        property bool draggingAnim: false
        property bool draggingState: tgl.draggingState
        property real pos1
        property real pos2
        property real diff: pos2 - pos1
        property real draggingStartDistance: 6

        function _begin() {
            pos1            = point.x;
            pos2            = point.x;
            dragging        = false;
            draggingAnim    = false;

            console.log(`SWITCH drag begin: ${pos1.toFixed(0)}|${pos2.toFixed(0)}|${diff.toFixed(0)}|${dragging}|${draggingState}`);
        }

        function _move() {
            /* store value */
            pos2 = point.x;

            /* check for dragging */
            if (diff > 0)
            {
                if (diff > draggingStartDistance)
                {
                    dragging        = true;
                    draggingAnim    = true;
                }
            }
            if (diff < 0)
            {
                if (diff < -draggingStartDistance)
                {
                    dragging        = true;
                    draggingAnim    = true;
                }
            }

            console.log(`SWITCH drag move: ${pos1.toFixed(0)}|${pos2.toFixed(0)}|${diff.toFixed(0)}|${dragging}|${draggingState}`);
        }

        function _end() {
            /* turn off anim */
            draggingAnim    = false;

            /* not dragging */
            if (!dragging)
                _toggle();
            else

            /* on left side */
            if (!draggingState)
                _turnOff();

            /* on right side */
            else
                _turnOn();

            dragging    = false;

            console.log(`SWITCH drag end: ${pos1.toFixed(0)}|${pos2.toFixed(0)}|${diff.toFixed(0)}|${dragging}|${draggingState}`);
        }

        function _turnOff() {
            if (root.checked === false)
                return;

            root.setState(false);
            //root.clicked();
            console.log(`SWITCH drag turnOff`);
        }

        function _turnOn() {
            if (root.checked === true)
                return;

            root.setState(true);
            //root.clicked();
            console.log(`SWITCH drag turnOn`);
        }

        function _toggle() {
            root.toggle();
            console.log(`SWITCH drag toggle`);
        }
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
        desaturateEffect.desaturation = 0.5 * (!value);
    }

    /// @brief change style based on checkbox state
    function _setStyle() {
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

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    Item {
        id: content
        anchors.fill: parent
        visible: false

        /****************************************//**
         * Background frame
         ********************************************/

        DapQmlLabel {
            id: bg
            x: _centerHor(this)
            y: _centerVer(this)
            z: 0
            width: root.width - 12 * (root.width / 270)
            height: root.height - 36 * (root.height / 174)
            qss: "switch-bg-off"

            //onClicked: toggle()
        }

        /****************************************//**
         * Toggle
         ********************************************/

        DapQmlLabel {
            id: tgl
            //x: (checked === false) ? (-12 * (root.width / 270)) : (root.width - width + 12 * (root.width / 270))
            x: {
                if (root.internal.dragging)
                {
                    if (isLeftReached)
                        return minPos;
                    if (isRightReached)
                        return maxPos;
                    return draggingPos
                }
                return finalPos;
            }

            y: 0
            z: 1
            width: root.height
            height: root.height
            qss: "switch-toggle-off"

            property real finalPos:         (checked === false) ? minPos : maxPos
            property real draggingPos:      root.internal.pos2 - (width / 2)
            property real minPos:           (-12 * (root.width / 270))
            property real maxPos:           (root.width - width + 12 * (root.width / 270))
            property bool isLeftReached:    draggingPos <= minPos
            property bool isRightReached:   draggingPos >= maxPos
            property bool draggingState:    root.internal.pos2 >= root.width / 2

            //onClicked: toggle()

            Behavior on x {
                PropertyAnimation {
                    duration: root.internal.draggingAnim ? 0 : 125
                    easing.type: Easing.InQuad
                }
            }
        }
    }

    /****************************************//**
     * Saturation effect
     ********************************************/

    Desaturate {
        id: desaturateEffect
        anchors.fill: content
        source: content
        desaturation: root.enable ? 0 : 0.4
    }

    /****************************************//**
     * Mouse area
     ********************************************/

//    MouseArea {
//        anchors.fill: parent
//        onClicked: toggle()
//    }
    MultiPointTouchArea {
        id: draggingSpace
        anchors.fill: parent
        z: 10
        touchPoints: [
            TouchPoint {
                id: point
                onXChanged: root.internal._move()
            }
        ]
        onPressed:  internal._begin() // console.log(`switch pressed`);
        onReleased: internal._end()   // console.log(`switch released`);

//        Rectangle {
//            width: 30; height: 30
//            color: "green"
//            x: point.x
//            y: point.y
//        }
    }
}

/*-----------------------------------------*/
