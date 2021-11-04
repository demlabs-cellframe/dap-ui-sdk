import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

Button
{
    ///@detalis heightButton Button height.
    property int heightButton
    ///@detalis widthButton Button width.
    property int widthButton
    ///@detalis normalImageButton The picture on the Button is in normal state.
    property string normalImageButton
    ///@detalis hoverImageButton The image on the Button is in the mouseover state.
    property string hoverImageButton
    ///@detalis widthImageButton Image width.
    property int widthImageButton: 0 * pt
    ///@detalis heightImageButton Image height.
    property int heightImageButton: 0 * pt
    ///@detalis indentImageLeftButton: Indentation of the image from the left edge.
    property int indentImageLeftButton
    ///@detalis colorBackgroundNormal Button background color in normal state.
    property string colorBackgroundNormal
    ///@detalis colorBackgroundHover Button background color in hover state.
    property string colorBackgroundHover
    ///@detalis textButton Text button.
    property string textButton
    ///@detalis colorButtonTextNormal Button text color in normal state.
    property string colorButtonTextNormal
    ///@detalis colorButtonTextHover Button text color in hover state.
    property string colorButtonTextHover
    ///@detalis indentTextRight: Indentation of the text from the right edge.
    property int indentTextRight
    ///@detalis fontButton Font setting.
    property alias fontButton: buttonText.font
    ///@detalis horizontalAligmentText Horizontal alignment.
    property alias horizontalAligmentText:buttonText.horizontalAlignment
    ///@detalis colorBackgroundButton This property overrides the background color.
    property alias colorBackgroundButton: dapBackgroundButton.color
    ///@detalis colorTextButton This property overrides the color of the text.
    property alias colorTextButton: buttonText.color
    ///@detalis borderColorButton Sets the color of the border.
    property string borderColorButton
    ///@detalis borderWidthButton Sets the width of the border.
    property int borderWidthButton
    ///@detalis dapHorizontalAlignment Horizontal text alignment.
    property alias dapHorizontalAlignment: buttonText.horizontalAlignment
    ///@details button background radius
    property alias radius: dapBackgroundButton.radius
    property string shadowColor : "#2A2C33"
    property bool activeFrame : true


    id: dapButton

    ///@details empty default background
    background: Item { id:background }

    contentItem:
        Rectangle
        {
            id: dapBackgroundButton
            anchors.fill: parent
            LinearGradient
            {
                anchors.fill: parent
                source: parent
                start: Qt.point(0,parent.height/2)
                end: Qt.point(parent.width,parent.height/2)
                gradient:
                    Gradient {
                        GradientStop { position: 0; color: dapButton.hovered?"#E62083":"#C91D73"}
                        GradientStop { position: 1; color: dapButton.hovered?"#E62263":"#D51F5D"}
                    }
            }

            color: !dapButton.activeFrame? "transparent " :
                   dapButton.enabled ?
                   dapButton.hovered ? currTheme.buttonColorHover :
                                       currTheme.buttonColorNormal :
                                       currTheme.buttonColorNoActive

            implicitWidth: widthButton
            implicitHeight: heightButton
            border.color: borderColorButton
            border.width: borderWidthButton
            radius: currTheme.radiusButton

            ///button text
            Text
            {
                id: buttonText
                anchors.fill: parent
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
                anchors.rightMargin: indentTextRight
                color: currTheme.textColor
                text: qsTr(textButton)
            }

            ///button picture
            Image
            {
                id: iconNewWallet
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: indentImageLeftButton
                source: dapButton.hovered ? hoverImageButton : normalImageButton
                width: widthImageButton
                height: heightImageButton
            }
        }
    DropShadow {
//                Layout.alignment: buttonSend
        anchors.fill: dapBackgroundButton
        horizontalOffset: 2
        verticalOffset: 2
        radius: 10
        samples: 32
        color: shadowColor
        source: dapBackgroundButton
        smooth: true
        }
        MouseArea {
            enabled: false
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
        }

}
