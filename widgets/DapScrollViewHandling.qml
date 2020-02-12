import QtQuick 2.0

MouseArea
{
    id: dapScrollMouseArea

    ///@detalis scrollDirectionUp Direction of arrow at scrollButton
    property bool scrollDirectionUp: true
    ///@detalis scrollVisible Is scrollButton need
    property bool scrollVisible: false
    ///@detalis scrollMouseAtArrow Is mouse above the scrollButton
    property bool scrollMouseAtArrow: false
    ///@detalis viewData Flickable to attach the scroll button
    property Flickable viewData

    anchors.fill: parent
    //To use entered and exited signals
    hoverEnabled: true


    //If mouse pointer go to parent area
    onEntered:
    {
        if(containsMouse)
        {
            //ScrollButton is needed to be visible only if list lenght more than its height
            scrollVisible = viewData.contentHeight > viewData.height;
            //If user see the first element
            if(viewData.atYBeginning)
                scrollDirectionUp = false;
            //If user see the last element
            if(viewData.atYEnd)
                scrollDirectionUp = true;
        }
    }

    //If mouse pointer go out the parent area
    onExited:
    {
        //Because mouse can be out the parent area but at scrollButton; in this case scrollButton vust be visible
        scrollVisible = false || scrollMouseAtArrow;
    }

    //If user use a wheel
    //Most mouse types work in steps of 15 degrees,
    //in which case the delta value is a multiple of 120;
    //i.e., 120 units * 1/8 = 15 degrees.
    onWheel:
    {
        //If wheel was rotated up/right
        if(wheel.angleDelta.y > 0)
        {
            if(!viewData.atYBeginning)
            {
                scrollDirectionUp = true;
                //Cause MouseArea get all mouse event this string is need to flick listView
                viewData.flick(0, wheel.angleDelta.y * 3 * pt);
            }
            else
                scrollDirectionUp = false;
        }
        //If wheel was rotated down/left
        else if(wheel.angleDelta.y < 0)
        {
            if(!viewData.atYEnd)
            {
                scrollDirectionUp = false;
                //Cause MouseArea get all mouse event this string is need to flick listView
                viewData.flick(0, wheel.angleDelta.y * 3 * pt);
            }
            else
                scrollDirectionUp = true;
        }
    }


    //If mouse not at scrollButton it is need to chamge visibility
    onScrollMouseAtArrowChanged:
    {
        if(!scrollMouseAtArrow)
            scrollVisible = containsMouse;
    }


    //AutoUpdate on addind string to model
    Component.onCompleted:
    {
        viewData.onContentHeightChanged.connect(onEntered);
    }

}
