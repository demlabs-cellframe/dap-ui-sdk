import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Controls as OldControls
import QtQuick.Controls.Styles 1.4
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts

Rectangle
{
    id: dapMessageBox

    property alias dapTitleText: titleText
    property alias dapContentText: contentText
    property alias dapContentInput: contentInput
    property alias dapButtonOk: buttonOk
    property alias dapButtonBack: buttonBack

    ///@details fontMessage Font setting.
    property font fontMessage
    ///@details fontButtonText Font setting.
    property font fontButtonText
    color: currTheme.backgroundElements
    radius: currTheme.frameRadius

    layer.enabled: true
    layer.effect:
        DropShadow
        {
            horizontalOffset: 3
            verticalOffset: 3
            radius: 13
            samples: 17
            color: currTheme.shadowColor
        }

    Rectangle
    {
        id: frameTitle
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 44 
        color: currTheme.backgroundMainScreen
        radius: currTheme.frameRadius

        Text
        {
            id: titleText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 30 
            anchors.rightMargin: 30 
            font: fontMessage
            color: currTheme.textColor
        }
        Rectangle
        {
            anchors
            {
                top: titleText.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            height: 10 
            color: currTheme.backgroundMainScreen
        }
    }

    Rectangle
    {
        id: frameContent
        anchors.top: frameTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 20 
        anchors.leftMargin: 15 
        anchors.rightMargin: 15 
        anchors.bottomMargin: 32 
        height: 172 
        color: currTheme.backgroundElements

        Text
        {
            id: contentText
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            font: fontMessage
            color: currTheme.textColor
            wrapMode: Text.WordWrap
        }
        OldControls.TextField
        {
            id:contentInput
            anchors.top: contentText.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30 
            anchors.topMargin: 20 
            visible: false
            font: fontMessage
            clip:true
            style:
                TextFieldStyle
                {
                    textColor: currTheme.textColor
                    placeholderTextColor: currTheme.textColor
                    background:
                        Rectangle
                        {
                            border.width: 0
                            color: currTheme.backgroundElements
                        }
                }
            Rectangle //bottom line
            {
                anchors
                {
                    left: parent.left
                    right: parent.right
                    top: parent.bottom
                }
                height: 1 
                color: currTheme.borderColor
            }
        }

        DapButton
        {
            id: buttonBack
            widthButton: 78 
            heightButton: 36 
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            textButton: qsTr("Back")
            fontButton: fontButtonText
            horizontalAligmentText:Qt.AlignCenter
            visible: contentInput.visible
        }

        DapButton
        {
            id: buttonOk
            widthButton: 78 
            heightButton: 36 
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            textButton: qsTr("Ok")
            fontButton: fontButtonText
            horizontalAligmentText:Qt.AlignCenter
        }

    }
}
