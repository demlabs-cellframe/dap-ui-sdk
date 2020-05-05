import QtQuick 2.4

MouseArea
{
    ///@detalis scrollDownButtonImageSource Icons for non-focus scroll button with down arrow
    property string scrollDownButtonImageSource
    ///@detalis scrollDownButtonHoveredImageSource Icons for focus scroll button with down arrow
    property string scrollDownButtonHoveredImageSource
    ///@detalis scrollUpButtonImageSource Icons for non-focus scroll button with up arrow
    property string scrollUpButtonImageSource
    ///@detalis scrollUpButtonHoveredImageSource Icons for focus scroll button with up arrow
    property string scrollUpButtonHoveredImageSource
    ///@detalis viewData Flicable item to attach the scroll button
    property Flickable viewData
    ///@detalis scrollButton ScrollButton reference
    property alias scrollButton: dapScrollButton
    ///@detalis scrollButtonImage ScrollButtonImage reference
    property alias scrollButtonImage: dapScrollButtonImage
    ///@detalis scrollButtonTopMargin Top Margin from parent.top
    property int scrollButtonTopMargin
    ///@detalis scrollButtonBottomMargin Bottom Margin from parent.top
    property int scrollButtonBottomMargin
    ///@detalis scrollButtonLeftMargin Left Margin from parent.top
    property int scrollButtonLeftMargin
    ///@detalis scrollButtonRightMargin Right Margin from parent.top
    property int scrollButtonRightMargin
    ///@detalis scrollButtonArrowUp Sign of arrow direction on scrollButton
    property bool scrollButtonArrowUp
    ///@detalis scrollButtonVisible Sign of visibility of scrollButton
    property bool scrollButtonVisible
    ///@detalis mouseAtArrow Sign of mouse pointer position into scrollButton
    property bool mouseAtArrow

    id: dapScrollButton
    width: 36 * pt
    height: width

    hoverEnabled: true
    propagateComposedEvents: true
    visible: scrollButtonVisible

    Image
    {
        id: dapScrollButtonImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        sourceSize.height: parent.height
        sourceSize.width: parent.width
        visible: parent.visible
    }
}
