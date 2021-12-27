import QtQuick 2.1
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.5

Button {
    id: root
    width: 354
    height: 60

    function isStill()
    {
        return !root.hovered && !root.down && !root.checked;
    }

    /* disable background */
    background: Rectangle {
        color: 'transparent'
    }

    /* gradient rectangle */
    contentItem: Rectangle {
        id: rect
        anchors.fill: parent
        visible: false

        LinearGradient {
            anchors.fill: parent
            start: Qt.point(root.width, 0)
            end: Qt.point(0, root.height)
            gradient: Gradient {
                    GradientStop { position: 0.0; color: isStill() ? "#E50778" : "#F53172" }
                    GradientStop { position: 1.0; color: isStill() ? "#E804C3" : "#E62CC7" }
                }
        }

        /* text label */
        Text {
            anchors.centerIn: parent
            text: root.text
            color: 'white'
            font.family: "Roboto"
            font.pixelSize: 14
            font.weight: Font.DemiBold
        }

    }

    /* rounded mask */
    OpacityMask {
        id: mask
        cached: true
        anchors.fill: rect
        source: rect
        maskSource: Rectangle {
            width: root.width
            height: root.height
            radius: 50
        }
    }

    /* outline shadow */
    DropShadow {
        anchors.fill: mask
        horizontalOffset: 4
        verticalOffset: 4
        radius: 25
        samples: 17
        color: "#40142724"
        source: mask
        visible: isStill()
    }

    /* inline shadow */
    InnerShadow {
        anchors.fill: mask
        horizontalOffset: 4
        verticalOffset: 4
        radius: 17
        samples: 17
        color: "#65003A43"
        source: mask
        visible: !isStill()
    }
}
