import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as OldControls
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

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
    radius: currTheme.radiusRectangle

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
        height: 44 * pt
        color: currTheme.backgroundMainScreen
        radius: currTheme.radiusRectangle

        Text
        {
            id: titleText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 30 * pt
            anchors.rightMargin: 30 * pt
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
            height: 10 * pt
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
        anchors.topMargin: 20 * pt
        anchors.leftMargin: 15 * pt
        anchors.rightMargin: 15 * pt
        anchors.bottomMargin: 32 * pt
        height: 172 * pt
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
            height: 30 * pt
            anchors.topMargin: 20 * pt
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
                height: 1 * pt
                color: currTheme.borderColor
            }
        }

        DapButton
        {
            id: buttonBack
            widthButton: 78 * pt
            heightButton: 36 * pt
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
            widthButton: 78 * pt
            heightButton: 36 * pt
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            textButton: qsTr("Ok")
            fontButton: fontButtonText
            horizontalAligmentText:Qt.AlignCenter
        }

    }
}
