import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

Button
{
    id: dapButton

    ///@detalis heightButton Button height.
    property int heightButton
    ///@detalis widthButton Button width.
    property int widthButton
    ///@detalis normalImageButton The picture on the Button is in normal state.
    property string normalImageButton
    ///@detalis hoverImageButton The image on the Button is in the mouseover state.
    property string hoverImageButton
    ///@detalis widthImageButton Image width.
    property int widthImageButton: 0
    ///@detalis heightImageButton Image height.
    property int heightImageButton: 0
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

    property string gradientColorNormal0 : "#f0f000"
    property string gradientColorNormal1 : "#f00000"
    property string gradientColorHovered0 : "#f0f0a0"
    property string gradientColorHovered1 : "#f000a0"

    property string shadowColor : "#1F242F"
//    property string shadowColor : currTheme.buttonShadow
    property string innerShadowColor : currTheme.buttonInnerShadow
    property string innerShadowPressColor : "#1F242F"

    property bool activeFrame : true

    property bool selected: true

    hoverEnabled: true

    ///@details empty default background
    background: Item { id: background }

    Component.onCompleted:
    {
        setButtonColors()
    }

    onEnabledChanged:
    {
        setButtonColors()
    }

    onSelectedChanged:
    {
        setButtonColors()
    }

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
                        GradientStop
                        {
                            id: grad0
                            position: 0;
                            color: gradientColorNormal0
                        }
                        GradientStop
                        {
                            id: grad1
                            position: 1;
                            color: gradientColorNormal1

                        }
                    }
            }

            color: !dapButton.activeFrame ?
                       "transparent" :
                       shadowColor

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
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: indentImageLeftButton
                source: dapButton.hovered ? hoverImageButton : normalImageButton
                width: widthImageButton
                height: heightImageButton
            }

            ParallelAnimation {
                id: mouseEnterAnim
                PropertyAnimation {
                    target: grad0
                    properties: "color"
                    to: gradientColorHovered0
                    duration: 100
                }
                PropertyAnimation {
                    target: grad1
                    properties: "color"
                    to: gradientColorHovered1
                    duration: 100
                }
            }
            ParallelAnimation {
                id: mouseExitedAnim
                PropertyAnimation {
                    target: grad0
                    properties: "color"
                    to: gradientColorNormal0
                    duration: 100
                }
                PropertyAnimation {
                    target: grad1
                    properties: "color"
                    to: gradientColorNormal1
                    duration: 100
                }
            }

            MouseArea{
                id: control
                anchors.fill: parent
                hoverEnabled: true

                onEntered:
                    if(dapButton.enabled)
                        mouseEnterAnim.start()
                onExited:
                    if(dapButton.enabled && !pressed)
                        mouseExitedAnim.start()

                onPressed: {
                    if(dapButton.enabled && dapButton.activeFrame)
                    {
                        shadowAnimPress.start()
                        shadow.visible = false
                    }
                }

                onReleased: {
                    if(dapButton.enabled)
                    {
                        shadowAnimRelease.start()
                        shadow.visible = true

                        if (control.containsMouse)
                            dapButton.clicked()
                        else
                            mouseExitedAnim.start()
                    }
                }
            }
        }
    DropShadow {
        id: shadow
        anchors.fill: dapBackgroundButton
        horizontalOffset: 2
        verticalOffset: 2
        radius: 8
        samples: 10
        cached: true
        color: shadowColor
        source: dapBackgroundButton
        }

    InnerShadow {
        property bool isPressed: false
        id: light
        anchors.fill: dapBackgroundButton
        horizontalOffset: 1
        verticalOffset: 1
        radius: 5
        samples: 10
        cached: true
        color: innerShadowColor
        source: dapBackgroundButton
        visible: dapBackgroundButton.visible

        PropertyAnimation {
            id: shadowAnimPress
            target: light
            properties: "color"
            to: innerShadowPressColor
            duration: 80
        }
        PropertyAnimation {
            id: shadowAnimRelease
            target: light
            properties: "color"
            to: innerShadowColor
            duration: 80
        }
    }

    function setButtonColors()
    {
        if (dapButton.enabled)
        {
            if (selected)
            {
                gradientColorNormal0 = currTheme.buttonColorNormalPosition0
                gradientColorNormal1 = currTheme.buttonColorNormalPosition1
                gradientColorHovered0 = currTheme.buttonColorHoverPosition0
                gradientColorHovered1 = currTheme.buttonColorHoverPosition1
            }
            else
            {
                gradientColorNormal0 = "#373A42"
                gradientColorNormal1 = "#373A42"
                gradientColorHovered0 = "#272A32"
                gradientColorHovered1 = "#272A32"
            }

        }
        else
        {

            gradientColorNormal0 = "#B3B1BC"
            gradientColorNormal1 = "#B3B1BC"
            gradientColorHovered0 = "#B3B1BC"
            gradientColorHovered1 = "#B3B1BC"
//            gradientColorNormal0 = currTheme.buttonColorNoActive
//            gradientColorNormal1 = currTheme.buttonColorNoActive
//            gradientColorHovered0 = currTheme.buttonColorNoActive
//            gradientColorHovered1 = currTheme.buttonColorNoActive
        }
    }
}
