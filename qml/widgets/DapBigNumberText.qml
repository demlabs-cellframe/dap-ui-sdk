import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.4
import "qrc:/widgets"
import QtGraphicalEffects 1.0

import qmlclipboard 1.0

Item
{
    id: bigNumber

    property string fullNumber: "123.456789012345689"
    property int outSymbols: 10
    property int spacing: 10
    property bool showToolTip: true
    property bool alwaysHoverShow: false
    property bool copyButtonVisible: true
    property bool isAutoOutText: false

    property alias horizontalAlign: text.horizontalAlignment
    property alias verticalAlign: text.verticalAlignment

    property font textFont: mainFont.dapFont.medium12
    property string textColor: currTheme.textColor

    QMLClipboard{
        id: clipboard
    }

    Component.onCompleted:
    {
        if(!isAutoOutText)
        {
            width = text.width + button.width + spacing
            getOutText()
        }
    }

    onFullNumberChanged:
    {
        if(!isAutoOutText)
        {
            width = text.width + button.width + spacing
            getOutText()
        }
    }

    onOutSymbolsChanged:
    {
        if(!isAutoOutText)
            getOutText()
    }

    Text
    {
        id: text
//        height: bigNumber.height
        anchors.fill: parent
        font: textFont
        color: textColor
        text: isAutoOutText ? fullNumber : ""
        verticalAlignment: Qt.AlignVCenter
        elide: isAutoOutText ? Text.ElideMiddle: Text.ElideNone

        MouseArea
        {
            id: area
            anchors.fill: parent

            visible: showToolTip

            hoverEnabled: true

            DapCustomToolTip{
                id: tooltip
                parent: area
                visible: !isAutoOutText && area.containsMouse ? true : alwaysHoverShow && area.containsMouse ? true : area.containsMouse ?  text.implicitWidth > text.width ? true : false : false
                contentText: fullNumber
                onVisibleChanged: /*console.log(text.y, bigNumber.y, y)*/
                {
                    if(!isAutoOutText)
                        x = outSymbols/2 - text.implicitWidth/2
                    updatePos()
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
