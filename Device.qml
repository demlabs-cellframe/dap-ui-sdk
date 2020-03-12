import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: dapDevice
    
//    property string device: dp()
    //TODO: Temporary editing
    property string device: "desktop"
    property int dpi: Screen.pixelDensity * 25.4
    
    function dp(){
        if(dpi < 120) {
            device = "desktop"
        } else {
            device = "mobile"
        }
    }
}
