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
    ///@detalis Menu bar width.
    property alias dapMenuWidth: columnMenuTab.width
    property alias dapLogotypeHeight: logotype.height
    ///@detalis Menu bar widget.
    property Item dapMenuWidget
    ///@detalis screen downloader widget.
    property Item dabScreensWidget
    
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
            height: rowMainWindow.height
            // Logotype widget
            Item
            {
                id: logotype
                data: dapLogotype
                width: columnMenuTab.width
            }
            // Menu bar widget
            Item
            {
                id: menuWidget
                data: dapMenuWidget
                width: columnMenuTab.width
                height: columnMenuTab.height - logotype.height
            }
        }
        
        // Screen downloader widget
        Item
        {
            id: screens
            data: dabScreensWidget
            height: rowMainWindow.height
            width: rowMainWindow.width - columnMenuTab.width
        }
    }
}
