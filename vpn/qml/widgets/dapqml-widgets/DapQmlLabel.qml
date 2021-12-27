import QtQuick 2.1

Text {
    property string weight
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font {
        weight: weight
        family: "Lato"
        pixelSize: 20
      }
}
