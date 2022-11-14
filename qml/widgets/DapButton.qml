import QtQuick 2.9
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
    property alias imageMirror: img.mirror

    property color gradientColorNormal0: "#f0f000"
    property color gradientColorNormal1: "#f00000"
    property color gradientColorHovered0: "#f0f0a0"
    property color gradientColorHovered1: "#f000a0"

    property color defaultColorNormal0: currTheme.buttonColorNormalPosition0
    property color defaultColorNormal1: currTheme.buttonColorNormalPosition1
    property color defaultColorHovered0: currTheme.buttonColorHoverPosition0
    property color defaultColorHovered1: currTheme.buttonColorHoverPosition1
    property color defaultColorUnselectedNormal: "#373A42"
    property color defaultColorUnselectedHovered: "#272A32"
    property color defaultColorDisabled: "#B3B1BC"

    property color defaultColor: shadowColor

    property color shadowColor : "#1F242F"
//    property string shadowColor : currTheme.buttonShadow
    property color innerShadowColor : currTheme.buttonInnerShadow
    property color innerShadowPressColor : "#1F242F"

    property double opacityDropShadow: 1.0
    property double opacityInnerShadow: 1.0

    property alias dropShadow: shadow
    property alias innerShadow: light

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
                       defaultColor

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
                bottomPadding: OS_WIN_FLAG ? 2 : 0
                anchors.rightMargin: indentTextRight
                color: currTheme.textColor
                text: qsTr(textButton)
            }

            ///button picture
            Image
            {
                id: img
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: indentImageLeftButton
                source: dapButton.hovered ? hoverImageButton : normalImageButton
                mipmap: true
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
        opacity: opacityDropShadow
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
        opacity: opacityInnerShadow
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
                gradientColorNormal0 = defaultColorNormal0
                gradientColorNormal1 = defaultColorNormal1
                gradientColorHovered0 = defaultColorHovered0
                gradientColorHovered1 = defaultColorHovered1
            }
            else
            {
                gradientColorNormal0 = defaultColorUnselectedNormal
                gradientColorNormal1 = defaultColorUnselectedNormal
                gradientColorHovered0 = defaultColorUnselectedHovered
                gradientColorHovered1 = defaultColorUnselectedHovered
            }

        }
        else
        {

            gradientColorNormal0 = defaultColorDisabled
            gradientColorNormal1 = defaultColorDisabled
            gradientColorHovered0 = defaultColorDisabled
            gradientColorHovered1 = defaultColorDisabled
//            gradientColorNormal0 = currTheme.buttonColorNoActive
//            gradientColorNormal1 = currTheme.buttonColorNoActive
//            gradientColorHovered0 = currTheme.buttonColorNoActive
//            gradientColorHovered1 = currTheme.buttonColorNoActive
        }
    }
}
