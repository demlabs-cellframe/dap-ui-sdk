import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle {
    property alias borderMenuTab: dapListViewMenuTab
    property alias viewMenuTab: dapListViewMenuTab
    
    id: rectangleMenuTabBorder
    color: "#070023"
    
    ListView 
    {
        id: dapListViewMenuTab
        anchors.fill: parent
        
        
        delegate:  test
           
        spacing: 3 * pt
        clip: true
        interactive: false
    }
    focus: true
}









/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
