//****************************************************************************
//                  Implements the main application window.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: mainWindow

    ///@detalis Logotype.
    property Item dapLogotype
    ///@detalis Menu bar widget.
    property Item dapMenuWidget
    ///@detalis screen downloader widget.
    property Item dapScreensWidget
    ///@detalis status bar widget.
    property Item dapStatusBarWidget

    property Rectangle dapLeftMenuBackground
    property Rectangle dapLeftRectangleBackground
    
    anchors.fill: parent

    Rectangle
    {
        id: leftBackground
        width: dapLeftRectangleBackground.width
        height: dapLeftRectangleBackground.height
        color: dapLeftRectangleBackground.color
    }
    // The vertical location of virtual menu with Screen downloader widget and Status Bar
    Column
    {
        id: columnMainWindow
        anchors.fill: parent

        // The horizontal location of the virtual menu column and tab view loader
        Row
        {
            id: rowMainWindow
            width: columnMainWindow.width
            height: columnMainWindow.height - dapStatusBarWidget.height

            Rectangle
            {
                id: leftMenuBackground
                width: dapLeftMenuBackground.width
                height: dapLeftMenuBackground.height
                color: dapLeftMenuBackground.color
                radius: dapLeftMenuBackground.radius

                // Virtual logo column frame and menu bar
                Column
                {
                    id: columnMenuTab
                    height: rowMainWindow.height
                    // Logotype widget
                    Item
                    {
                        id: logotype
                        data: dapLogotype
                        width: columnMenuTab.width
                        height: dapLogotype.height
                    }
                    // Menu bar widget
                    Item
                    {
                        id: menuWidget
                        data: dapMenuWidget
                        width: dapMenuWidget.width
                        height: rowMainWindow.height - logotype.height
                    }
                }
            }


                // Screen downloader widget
                Item
                {
                    id: screens
                    data: dapScreensWidget
                    height: rowMainWindow.height
                    width: rowMainWindow.width - columnMenuTab.width
                }
        }
       Item
        {
            id: statusBarWidget
            data: dapStatusBarWidget
            width: columnMainWindow.width
            height: dapStatusBarWidget.height
        }
    }
}
