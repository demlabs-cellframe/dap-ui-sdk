import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0

import "qrc:/widgets"

import qmlclipboard 1.0

Item
{
    id: bigNumber

    property string fullNumber: "-"
    property string tokenName: ""
    property int outSymbols: 10
    property int spacing: 10
    property bool showToolTip: true
    property bool alwaysHoverShow: false
    property bool copyButtonVisible: true
    property bool isAutoOutText: false

    property alias horizontalAlign: textItem.horizontalAlignment
    property alias verticalAlign: textItem.verticalAlignment

    property font textFont: mainFont.dapFont.medium12
    property string textColor: currTheme.textColor
	
    property alias textElement: textItem

    QMLClipboard{
        id: clipboard
    }

    Component.onCompleted:
    {
//        print("DapBigNumberText Component.onCompleted", fullNumber)
        if(!isAutoOutText)
        {
//            width = textItem.width + button.width + spacing
            getOutText()
        }
    }

    onFullNumberChanged:
    {
        if(!isAutoOutText)
        {
//            width = textItem.width + button.width + spacing
            getOutText()
        }
    }

/*    onOutSymbolsChanged:
    {
//        print("DapBigNumberText onOutSymbolsChanged", fullNumber)

//        if(!isAutoOutText)
//            getOutText()
    }*/

    RowLayout
    {
        anchors.fill: parent
        spacing: 5

//        Text
//        {
//            Layout.alignment: Qt.AlignVCenter
//            height: bigNumber.height
//            text: fullNumber
//        }

        Text
        {
            id: textItem
            Layout.alignment: Qt.AlignVCenter
            height: bigNumber.height
//            anchors.fill: parent
            font: textFont
            color: textColor
            text: isAutoOutText ? fullNumber : ""
//            text: fullNumber
            verticalAlignment: Qt.AlignVCenter
//            elide: isAutoOutText ? Text.ElideMiddle: Text.ElideNone

            MouseArea
            {
                id: area
                anchors.fill: parent

                visible: showToolTip

                hoverEnabled: true

                DapCustomToolTip{
                    id: tooltip
                    parent: area
                    visible: !isAutoOutText && area.containsMouse ?
                                 true :
                                 alwaysHoverShow && area.containsMouse ?
                                     true :
                                     area.containsMouse ?
                                         textItem.implicitWidth > textItem.width ?
                                             true :
                                             false : false
                    contentText: fullNumber
                    onVisibleChanged: /*console.log(text.y, bigNumber.y, y)*/
                    {
                        if(!isAutoOutText)
                            x = outSymbols/2 - textItem.implicitWidth/2
                        updatePos()
                    }
                }
            }
        }

        DapCopyButton
        {
            id: button
//            x: text.width + spacing
//            y: (bigNumber.height - button.height)*0.5
            Layout.alignment: Qt.AlignVCenter
            Layout.topMargin: textItem.height*0.1
            visible: copyButtonVisible

            popupText: qsTr("Number copied")

            onCopyClicked:
            {
//                print("copyButtonClicked", fullNumber)

                clipboard.setText(fullNumber)
            }
        }

        Item
        {
            Layout.fillWidth: true
        }

    }


    function getOutText()
    {
//        print("getOutText", fullNumber, fullNumber.length)
//        print(fullNumber.includes("."))

        if (outSymbols >= fullNumber.length ||
            (fullNumber.includes(".") && outSymbols >= fullNumber.length-1))
        {
            textItem.text = fullNumber + " " + tokenName
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

            textItem.text = outText + " " + tokenName
            showToolTip = true
        }
    }

}
