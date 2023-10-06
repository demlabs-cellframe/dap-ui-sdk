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
    width: frameSizer.width
    height: textLabel.contentHeight < contentSizer.height
            ? frameSizer.height
            : frameSizer.height * 0.75 + textLabel.contentHeight
    clip: false

    //                Component.onCompleted: StyleDebugTree.describe (
    //                   "root",
    //                    ["x", "y", "width", "height"],
    //                   this);

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property QtObject internal: QtObject {
        property bool show: false
        property string message: NotificationCtl.message
        property string typeString: typeList[type]
        property int type: NotificationCtl.type

        property var typeList: [
            "",
            qsTr("Error"),
            qsTr("Notification"),
            qsTr("Warning"),
        ]

        onShowChanged: {
            if (show)
                hideTimer.start();
            else
                hideTimer.stop();
            root.y = show ? (positioner.y) : (0 - root.height * 1.5)
        }
        onTypeChanged: typeString = typeList[type]
    }

    Timer {
        id: hideTimer
        interval: 3000
        running: false
        repeat: false
        onTriggered: { setShow (false); }
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

    DapQmlDummy {
        id: frameSizer
        qss: "notification-frame"
    }

    DapQmlDummy {
        id: contentSizer
        qss: "notification-content"
        property string color
        property real radius
    }

    /****************************************//**
     * Content Root
     ********************************************/

    DapQmlDummy {
        id: contentRoot
        anchors.fill: parent
        visible: false

        //                Component.onCompleted: StyleDebugTree.describe (
        //                   "contentRoot",
        //                    ["x", "y", "width", "height"],
        //                   this);

        /****************************************//**
         * Content
         ********************************************/

        DapQmlRectangle {
            id: content
            anchors.centerIn: parent
            width: contentSizer.width
            height: root.height - frameSizer.height + contentSizer.height
            color: contentSizer.color
            radius: contentSizer.radius

            //                Component.onCompleted: StyleDebugTree.describe (
            //                   "content",
            //                    ["x", "y", "width", "height"],
            //                   this);
        }

        /****************************************//**
         * Text
         ********************************************/

        DapQmlLabel {
            id: textLabel
            anchors.fill: content
            disableClicking: true
            text: root.internal.message
            qss: "c-label"
        }

        /****************************************//**
         * Header
         ********************************************/

        DapQmlRectangle {
            id: header
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: content.top
            qss: "notification-header"
            borderWidth: 1

            property real fontSize: 15

            //                Component.onCompleted: StyleDebugTree.describe (
            //                   "header",
            //                    ["x", "y", "width", "height"],
            //                   this);

            DapQmlLabel {
                anchors.fill: parent
                fontSize: parent.fontSize
                text: root.internal.typeString
                qss: root.internal.type === 2 ? "c-label" : "c-error"
            }
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
                anchors.margins: parent.width * 0.275
                radius: width * 0.5
                color: parent.hovered ? parent.hoveredColor : parent.idleColor

                /* close icon image */

                DapQmlImage {
                    anchors.fill: parent
                    anchors.margins: parent.width * 0.15
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

    /****************************************//**
     * Content Shadow
     ********************************************/

    DropShadow {
        anchors.fill: contentRoot
        radius: header.height * 0.5
        samples: 17
        color: `#30${contentShadowColor.color.substring(1)}`
        source: contentRoot

        DapQmlDummy {
            id: contentShadowColor
            property string color
            qss: "notification-shadow"
        }
    }
}

/*-----------------------------------------*/
