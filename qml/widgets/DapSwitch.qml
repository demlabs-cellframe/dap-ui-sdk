import QtQuick 2.4
import QtGraphicalEffects 1.0

Item {
    id: toggleswitch

    property string imageOn: "qrc:/icons/switch_on.svg"
    property string imageOff: "qrc:/icons/switch_off.svg"
    property color backgroundColor: "#666"
    property color borderColor: "gray"
    property color shadowColor: "black"

    property int indicatorSize: 0

    width: 100; height: 50

    property bool checked: false

    signal toggled()

    property int leverMargin: 0

    property int leverOffPosition: leverMargin
    property int leverOnPosition: width - lever.width - leverMargin

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
        border.width: 2
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

    DapImageRender{
        id:lever
        x: leverOffPosition; y: leverMargin
        width: indicatorSize ? indicatorSize : background.height - leverMargin*2
        height: indicatorSize ? indicatorSize : background.height - leverMargin*2
        source: imageOff
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
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 100 }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: toggle()
    }


}
