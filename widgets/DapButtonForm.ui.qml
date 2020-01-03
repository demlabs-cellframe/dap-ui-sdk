import QtQuick 2.0
import QtQuick.Controls 2.0

Button {
    ///@detalis heightButton Button height.
    property int heightButton
    ///@detalis widthButton Button width.
    property int widthButton
    ///@detalis normalImageButton The picture on the Button is in normal state.
    property string normalImageButton
    ///@detalis hoverImageButton The image on the Button is in the mouseover state.
    property string hoverImageButton
    ///@detalis widthImageButton Image width.
    property int widthImageButton
    ///@detalis heightImageButton Image height.
    property int heightImageButton
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
    property alias fontButton:buttonText.font
    ///@detalis existenceImage Indicates the presence of an image.
    property bool existenceImage: true
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

    id: dapButton

    contentItem: 
        Rectangle 
        {
            id: dapBackgroundButton
            anchors.fill: parent
            color: dapButton.hovered ? colorBackgroundHover : colorBackgroundNormal
            implicitWidth: widthButton
            implicitHeight: heightButton
            border.color: borderColorButton
            border.width: borderWidthButton
            ///button text
            Text 
            {
                id: buttonText
                anchors.fill: parent
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignRight
                anchors.rightMargin: indentTextRight
                color: dapButton.hovered ? colorButtonTextHover : colorButtonTextNormal
                text: qsTr(textButton)
            }
    
            ///button picture
            Image
            {
                id: iconNewWallet
                anchors.verticalCenter: if(existenceImage)parent.verticalCenter
                anchors.left: if(existenceImage)parent.left
                anchors.leftMargin:if(existenceImage) indentImageLeftButton
                source: if(existenceImage) dapButton.hovered ? hoverImageButton : normalImageButton
                width: if(existenceImage)widthImageButton
                height:if(existenceImage) heightImageButton
            }
        }
}
