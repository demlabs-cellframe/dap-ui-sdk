import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    id: dapFrame

    property alias frame: dapFrame
    property Item contenetItemScreen: Item { }

    anchors.fill: parent

    Item
    {
        id: contenetItemScreen
        data: contenetItemScreen
    }
}
