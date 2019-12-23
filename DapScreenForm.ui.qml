import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    id: dapFrame

    property alias frame: dapFrame
    property Item contenetItemScreen

    anchors.fill: parent

    Item {
        id: contenetItemScreen
        data: contenetItemScreen
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
