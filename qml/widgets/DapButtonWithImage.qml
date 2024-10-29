import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Button {
    property string pathImage: ""
    property bool isLeftImage: true
    property string buttonText: ""
    property int widthImage: 24
    property int heightImage: 24
    property color textCollor: currTheme.white
    property color backgroundColor: currTheme.buttonGray
    property color hoverBackgroundColor: currTheme.buttonLightGray
    property int backgroundRadius: 4

    contentItem: RowLayout {
        spacing: 4
        Item {
            Layout.fillWidth: true
        }
        Image
        {
            mipmap: true
            source: pathImage
            visible: isLeftImage
            width: isLeftImage ? widthImage : 0
            height: isLeftImage ? heightImage : 0
        }

        Text
        {
            text: buttonText
            color: textCollor
            font: mainFont.dapFont.regular14
            verticalAlignment: Text.AlignVCenter
        }

        Image
        {
            mipmap: true
            source: pathImage
            visible: !isLeftImage
            width: !isLeftImage ? widthImage : 0
            height: !isLeftImage ? heightImage : 0
        }

        Item
        {
            Layout.fillWidth: true
        }
    }

    background: Rectangle
    {
        id: backgroundRect
        color: !hovered ? backgroundColor : hoverBackgroundColor
        radius: backgroundRadius
    }
}
