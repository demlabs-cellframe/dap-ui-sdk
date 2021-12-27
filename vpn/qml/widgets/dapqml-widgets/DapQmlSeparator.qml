import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.5

Rectangle {
    id: root
    width: 354
    height: 4
    color: "transparent"

    /* holder */
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"

        /* dark rectangle */
        Rectangle {
            id: rectUp
            anchors.fill: parent
            color: "transparent"
            visible: false

            /* bright rectangle */
            Rectangle {
                id: rectDown
                anchors.fill: parent
                color: "#F7F8FA"
                visible: false
            }

            /* bright rounded mask */
            OpacityMask {
                id: mask1
                cached: true
                anchors.fill: rectDown
                source: rectDown
                visible: false
                maskSource: Rectangle {
                    width: root.width
                    height: root.height
                    radius: 50
                }
            }

            /* bright shadow */
            InnerShadow {
                anchors.fill: mask1
                horizontalOffset: -2
                verticalOffset: -2
                radius: 2
                samples: 16
                color: "#FFFFFF"
                source: mask1
            }
        }

        /* dark rounded mask */
        OpacityMask {
            id: mask2
            cached: true
            anchors.fill: rectUp
            source: rectUp
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: 50
            }
        }

        /* dark shadow */
        InnerShadow {
            anchors.fill: mask2
            horizontalOffset: 2
            verticalOffset: 2
            radius: 3
            samples: 16
            color: "#20000014"
            source: mask2
        }
    }

//    /* bright rectangle */
//    Rectangle {
//        id: rectColor
//        anchors.fill: root
//        color: "green"//"#F7F8FA"
//        visible: false
//    }

//    /* bright rounded mask */
//    OpacityMask {
//        id: mask1
//        cached: true
//        anchors.fill: rectColor
//        source: rectColor
//        maskSource: Rectangle {
//            width: root.width
//            height: root.height
//            radius: 50
//        }
//    }
}
