import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: mainWindow

    readonly property FontLoader fontRobotoLight:
        FontLoader
        {
            source: "qrc:/res/fonts/roboto_light.ttf"
        }
    readonly property FontLoader fontRobotoRegular:
        FontLoader
        {
            source: "qrc:/res/fonts/roboto_regular.ttf"
        }

    readonly property FontLoader fontRobotoMedium:
        FontLoader
        {
            source: "qrc:/res/fonts/roboto_medium.ttf"
        }


    
    property alias iconLogotype: iconLogotype
    property alias frameLogotype: frameLogotype
    property alias menuWidth: columnMenuTab.width
    property alias menuTabWidget: menuTabWidget
    property alias screens: stackViewScreen
    
    anchors.fill: parent

    Row
    {
        id: rowMainWindow
        anchors.fill: parent

        Column
        {
            id: columnMenuTab
            width: 180 * pt
            height: rowMainWindow.height
            
            Rectangle
            {
                id: frameLogotype
                width: columnMenuTab.width
                height: 60 * pt
                color: "#070023"
                Image {
                    id: iconLogotype
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    width: 111 * pt 
                    height: 24 * pt
                    anchors.leftMargin: 24 * pt
                }
            }
        
            DapMenuTabWidget
            {
                id: menuTabWidget
                width: columnMenuTab.width
                height: columnMenuTab.height - frameLogotype.height
            }
        }
        
        Loader 
        {
            id: stackViewScreen
            clip: true
            height: rowMainWindow.height
            width: rowMainWindow.width - columnMenuTab.width
        }
    }
}
