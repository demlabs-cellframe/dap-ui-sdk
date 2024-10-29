import QtQuick 2.0
import QtGraphicalEffects 1.0

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
    ColorOverlay {
        id: overlay
        anchors.fill: image
        source: image
        color: "#FFFF0000"
        visible: false
      }
}
