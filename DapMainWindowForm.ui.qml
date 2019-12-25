//****************************************************************************
//                  Implements the main application window.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: mainWindow

    ///@detalis Roboto light font downloader.
    readonly property FontLoader dapFontRobotoLight:
        FontLoader
        {
            source: "qrc:/res/fonts/roboto_light.ttf"
        }
    ///@detalis Roboto regular font downloader.
    readonly property FontLoader dapFontRobotoRegular:
        FontLoader
        {
            source: "qrc:/res/fonts/roboto_regular.ttf"
        }
    ///@detalis Roboto medium font downloader.
    readonly property FontLoader dapFontRobotoMedium:
        FontLoader
        {
            source: "qrc:/res/fonts/roboto_medium.ttf"
        }

    ///@detalis Logo icon.
    property alias dapIconLogotype: iconLogotype
    ///@detalis Logo frame.
    property alias dapFrameLogotype: frameLogotype
    ///@detalis Menu bar width.
    property alias dapMenuWidth: columnMenuTab.width
    ///@detalis Menu bar widget.
    property alias dapMenuTabWidget: menuTabWidget
    ///@detalis Screen tab loader.
    property alias dabScreens: stackViewTabs
    
    anchors.fill: parent

    // The horizontal location of the virtual menu column and tab view loader
    Row
    {
        id: rowMainWindow

        anchors.fill: parent

        // Virtual logo column frame and menu bar
        Column
        {
            id: columnMenuTab
            width: 180 * pt
            height: rowMainWindow.height
            
            // Logotype
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

            // Menu bar
            DapMenuTabWidget
            {
                id: menuTabWidget
                width: columnMenuTab.width
                height: columnMenuTab.height - frameLogotype.height
            }
        }
        
        // Screen tab loader
        Loader 
        {
            id: stackViewTabs
            clip: true
            height: rowMainWindow.height
            width: rowMainWindow.width - columnMenuTab.width
        }
    }
}
