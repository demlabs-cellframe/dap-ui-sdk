import QtQuick 2.0
import QtQuick.Controls 1.4

DapMenuTabWidgetForm
{
    property int widthItemMenu: viewMenuTab.width
    property int heightItemMenu: 60 * pt
    property int widthIconItemMenu: 18 * pt
    property int heightIconItemMenu: 18 * pt
    property string normalColorItemMenu: "transparent"
    property string selectColorItemMenu: "#D51F5D"

    Component
    {
    id: itemMenuTabDelegate

        Rectangle
        {
            id: frameItemMenu

            property bool isPushed: viewMenuTab.currentIndex === index

            width: widthItemMenu
            height: heightItemMenu
            color: normalColorItemMenu

            Image
            {
                id: iconItem
                anchors.left: parent.left
                anchors.leftMargin: 24 * pt
                anchors.verticalCenter: parent.verticalCenter
                height: heightIconItemMenu
                width: widthIconItemMenu
                source: normalIcon
            }

            Text {
                id: textItem
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: iconItem.right
                anchors.leftMargin: 18 * pt
                font.family: fontRobotoLight.name
                font.pixelSize: 16 * pt
                color: "#FFFFFF"
                text: name
            }

            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true

                onEntered:
                {
                    if(!frameItemMenu.isPushed)
                    {
                        iconItem.source = hoverIcon;
                        textItem.font.family = fontRobotoRegular.name;
                    }
                }

                onExited:
                {
                    if(!frameItemMenu.isPushed)
                    {
                        iconItem.source = normalIcon
                        textItem.font.family = fontRobotoLight.name;
                    }
                }

                onClicked:
                {
                    viewMenuTab.currentIndex = index;
                }
            }

            onIsPushedChanged:
            {
                frameItemMenu.color = (isPushed ?  selectColorItemMenu : normalColorItemMenu);
                iconItem.source = isPushed ? model.hoverIcon : model.normalIcon;
                textItem.font.family = (isPushed ? fontRobotoRegular.name : fontRobotoLight.name);
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
