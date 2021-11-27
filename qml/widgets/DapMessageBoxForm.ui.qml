import QtQuick 2.4
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

Rectangle
{
    id: dapMessageBox

    property alias dapTitleText: titleText
    property alias dapContentText: contentText
    property alias dapButtonOk: buttonOk

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
        anchors.topMargin: 30 * pt
        anchors.leftMargin: 30 * pt
        anchors.rightMargin: 30 * pt
        anchors.bottomMargin: 32 * pt
        height: 172 * pt
        color: currTheme.backgroundElements

        Text
        {
            id: contentText
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: buttonOk.top
            font: fontMessage
            color: currTheme.textColor
            wrapMode: Text.WordWrap
        }

        DapButton
        {
            id: buttonOk
            widthButton: 78 * pt
            heightButton: 36 * pt
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            textButton: qsTr("OK")
            fontButton: fontButtonText
            horizontalAligmentText:Qt.AlignCenter
        }
    }
}
