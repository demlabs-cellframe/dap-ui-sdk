import QtQuick

Item {
    property alias source: image.source
    property int innerWidth
    property int innerHeight
    property alias img: image
    property alias mirror: image.mirror

    width: innerWidth
    height: innerHeight
    Image
    {
        anchors.fill: parent
        id: image
        sourceSize.width: innerWidth
        sourceSize.height: innerHeight
        fillMode: Image.PreserveAspectFit
        visible: true
        antialiasing: true
    }
    Rectangle {
        id: overlay
        anchors.fill: image
        color: "#FFFF0000"
        opacity: 0.5
        visible: false
    }
}
