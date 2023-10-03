/* INCLUDES */

import QtQuick 2.0
import QtGraphicalEffects 1.0
import NotificationCtl 1.0
//import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Qui Tool Notification
 * @date 02.10.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlDummy {
    id: root
    x: (parent.width - width) / 2
    y: (0 - height * 1.5)
    clip: false
    qss: "notification-frame"

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property QtObject internal: QtObject {
        property bool show: false

        onShowChanged: root.y = show ? (positioner.y) : (0 - root.height * 1.5)
    }

    Timer {
        interval: 1000
        running: true
        repeat: false
        onTriggered: { setShow (!root.internal.show); }
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigCloseClicked();
    signal sigBodyClicked();

    Behavior on y {
        PropertyAnimation {
            easing.type: Easing.OutQuad
            duration: 250
        }
    }

    Component.onCompleted: {
        setShow(false);
        NotificationCtl.attach (root);
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setShow(a_value) {
        root.internal.show  = a_value;
    }

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/

    DapQmlDummy {
        id: positioner
        qss: "notification-position"
    }

    /****************************************//**
     * Content
     ********************************************/

    DapQmlRectangle {
        id: content
        anchors.centerIn: parent
        qss: "notification-content"
        visible: false
    }

    /****************************************//**
     * Shadow
     ********************************************/

    DropShadow {
        anchors.fill: content
        radius: content.height / 5
        samples: 17
        color: `#80${shadowColor.color.substring(1)}`
        source: content

        DapQmlDummy {
            id: shadowColor
            property string color
            qss: "notification-shadow"
        }
    }

    /****************************************//**
     * Text
     ********************************************/

    DapQmlLabel {
        anchors.fill: content
        disableClicking: true
        text: "test"
        qss: "c-label"
    }

    /****************************************//**
     * Clickable area
     ********************************************/

    MouseArea {
        anchors.fill: content
        onClicked: root.sigBodyClicked();
    }

    /****************************************//**
     * Close button
     ********************************************/

    DapQmlRectangle {
        id: closeBtn
        anchors.verticalCenter: content.top
        anchors.horizontalCenter: content.right
        qss: "notification-close-btn-area"

        /* variables */

        property bool hovered: false
        property string image
        property color idleColor
        property color hoveredColor

        /* button frame */

        DapQmlRectangle {
            anchors.fill: parent
            anchors.margins: width * 0.275
            radius: width * 0.5
            color: parent.hovered ? parent.hoveredColor : parent.idleColor

            /* close icon image */

            DapQmlImage {
                anchors.fill: parent
                anchors.margins: width * 0.15
                scaledPixmap: closeBtn.image

//                Component.onCompleted: StyleDebugTree.describe (
//                   "not-image",
//                    ["x", "y", "width", "height"],
//                   this);
            }

//            Component.onCompleted: StyleDebugTree.describe (
//               "not-circle",
//                ["x", "y", "width", "height"],
//               this);
        }

        /* clickable area */

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.sigCloseClicked();
            onEntered: parent.hovered = true
            onExited:  parent.hovered = false
        }

//        Component.onCompleted: StyleDebugTree.describe (
//           "not-area",
//            ["x", "y", "width", "height"],
//           this);
    }
}

/*-----------------------------------------*/
