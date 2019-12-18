import QtQuick 2.4
import QtQuick.Controls 2.0

Page {
    property DapTopPanelForm topPanelForm: undefined
    property alias separator: separator
    property alias rghtPanelForm: dapRightPanelForm

    anchors.fill: parent

    header: topPanelForm

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
                width: 400 * pt
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }
        }
    }
}







/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
