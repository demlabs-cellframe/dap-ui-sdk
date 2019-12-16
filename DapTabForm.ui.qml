import QtQuick 2.4
import QtQuick.Controls 2.0

Page {
    property alias topPanelForm: dapTopPanelForm
    property alias separator: separator
    property alias rghtPanelForm: dapRightPanelForm

    anchors.fill: parent

    header: DapTopPanelForm {
        id: dapTopPanelForm
        width: parent.width
        height: 64 * pt
    }

    contentItem: Item {
        anchors.fill: parent

        Row {
            anchors.fill: parent

            DapScreenForm {
                id: dapScreenForm
                height: parent.height

                anchors.right: separator.left
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: separator
                height: parent.height
                width: 1 * pt
                color: "green"

                anchors.right: dapRightPanelForm.left
                anchors.left: dapScreenForm.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }

            DapRightPanelForm {
                id: dapRightPanelForm
                visible: false
                height: parent.height
                width: 150 * pt
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }
        }
    }
}
