import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects

Rectangle
{
    id: selectorSwitchItem

    property int itemHorisontalBorder: 10
    property int itemVerticalBorder: -2
    property int viewerBorder: 4
    property int animDuration: 200
    property string firstName: "first"
    property string firstColor: currTheme.mainButtonColorNormal0
    property bool firstSelected: true
    property string secondName: "second"
    property string secondColor: currTheme.mainButtonColorNormal1
    property bool secondSelected: !firstSelected
    property bool secondEnabled: true
    property string secondDisabledColor: currTheme.input
    property bool isHovered: mouseArea.containsMouse

    signal toggled()

    width: firstItem.width + secondItem.width + viewerBorder*2
    implicitHeight: height
    implicitWidth: width

    border.color: currTheme.input
    color: currTheme.mainBackground
    radius: height * 0.5

    Rectangle
    {
        id: firstItem
        x: viewerBorder
        anchors.verticalCenter: parent.verticalCenter
        z: 1
        color: "transparent"
        radius: height * 0.5
        width: firstText.width + itemHorisontalBorder * 2
        height: selectorSwitchItem.height - viewerBorder * 2

        Text
        {
            id: firstText
            x: itemHorisontalBorder
            anchors.verticalCenter: parent.verticalCenter
            height: firstItem.height
            bottomPadding: CURRENT_OS === "win" ? 2 : 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: currTheme.white
            font: mainFont.dapFont.medium14
            text: firstName
        }

    }

    Rectangle
    {
        id: secondItem
        x: viewerBorder + firstItem.width
        anchors.verticalCenter: parent.verticalCenter
        z: 1
        color: "transparent"
        radius: height * 0.5
        width: secondText.width + itemHorisontalBorder * 2
        height: selectorSwitchItem.height - viewerBorder * 2
        opacity: secondEnabled ? 1.0 : 0.5

        Text
        {
            id: secondText
            x: itemHorisontalBorder
            anchors.verticalCenter: parent.verticalCenter
            height: secondItem.height
            bottomPadding: CURRENT_OS === "win" ? 2 : 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: secondEnabled ? currTheme.white : currTheme.gray
            font: mainFont.dapFont.medium14
            text: secondName
        }

    }

    Rectangle
    {
        id: selectedRect
        x: firstItem.x
        y: firstItem.y
        z: 0
        color: firstColor
        radius: height * 0.5
        height: firstItem.height
        width: firstItem.width

    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: secondEnabled || firstSelected
        hoverEnabled: true

        onClicked: {
            if (firstSelected)
            {
                if (secondEnabled) {
                    firstSelected = false
                    firstAnim.stop()
                    secondAnim.start()
                }
            }
            else
            {
                firstSelected = true
                secondAnim.stop()
                firstAnim.start()
            }

            toggled()
        }
    }

    ParallelAnimation {
        id: firstAnim
        PropertyAnimation {
            target: selectedRect
            properties: "color"
            to: firstColor
            duration: animDuration
        }
        PropertyAnimation {
            target: selectedRect
            properties: "x"
            to: firstItem.x
            duration: animDuration
        }
        PropertyAnimation {
            target: selectedRect
            properties: "width"
            to: firstItem.width
            duration: animDuration
        }
    }

    ParallelAnimation {
        id: secondAnim
        PropertyAnimation {
            target: selectedRect
            properties: "color"
            to: secondEnabled ? secondColor : secondDisabledColor
            duration: animDuration
        }
        PropertyAnimation {
            target: selectedRect
            properties: "x"
            to: secondItem.x
            duration: animDuration
        }
        PropertyAnimation {
            target: selectedRect
            properties: "width"
            to: secondItem.width
            duration: animDuration
        }
    }

    function setSelected(position)
    {
        if (position === "first")
        {
            firstSelected = true
            selectedRect.color = firstColor
            selectedRect.x = firstItem.x
            selectedRect.width = firstItem.width
        }
        else
        {
            if (secondEnabled) {
                firstSelected = false
                selectedRect.color = secondColor
                selectedRect.x = secondItem.x
                selectedRect.width = secondItem.width
            }
        }
    }

}
