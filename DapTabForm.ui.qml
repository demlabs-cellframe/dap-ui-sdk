import QtQuick 2.4
import QtQuick.Controls 2.0

Page {
    property DapTopPanelForm topPanelForm: DapTopPanelForm { }
    property DapScreenForm screenForm: DapScreenForm { }
    property alias separator: dapSeparator
    property DapRightPanelForm rightPanelForm: DapRightPanelForm { }

    anchors.fill: parent

    header: topPanelForm

    contentItem:
        Rectangle {

        Item
        {
            id: dapScreenForm
            data: screenForm
            height: parent.height
            anchors.left: parent.left
            anchors.right: separator.left
        }

        Rectangle {
            id: dapSeparator
            height: parent.height
            width: 3 * pt
            color: "green"
            anchors.right: dapRightPanelForm.left
        }

        Item
        {
            id: dapRightPanelForm
            data: rightPanelForm
            height: parent.height
            width: rightPanelForm.width
            anchors.right: parent.right
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
