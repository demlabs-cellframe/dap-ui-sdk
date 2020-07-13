//**************************************************************************** 
//              Implements a general view of the top tab bar.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: frameTopPanel
    
    ///@detalis Top panel frame.
    property alias dapFrame: frameTopPanel

    property Rectangle dapTopPanelBackground
    property Rectangle dapTopPanelRectangleBackground

    width: dapFrame.width
    height: dapFrame.height
    color: dapFrame.color

    Rectangle
    {
        id: unroundedTopBackground
        anchors.right: frameTopPanel.right
        width: dapTopPanelRectangleBackground.width
        height: dapTopPanelRectangleBackground.height
        color: dapTopPanelRectangleBackground.color
    }

    Rectangle
    {
        id: roundedTopBackground
        anchors.right: frameTopPanel.right
        width: dapTopPanelBackground.width
        height: dapTopPanelBackground.height
        radius: dapTopPanelBackground.radius
        color: dapTopPanelBackground.color

    }

}
