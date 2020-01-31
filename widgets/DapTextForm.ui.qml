import QtQuick 2.4
import QtQuick.Controls 2.2

Item 
{
    id: dapText

    ///@details Text.
    property alias text: fullText.text
    ///@details Text color.
    property alias color: elidedText.color
    ///@details Font.
    property alias font: elidedText.font
    ///@details Elide style.
    property int elide: Qt.ElideRight
    ///@details The number of symbols to display.
    property int maxSymbolCapacity: 0
    ///@details Text to display.
    property alias elidedText: elidedText.text

    property alias _fullText: fullText
    property alias _metrics: metrics

    // Displays the text
    Text 
    {
        id: elidedText
    }

    // Calculates elide
    FontMetrics 
    {
        id: metrics
        font: parent.font
    }

    // Stores full string to copy
    TextInput 
    {
        id: fullText
        visible: false
    }
}
