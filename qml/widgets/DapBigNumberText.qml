import QtQuick 2.4
import QtQuick.Controls 2.4
import "qrc:/widgets"

import qmlclipboard 1.0

Item
{
    id: bigNumber
    width: text.width + button.width + spacing

    property string fullNumber: "123.456789012345689"
    property int outSymbols: 10
    property int spacing: 10
    property bool showToolTip: true
    property bool copyButtonVisible: true

    property font textFont: mainFont.dapFont.medium14
    property string textColor: currTheme.textColor

    QMLClipboard{
        id: clipboard
    }

    Component.onCompleted:
    {
        getOutText()
    }

    onFullNumberChanged:
    {
        getOutText()
    }

    onOutSymbolsChanged:
    {
        getOutText()
    }

    Text
    {
        id: text
        height: bigNumber.height
        font: textFont
        color: textColor
        text: ""
        verticalAlignment: Qt.AlignBottom

        MouseArea
        {
            id: area
            anchors.fill: parent

            visible: showToolTip

            hoverEnabled: true

            ToolTip {
                id: tooltip
                parent: area
                visible: area.containsMouse
                text: fullNumber

                contentItem:
                Text
                {
                    color: currTheme.textColor
                    text: tooltip.text
                }

                background:
                Rectangle
                {
                    border.color: currTheme.borderColor
                    radius: height*0.5
                    color: currTheme.backgroundElements
                }
            }
        }
    }

    DapCopyButton
    {
        id: button
        x: text.width + spacing
        y: (bigNumber.height - button.height)*0.5
        visible: copyButtonVisible

        popupText: qsTr("Number copied")

        onCopyClicked:
        {
            print("copyButtonClicked", fullNumber)

            clipboard.setText(fullNumber)
        }
    }

    function getOutText()
    {
//        print("getOutText", fullNumber, fullNumber.length)
        print(fullNumber.includes("."))

        if (outSymbols >= fullNumber.length ||
            (fullNumber.includes(".") && outSymbols >= fullNumber.length-1))
        {
            text.text = fullNumber
            showToolTip = false
        }
        else
        {
            var outText = ""
            var i = 0
            var symbols = outSymbols

            while (i < symbols)
            {
                outText += fullNumber[i]
                ++i
                if (fullNumber[i] === '.')
                    ++symbols
            }

            outText += "..."

            text.text = outText
            showToolTip = true
        }
    }

}
