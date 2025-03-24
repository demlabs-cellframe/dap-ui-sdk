import QtQuick 2.4
import QtQml
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import Qt5Compat.GraphicalEffects

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
        getOutText()
    }

    onFullNumberChanged:
    {
        getOutText()
    }

    RowLayout
    {
        anchors.fill: parent
        spacing: 5

        Text
        {
            id: textItem
            Layout.alignment: Qt.AlignVCenter
//            Layout.fillWidth: true
            height: bigNumber.height
//            anchors.fill: parent
            font: textFont
            color: textColor
            text: ""
//            text: fullNumber
            verticalAlignment: Qt.AlignVCenter

            MouseArea
            {
                id: area
                anchors.fill: parent

                visible: showToolTip

                hoverEnabled: true

                DapCustomToolTip{
                    id: tooltip
                    parent: area
                    visible: area.containsMouse
                    contentText: fullNumber
                    onVisibleChanged:
                    {
                        updatePos()
                    }
                }
            }
        }

        DapCopyButton
        {
            id: button
            Layout.alignment: Qt.AlignVCenter
            Layout.topMargin: textItem.height*0.1
            visible: copyButtonVisible

            popupText: qsTr("Number copied")

            onCopyClicked:
            {
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
