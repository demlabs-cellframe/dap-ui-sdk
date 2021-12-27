import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: toggleswitch

    property string imageOn: "qrc:/icons/switch_on.png"
    property string imageOff: "qrc:/icons/switch_off.png"
    property color backgroundColor: "#666"
    property color borderColor: "gray"
    property color shadowColor: "black"

    width: 100; height: 50

    property bool checked: false

    signal toggled()

    property int leverOffPosition: 2
    property int leverOnPosition: width - lever.width - 2

    function toggle() {
        if (toggleswitch.state == "on")
        {
            toggleswitch.state = "off";
            toggled()
        }
        else
        {
            toggleswitch.state = "on";
            toggled()
        }
    }

    Component.onCompleted:
    {
        positionTransition.enabled = false
        if (checked)
            toggleswitch.state = "on";
        else
            toggleswitch.state = "off";
        positionTransition.enabled = true
    }

    Rectangle {
        id: background
        implicitWidth: parent.width
        implicitHeight: parent.height
        radius: height*0.5
        color: backgroundColor
        border.color: borderColor
        border.width: leverOffPosition
    }

    InnerShadow
    {
        id: shadow
        anchors.fill: background
        horizontalOffset: 4
        verticalOffset: 4
        radius: 10
        fast: true
        cached: true
        color: shadowColor
        source: background
        visible: background.visible
    }

    Image {
        id: lever
        x: leverOffPosition; y: 2
        width: background.height-4
        height: background.height-4
        source: imageOff
        fillMode: Image.PreserveAspectFit
    }

    states: [
        State {
            name: "on"
            PropertyChanges { target: lever; x: leverOnPosition }
            PropertyChanges { target: lever; source: imageOn }
            PropertyChanges { target: toggleswitch; checked: true }
        },
        State {
            name: "off"
            PropertyChanges { target: lever; x: leverOffPosition }
            PropertyChanges { target: lever; source: imageOff }
            PropertyChanges { target: toggleswitch; checked: false }
        }
    ]

    transitions: Transition {
        id: positionTransition
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 150 }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: toggle()
    }

}
