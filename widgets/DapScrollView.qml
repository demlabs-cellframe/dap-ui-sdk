import QtQuick 2.4

DapScrollViewForm
{
    // All the logic (including state names and vars) was taken from master branch

    // Var for current pos while scrolling (in function updateY())
    property var contentPos: 0.0

    // Connect to new ListView if changed
    onViewDataChanged:
    {
        viewData.contentYChanged.connect(updateY)
    }

    // Changes position of arrows when scroll
    function updateY()
    {
        if (viewData.atYBeginning)
        {
            scrollButton.state = "goUp"
        }
        else if (viewData.atYEnd)
        {
            scrollButton.state = "goDown"
        }
        else if (contentPos > viewData.contentItem.y)
        {
            scrollButton.state = "goUp"
        }
        else
        {
            scrollButton.state = "goDown"
        }
        contentPos = viewData.contentItem.y
    }

    onEntered:
    {
        scrollButton.visible = true;
    }

    onExited:
    {
        scrollButton.visible = false;
    }

    scrollButton.onEntered:
    {
        if (scrollButton.state === "goUp")
        {
            scrollButtonImage.source = scrollDownButtonHoveredImageSource
        }
        else if (scrollButton.state === "goDown")
        {
            scrollButtonImage.source = scrollUpButtonHoveredImageSource
        }
    }

    scrollButton.onExited:
    {
        if (scrollButton.state === "goUp")
        {
            scrollButtonImage.source = scrollDownButtonImageSource
        }
        else if (scrollButton.state === "goDown")
        {
            scrollButtonImage.source = scrollUpButtonImageSource
        }
    }

    scrollButton.onClicked:
    {
        if(scrollButton.state === "goUp")
        {
            viewData.positionViewAtEnd();
            scrollButton.state = "goDown";
        }
        else if(scrollButton.state === "goDown")
        {
            viewData.positionViewAtBeginning();
            scrollButton.state = "goUp";
        }
    }

    scrollButton.states:
    [
        State
        {
            name: "goDown"
            PropertyChanges
            {
                target: scrollButton
                onStateChanged:
                {
                    scrollButton.anchors.top = undefined
                    scrollButton.anchors.bottom = parent.bottom
                    scrollButton.exited()
                }
            }
        },

        State
        {
            name: "goUp"
            PropertyChanges
            {
                target: scrollButton
                onStateChanged:
                {
                    scrollButton.anchors.bottom = undefined
                    scrollButton.anchors.top = parent.top
                    scrollButton.exited()
                }
            }
        }

    ]

    scrollButton.state: "goUp"
}
