import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/widgets"

Item
{
    property real currentValue: 0.01
    property real spinBoxStep: 0.01
    property real minimalValue: 0.0

    property string valueName: "-"
    property int powerRound: 2

    property color currentColor: "#CAFC33"
    property string currentState: "Recommended"

    id: root
    width: 278
    height: 82

    ListModel
    {
        id: statesData
    }

    Component.onCompleted: init(rangeValues)

    property var rangeValues:
    {
        "veryLow": 0.03,
        "low": 0.04,
        "middle": 0.05,
        "high": 0.06,
        "veryHigh": 0.07,
    }

    // Custom SpinBox
    Item
    {
        id: feeSpinbox
        width: parent.width
        height: 40
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        // Minus
        Rectangle
        {
            width: 40

            anchors.right: valueField.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: 14

            radius: 4
            color: minusArea.containsMouse ? currTheme.inputActive : currTheme.input

            Rectangle
            {
                height: 2
                width: 14
                border.width: 2
                border.color: currTheme.white
                anchors.centerIn: parent
            }

            MouseArea
            {
                id: minusArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    stepValue(-1 * spinBoxStep)
                }
            }
        }

        // Spin value
        Rectangle
        {
            id: valueField
            width: 170

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            color: currTheme.secondaryBackground
            border.width: 1
            border.color: currTheme.input
            radius: 4

            Item
            {
                id: inputValueItem
                implicitWidth: textMetrics.width + 16 + valueNameText.width > valueField.width - 16 ? valueField.width - 16 : textMetrics.width + 16 + valueNameText.width
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 1
                anchors.bottomMargin: 1
                anchors.leftMargin: 8
                anchors.rightMargin: 8

                DapTextField
                {
                    id: valueText
                    height: parent.height
                    anchors.left: parent.left
                    anchors.right: valueNameText.left
                    text: currentValue
                    font: mainFont.dapFont.regular16
                    validator: RegExpValidator { regExp: /[0-9]*\.?[0-9]{0,18}/ }
                    placeholderText: "0.0"
                    horizontalAlignment: Text.AlignRight
                    borderWidth: 0
                    borderRadius: 0
                    placeholderColor: currTheme.gray
                    selectByMouse: true

                    onTextChanged:
                    {
                        setValue(text)
                        textMetrics.text = text
                    }

                    onEditingFinished:
                    {
                        if(!focus) cursorPosition = 0
                    }

                    onFocusChanged:
                    {
                        cursorPosition = 0
                    }

                    TextMetrics
                    {
                        id: textMetrics
                        font: valueText.font
                        text: valueText.text
                    }
                }

                Text
                {
                    id: valueNameText
                    width: contentWidth
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignLeft
                    text: valueName
                    font: mainFont.dapFont.regular16
                    color: currTheme.white
                }
            }
        }

        // Plus
        Rectangle
        {
            width: 40

            anchors.left: valueField.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 14

            radius: 4
            color: plusArea.containsMouse ? currTheme.inputActive : currTheme.input

            Rectangle
            {
                width: 14
                height: 2
                border.width: 2
                border.color: currTheme.white
                anchors.centerIn: parent
            }

            Rectangle
            {
                width: 2
                height: 14
                border.width: 2
                border.color: currTheme.white
                anchors.centerIn: parent
            }

            MouseArea
            {
                id: plusArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    stepValue(spinBoxStep)
                }
            }
        }
    }

    RowLayout
    {
        id: colorRect
        width: 278
        height: 4

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: feeSpinbox.bottom
        anchors.topMargin: 12

        spacing: 5

        Repeater
        {
            model: statesData

            delegate:
                Rectangle
            {
                width: 52
                Layout.fillHeight: true
                radius: 12
                color: model.enabled ? currentColor : "#666E7D"

                Behavior on color
                {
                    PropertyAnimation{duration: 150}
                }

                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: setValue(model.minValue)
                }
            }
        }
    }

    Text
    {
        id: textState
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: colorRect.bottom
        anchors.topMargin: 8
        text: currentState
        color: currentColor

        Behavior on color{
            PropertyAnimation{duration: 150}
        }
    }

    function stepValue(step)
    {
        var summ = mathWorker.summDouble(currentValue, step)
        if(summ !== "") setValue(summ)
   }

    function setValue(value)
    {
        var number = parseFloat(value)
        if(!isNaN(number))
        {
            if( number >= minimalValue)
            {
                currentValue = number
                updateState()
            }
        }
    }

    function updateState()
    {
        if(statesData.count == 0)
            return

        var checkSearch = false
        var idxSearch = -1

        for(var i = statesData.count -1; i >= 0; i--)
        {
            statesData.get(i).enabled = checkSearch

            if(statesData.get(i).minValue >= currentValue || checkSearch)
                continue;

            idxSearch = i
            checkSearch = true
            currentState = statesData.get(i).name
            statesData.get(i).enabled = true
        }

        if(idxSearch < 0)
        {
            currentState = statesData.get(0).name
        }

        if(currentState === "Very low")
            currentColor = "#FF5F5F"
        else if(currentState === "Low")
            currentColor = "#FFCD44"
        else if(currentState === "Recommended")
            currentColor = "#CAFC33"
        else if(currentState === "High")
            currentColor = "#79FFFA"
        else if(currentState === "Very high")
            currentColor = "#9580FF"
    }

    function init(ranges)
    {
        rangeValues = ranges
        initStates()
        setValue(rangeValues.middle)
    }

    function initStates()
    {
        statesData.clear()
        //very low range
        statesData.append(
                    {
                        name: "Very low",
                        minValue: minimalValue,
                        maxValue: rangeValues.veryLow,
                        enabled: false
                    })

        //low range
        statesData.append(
                    {
                        name: "Low",
                        minValue: rangeValues.veryLow,
                        maxValue: rangeValues.low,
                        enabled: false
                    })

        //recomemnded range
        statesData.append(
                    {
                        name: "Recommended",
                        minValue: rangeValues.low,
                        maxValue: rangeValues.middle,
                        enabled: false
                    })

        //high range
        statesData.append(
                    {
                        name: "High",
                        minValue: rangeValues.middle,
                        maxValue: rangeValues.high,
                        enabled: false
                    })

        //very high range
        statesData.append(
                    {
                        name: "Very high",
                        minValue: rangeValues.high,
                        maxValue: rangeValues.veryHigh,
                        enabled: false
                    })
    }
}
