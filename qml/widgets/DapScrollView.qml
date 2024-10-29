import QtQuick 2.4

DapScrollViewForm
{
    //Default
    scrollButtonImage.source: scrollButtonArrowUp ? scrollUpButtonImageSource : scrollDownButtonImageSource
    x: parent.width - width - scrollButtonRightMargin
    y: scrollButtonArrowUp ? scrollButtonTopMargin : parent.height - height - scrollButtonBottomMargin


     //If mouse pointer go into scrollButton
    scrollButton.onEntered:
    {
        scrollButtonImage.source = scrollButtonArrowUp ? scrollUpButtonHoveredImageSource :
                                                         scrollDownButtonHoveredImageSource;
        mouseAtArrow = true;
    }

    //If mouse pointer go out scrollButton
    scrollButton.onExited:
    {
        scrollButtonImage.source = scrollButtonArrowUp ? scrollUpButtonImageSource :
                                                         scrollDownButtonImageSource;
        mouseAtArrow = false;
    }

    //Change listView visible position on scrollButton click
    scrollButton.onClicked:
    {
        scrollButtonArrowUp ?  viewData.positionViewAtBeginning() : viewData.positionViewAtEnd();
    }

    //Change arrow image then arrow direction is need to change
    onScrollButtonArrowUpChanged:
    {
        scrollButtonImage.source = scrollButtonArrowUp ? scrollUpButtonImageSource :
                                                         scrollDownButtonImageSource;
    }
}
