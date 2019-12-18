import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    property alias frameMenuTab: dapFrameMenuTab
    property alias viewMenuTab: dapListViewMenuTab
    property string puthScreen: ""


    id: dapFrameMenuTab
    color: "#070023"

    ListView
    {
        id: dapListViewMenuTab
        anchors.fill: parent
        delegate: itemMenuTabDelegate
        spacing: 3 * pt
        clip: true
        interactive: false
    }
    focus: true
}
