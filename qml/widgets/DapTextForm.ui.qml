import QtQuick 2.4
import QtQuick.Controls 2.0

TextInput
{
    id: dapText

    ///@detalis fullText Full text string
    property string fullText
    ///@detalis elText Elided text string
    property string elText
    ////@detalis fontDapText Font setting combobox.
    property font fontDapText
    ///@details elide Elide style.
    property int textElide: Text.ElideRight
    ///@detalis textMetric Text Metric to elide text
    property alias textMetric: dapTextInputTextMetric
    ///@detalis fontMetric Font Metric to check elided text
    property alias fontMetric: dapTextInputFontMetric
    ///@detalis textColor Color of text
    property color textColor

    font: fontDapText
    color: textColor
    //To do elide
    TextMetrics
    {
        id: dapTextInputTextMetric
        font: fontDapText
        elide: textElide
        text: fullText
    }

    // To check elide
    FontMetrics
    {
        id: dapTextInputFontMetric
        font: fontDapText
    }
    text: elText
    enabled: false
}
