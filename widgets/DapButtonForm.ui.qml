import QtQuick 2.0
import QtQuick.Controls 2.0

Button {
    ///@detalis heightButton Button height.
    property int heightButton: 36 * pt
    ///@detalis widthButton Button width.
    property int widthButton: 120 * pt
    ///@detalis normalImageButton The picture on the Button is in normal state.
    property string normalImageButton: "qrc:/res/icons/new-wallet_icon_dark.png"
    ///@detalis hoverImageButton The image on the Button is in the mouseover state.
    property string hoverImageButton: "qrc:/res/icons/new-wallet_icon_dark_hover.png"
    ///@detalis widthImageButton Image width.
    property int widthImageButton: 28 * pt
    ///@detalis heightImageButton Image height.
    property int heightImageButton: 28 * pt
    ///@detalis indentImageLeftButton: Indentation of the image from the left edge.
    property int indentImageLeftButton: 10 * pt
    ///@detalis colorBackgroundNormal Button background color in normal state.
    property string colorBackgroundNormal:"#070023"
    ///@detalis colorBackgroundHover Button background color in hover state.
    property string colorBackgroundHover: "#D51F5D"
    ///@detalis textButton Text button.
    property string textButton: "New Wallet"
    ///@detalis colorButtonTextNormal Button text color in normal state.
    property string colorButtonTextNormal: "#FFFFFF"
    ///@detalis colorButtonTextHover Button text color in hover state.
    property string colorButtonTextHover: "#FFFFFF"
    ///@detalis indentTextRight: Indentation of the text from the right edge.
    property int indentTextRight: 20 * pt
    ///@detalis fontSizeButton Font size.
    property int fontSizeButton: 14 * pt
    ///@detalis existenceImage Indicates the presence of an image.
    property bool existenceImage:true
    ///@detalis horizontalAligmentText Horizontal alignment.
    property alias horizontalAligmentText:templateText.horizontalAlignment
    ///@detalis colorBackgroundButton This property overrides the background color.
    property alias colorBackgroundButton: dapBackgroundButton.color
    ///@detalis colorTextButton This property overrides the color of the text.
    property alias colorTextButton: templateText.color
    ///@detalis borderColorButton Sets the color of the border.
    property string borderColorButton: "#000000"
    ///@detalis borderWidthButton Sets the width of the border.
    property int borderWidthButton: 0

    id: dapButton

    contentItem: Rectangle {
        id: dapBackgroundButton
        anchors.fill: parent
        color: dapButton.hovered ? colorBackgroundHover : colorBackgroundNormal
        implicitWidth: widthButton
        implicitHeight: heightButton
        border.color: borderColorButton
        border.width: borderWidthButton
        ///button text
        Text {
            id: templateText
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignRight
            anchors.rightMargin: indentTextRight
            font.pixelSize: fontSizeButton
            font.family: "Roboto"
            font.weight: Font.Normal
            color: dapButton.hovered ? colorButtonTextHover : colorButtonTextNormal
            text: qsTr(textButton)
        }

        ///button picture
        Image {
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
