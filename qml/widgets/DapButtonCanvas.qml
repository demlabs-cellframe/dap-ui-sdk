import QtQuick
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

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
    ///@detalis widthImageButton DapImageRender width.
    property int widthImageButton: 0
    ///@detalis heightImageButton DapImageRender height.
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
    property string colorButtonTextNormal: currTheme.white
    ///@detalis colorButtonTextHover Button text color in hover state.
    property string colorButtonTextHover: currTheme.white
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

    property color defaultColorNormal0: currTheme.mainButtonColorNormal0
    property color defaultColorNormal1: currTheme.mainButtonColorNormal1
    property color defaultColorHovered0: currTheme.mainButtonColorHover0
    property color defaultColorHovered1: currTheme.mainButtonColorHover1
    property color defaultColorUnselectedNormal: currTheme.secondaryButtonColor
    property color defaultColorUnselectedHovered: "#272A32"
    property color defaultColorDisabled: currTheme.gray

    property color gradientColorNormal0: defaultColorNormal0
    property color gradientColorNormal1: defaultColorNormal1
    property color gradientColorHovered0: defaultColorHovered0
    property color gradientColorHovered1: defaultColorHovered1

    property color defaultColor: shadowColor

    property color shadowColor : currTheme.mainButtonShadow
//    property string shadowColor : currTheme.buttonShadow
    property color innerShadowColor : currTheme.buttonsShadowInner
    property color innerShadowPressColor : "#1F242F"

    property double opacityDropShadow: 0.44
    property double opacityInnerShadow: 0.3

    property alias dropShadow: shadow
    property alias innerShadow: light

    property bool activeFrame : true

    property bool selected: true

    //Blocked interactive in list view if pressed and unlock if released
    property bool listInteractivFlagDisabled: false
    property Item parentList

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
            property color grad0: gradientColorNormal0
            property color grad1: gradientColorNormal1
            id: dapBackgroundButton
            anchors.fill: parent

            /* mask source */
            Rectangle {
                id: contenMask
                anchors.fill: parent
                radius: dapBackgroundButton.radius
                visible: false
            }
            /* mask */
            OpacityMask {
                anchors.fill: content
                source: content
                maskSource: contenMask
            }
            Canvas{
                id: content
                anchors.fill: parent
                opacity: 0
                onPaint: {
                    var ctx = getContext("2d")

                    var gradient = ctx.createLinearGradient(0,parent.height/2,parent.width,parent.height/2)
                    gradient.addColorStop(0, dapBackgroundButton.grad0)
                    gradient.addColorStop(1, dapBackgroundButton.grad1)

                    ctx.fillStyle = gradient
                    ctx.fillRect(0,0,parent.width,parent.height)
                }
            }
            onGrad0Changed: {
                content.requestPaint()
            }
            onGrad1Changed: {
                content.requestPaint()
            }

            color: dapButton.activeFrame ?
                       defaultColor :
                       "transparent"

            Component.onCompleted: content.requestPaint()

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
                bottomPadding: CURRENT_OS === "win" ? 2 : 0
                anchors.rightMargin: indentTextRight
                color: control.containsMouse ? colorButtonTextHover : colorButtonTextNormal
                text: qsTr(textButton)
            }

            ///button picture
            DapImageRender
            {
                id: img
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
                    target: dapBackgroundButton
                    properties: "grad0"
                    to: gradientColorHovered0
                    duration: 100
                }
                PropertyAnimation {
                    target: dapBackgroundButton
                    properties: "grad1"
                    to: gradientColorHovered1
                    duration: 100
                }
            }
            ParallelAnimation {
                id: mouseExitedAnim
                PropertyAnimation {
                    target: dapBackgroundButton
                    properties: "grad0"
                    to: gradientColorNormal0
                    duration: 100
                }
                PropertyAnimation {
                    target: dapBackgroundButton
                    properties: "grad1"
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
                        if(listInteractivFlagDisabled)
                            parentList.interactive = false
                        shadowAnimPress.start()
                        shadow.visible = false
                    }
                }

                onReleased: {
                    if(dapButton.enabled)
                    {
                        shadowAnimRelease.start()
                        shadow.visible = true

                        if(listInteractivFlagDisabled)
                            parentList.interactive = true

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
        radius: 7
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
        radius: 4
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
                gradientColorHovered0 = defaultColorHovered0
                gradientColorHovered1 = defaultColorHovered1
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
