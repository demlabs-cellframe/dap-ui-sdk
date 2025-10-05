/* INCLUDES */

import QtQuick 2.0
import Qt5Compat.GraphicalEffects
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
    x: Math.round ((parent.width - width) / 2)
    //y: (0 - height * 1.5)
    width: Math.round (frameSizer.width)
    height: textLabel.contentHeight < contentSizer.height
            ? Math.round (frameSizer.height)
            : Math.round (frameSizer.height * 0.75 + textLabel.contentHeight)
    clip: false
//    color: "green"

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
//            if (show)
//                hideTimer.start();
//            else
//                hideTimer.stop();
            //root.y = show ? (positioner.y) : (0 - root.height * 1.5)
            root.updateState();
        }
        onTypeChanged: typeString = typeList[type]
    }

    property QtObject swipeCtl: QtObject {
        property bool active: root.internal.show
        property bool dragging: false
        property real dragStart
        property real dragPosition

        onDraggingChanged: {
            root.updateState();
        }
    }

    state: "hidden"
    states: [
        State {
            name: "dragged"
            PropertyChanges { target: positionAnimation; duration: 0 }
            PropertyChanges { target: hideTimer; running: false }
            PropertyChanges { target: contentRoot; y: Math.round (positioner.y + root.swipeCtl.dragPosition) }
        },
        State {
            name: "showed"
            PropertyChanges { target: positionAnimation; duration: 250 }
            PropertyChanges { target: hideTimer; running: true }
            PropertyChanges { target: contentRoot; y: Math.round (positioner.y) }
        },
        State {
            name: "hidden"
            PropertyChanges { target: positionAnimation; duration: 250 }
            PropertyChanges { target: hideTimer; running: false }
            PropertyChanges { target: contentRoot; y: Math.round (0 - root.height * 1.5) }
        }
    ]

    Timer {
        id: hideTimer
        interval: 3300
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

    Component.onCompleted: {
        setShow(false);
        NotificationCtl.attach (root);

//        StyleDebugTree.describe (
//           "root",
//            ["x", "y", "width", "height"],
//           this);
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setShow(a_value) {
        root.internal.show  = a_value;
    }

    function updateState() {
        let stateIndex = (internal.show * 2) + (swipeCtl.dragging * 1);
        let stateList = [
                "hidden",
                "hidden",
                "showed",
                "dragged",
            ];
        let newState    = stateList[stateIndex];
        //console.log(`new state: ${newState}`);
        root.state      = newState;
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
        width: parent.width
        height: parent.height
        z: 8
        //visible: false

        Behavior on y {
            PropertyAnimation {
                id: positionAnimation
                easing.type: Easing.OutQuad
                duration: 250
            }
        }

        // onYChanged: console.log(`notification y ${y}`)

//        Component.onCompleted: StyleDebugTree.describe (
//           "contentRoot",
//            ["x", "y", "width", "height"],
//           this);

        /****************************************//**
         * Content
         ********************************************/

        DapQmlRectangle {
            id: content
            //anchors.centerIn: parent
            x: Math.round ((parent.width - width) / 2)
            y: Math.round ((parent.height - height) / 2)
            width: Math.round (contentSizer.width)
            height: Math.round (root.height - frameSizer.height + contentSizer.height)
            color: contentSizer.color
            radius: contentSizer.radius

//            Component.onCompleted: StyleDebugTree.describe (
//               "content",
//                ["x", "y", "width", "height"],
//               this);

            /****************************************//**
             * Text
             ********************************************/

            DapQmlLabel {
                id: textLabel
//                x: Math.round ((parent.width - width) / 2)
//                y: Math.round ((parent.height - height) / 2)
//                width: contentWidth
//                height: contentHeight
                anchors.fill: parent
                anchors.leftMargin: fontSize * 0.5
                anchors.rightMargin: fontSize * 0.5
                disableClicking: true
                wrapMode: Text.WordWrap
                text: root.internal.message
                //font.pixelSize: 14
                qss: "notification-text"

//                Component.onCompleted: StyleDebugTree.describe (
//                      "textLabel",
//                      ["x", "y", "width", "height"],
//                      this);
            }
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

//            Component.onCompleted: StyleDebugTree.describe (
//               "header",
//                ["x", "y", "width", "height"],
//               this);

            DapQmlLabel {
                anchors.fill: parent
                fontSize: parent.fontSize
                disableClicking: true
                text: root.internal.typeString
                qss: root.internal.type === 2 ? "c-label" : "c-error"
            }
        }

//        /****************************************//**
//         * Clickable area
//         ********************************************/

//        MouseArea {
//            anchors.fill: content
//            onClicked: root.sigBodyClicked();
//        }

    //        Component.onCompleted: StyleDebugTree.describe (
    //           "not-area",
    //            ["x", "y", "width", "height"],
    //           this);
    }

    /****************************************//**
     * Close button
     ********************************************/

    DapQmlRectangle {
        id: closeBtn
        anchors.top: contentRoot.top
        anchors.right: contentRoot.right
        anchors.topMargin: 0 - height * 0.175
        anchors.rightMargin: 0 - width * 0.175
        z: 10
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
    }

    /* clickable area */

    MouseArea {
        anchors.fill: closeBtn
        z: 11
        hoverEnabled: true
        onClicked: root.sigCloseClicked();
        onEntered: closeBtn.hovered = true
        onExited:  closeBtn.hovered = false
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

//        Component.onCompleted: StyleDebugTree.describe (
//           "contentRoot-shadow",
//            ["x", "y", "width", "height"],
//           this);
    }

    /****************************************//**
     * Swipe
     ********************************************/

    MultiPointTouchArea {
        id: swipeArea
        anchors.fill: parent
        anchors.leftMargin: closeBtn.width
        anchors.rightMargin: closeBtn.width
        enabled: root.internal.show
        touchPoints: [
            TouchPoint {
                id: point
                //onXChanged: root.internal._move()
                onPressedChanged: {
                    if (pressed === false)
                    {
                        if (Math.abs (root.swipeCtl.dragPosition) < 8)
                            root.sigBodyClicked();
                    }

                    root.swipeCtl.dragStart     = y;
                    root.swipeCtl.dragPosition  = 0;
                    root.swipeCtl.dragging      = pressed;
                }
                onYChanged: {
                    if (root.swipeCtl.dragging === false)
                        return;

                    /* calc new pos */
                    let newDragPos  = y - root.swipeCtl.dragStart;

                    /* set pos only if dragged up */
                    root.swipeCtl.dragPosition = newDragPos < 0 ? newDragPos : 0;

                    /* check if need to be closed */
                    if (root.swipeCtl.dragPosition < 0 - (contentRoot.height / 2))
                    {
                        root.setShow (false);
                        root.swipeCtl.dragging  = false;
                    }

                    //console.log(`pos: ${root.swipeCtl.dragPosition} : ${root.swipeCtl.dragStart} : ${y} : ${contentRoot.height / 2}`)
                }
            }
        ]
    }
}

/*-----------------------------------------*/
