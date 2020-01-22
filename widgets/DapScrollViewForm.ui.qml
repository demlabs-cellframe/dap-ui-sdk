import QtQuick 2.4

MouseArea
{
    id: dapScrollMouseArea

    // Icons for scroll button
    property string scrollDownButtonImageSource
    property string scrollDownButtonHoveredImageSource
    property string scrollUpButtonImageSource
    property string scrollUpButtonHoveredImageSource

    // ListView to attach the scroll button
    property ListView viewData

    property alias scrollMouseArea: dapScrollMouseArea
    property alias scrollButton: dapScrollButton
    property alias scrollButtonImage: dapScrollButtonImage

    anchors.fill: parent
    hoverEnabled: true

    MouseArea
    {
        id: dapScrollButton
        width: 36 * pt
        height: width
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10 * pt
        anchors.topMargin: 10 * pt
        anchors.rightMargin: 10 * pt
        hoverEnabled: true
        visible: false

        Image
        {
            id: dapScrollButtonImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: scrollDownButtonImageSource
            sourceSize.height: parent.height
            sourceSize.width: parent.width
        }
    }
}
