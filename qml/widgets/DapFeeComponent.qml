import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/widgets"
import DapCoinCalculator

Item {
    id: root
    property string currentValue: "0.01"
    property string minimalValue: "0.0"
    property string maximumValue: "100.0"

    property string valueName: "-"

    property bool editable: true

    property color currentColor: "#CAFC33"
    property string currentState: "Recommended"

    signal valueChange
    //width: 278
    height: 82 * guiApp.scaleFactor

    // Create DapCoinCalculator instance for precise calculations
    DapCoinCalculator {
        id: coinCalculator
    }

    ListModel {
        id: statesData
    }

    Component.onCompleted: init(rangeValues)

    property var rangeValues: {
        "veryLow": "0.03",
        "low": "0.04",
        "middle": "0.05",
        "high": "0.06",
        "veryHigh": "0.07",
        "veryHighStop": "0.14"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 16 * guiApp.scaleFactor
        anchors.rightMargin: 16 * guiApp.scaleFactor

        // Input field
        Rectangle {
            id: valueField
            Layout.fillWidth: true
            Layout.preferredHeight: 40 * guiApp.scaleFactor
            Layout.alignment: Qt.AlignHCenter

            color: currTheme.secondaryBackground
            border.width: 1 * guiApp.scaleFactor
            border.color: currTheme.input
            radius: 4

            Item {
                id: inputValueItem
                property int spacing: 16 * guiApp.scaleFactor
                implicitWidth: textMetrics.width + spacing + valueNameText.width > valueField.width - spacing ? valueField.width - spacing : textMetrics.width + spacing + valueNameText.width
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 1 * guiApp.scaleFactor
                anchors.bottomMargin: 1 * guiApp.scaleFactor
                anchors.leftMargin: 8 * guiApp.scaleFactor
                anchors.rightMargin: 8 * guiApp.scaleFactor

                DapTextField {
                    id: valueText
                    height: parent.height
                    anchors.left: parent.left
                    anchors.right: valueNameText.left
                    text: currentValue
                    font: mainFont.dapFont.regular16
                    regExpValidator: /^[0-9]*\.?[0-9]{0,18}$/
                    inputMethodHints: CURRENT_OS === "ios" ? Qt.ImhNone : Qt.ImhFormattedNumbersOnly
                    defaultPlaceholderText: "0.0"
                    horizontalAlignment: Text.AlignRight
                    borderWidth: 0
                    borderRadius: 0
                    placeholderColor: currTheme.gray
                    selectByMouse: editable
                    enabled: editable

                    // Keys.onReturnPressed: focus = false
                    // Keys.onEnterPressed: focus = false

                    onTextChanged: {
                        textMetrics.text = text;
                    }

                    onEditingFinished: {
                        // Add .0 to whole numbers, then validate range
                        let processedValue = text;

                        // Add .0 if it's a whole number without decimal point
                        if (processedValue !== "" && processedValue.indexOf('.') === -1) {
                            processedValue += ".0";
                        }

                        if (coinCalculator.isValid(processedValue)) {
                            let clampedValue = processedValue;
                            if (coinCalculator.isLess(processedValue, minimalValue)) {
                                clampedValue = minimalValue;
                            } else if (coinCalculator.isGreater(processedValue, maximumValue)) {
                                clampedValue = maximumValue;
                            }

                            if (clampedValue !== text) {
                                text = clampedValue;
                            }

                            setValue(clampedValue);
                        }

                        if (!focus)
                            cursorPosition = 0;
                    }

                    onFocusChanged: {
                        cursorPosition = 0;
                    }

                    TextMetrics {
                        id: textMetrics
                        font: valueText.font
                        text: valueText.text
                    }
                }

                Text {
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

        RowLayout {
            id: colorRect
            Layout.preferredWidth: 278 * guiApp.scaleFactor
            Layout.preferredHeight: 4 * guiApp.scaleFactor
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 12 * guiApp.scaleFactor

            spacing: 5 * guiApp.scaleFactor

            Repeater {
                model: statesData

                delegate: Rectangle {
                    width: 52 * guiApp.scaleFactor
                    height: 4 * guiApp.scaleFactor
                    radius: 12
                    color: model.enabled ? currentColor : "#666E7D"

                    Behavior on color {
                        PropertyAnimation {
                            duration: 150
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: setValue(model.minValue)
                    }
                }
            }
        }

        Text {
            id: textState
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 8 * guiApp.scaleFactor
            text: currentState
            color: currentColor

            Behavior on color {
                PropertyAnimation {
                    duration: 150
                }
            }
        }
    }

    function updateMedian(new_median)
    {
        if (new_median !== root.medianStr) {
            if (root.medianStr !== "") {
                notifyAboutChange(new_median);
            } else {
                init(generateRanges(new_median));
            }
            root.medianStr = new_median;
        }
    }

    function resetFeeData() {
        valueName = "";
        minimalValue = "0.000000000000000001";
        maximumValue = "100.0";
        medianStr = "0.001";
    }

    function notifyAboutChange(new_median) {
        var new_ranges = generateRanges(new_median);
        root.rangeValues = new_ranges;
        root.initStates();
        root.setValue(new_median);
    }

    function generateRanges(median_str) {
        let rawLow = coinCalculator.divide(coinCalculator.add(median_str, minimalValue), "2.0");
        let rawHigh = coinCalculator.divide(coinCalculator.add(maximumValue, median_str), "2.0");

        return {
            "veryLow": minimalValue,
            "low": roundValue(rawLow),
            "middle": median_str,
            "high": roundValue(rawHigh),
            "veryHigh": maximumValue,
            "veryHighStop": coinCalculator.multiply(maximumValue, "2.0")
        };
    }

    function roundValue(value_str) {
        // If has integer part - drop decimal, if only decimal - round to significant
        if (!coinCalculator.isValid(value_str) || coinCalculator.isEqual(value_str, "0")) {
            return value_str;
        }

        let valueStr = value_str.toString();
        let decimalIndex = valueStr.indexOf('.');

        if (decimalIndex === -1) {
            // No decimal point - return as is
            return valueStr;
        }

        let integerPart = valueStr.substring(0, decimalIndex);

        if (integerPart !== "0" && integerPart !== "") {
            // Has integer part - drop decimal but add .0
            return integerPart + ".0";
        } else {
            // Only decimal part - find first significant digit
            let firstSignificantPos = -1;
            for (let i = decimalIndex + 1; i < valueStr.length; i++) {
                if (valueStr[i] !== '0') {
                    firstSignificantPos = i - decimalIndex;
                    break;
                }
            }

            if (firstSignificantPos === -1) {
                return "0";
            }

            // Round to first significant digit (0.001, 0.01, 0.1)
            let roundedValue = "0." + "0".repeat(firstSignificantPos - 1) + "1";
            return roundedValue;
        }
    }

    function setValue(value) {
        if (coinCalculator.isValid(value)) {
            if (coinCalculator.isGreaterOrEqual(value, minimalValue) && coinCalculator.isLessOrEqual(value, maximumValue)) {
                currentValue = value;
                valueText.text = value; // Sync with input field
                updateState();
                valueChange();
            }
        }
    }

    function updateState() {
        if (statesData.count == 0)
            return;

        // Reset all states
        for (var i = 0; i < statesData.count; i++) {
            statesData.get(i).enabled = false;
        }

        var currentStateIndex = -1;

        // Logic: split by median first, then check sub-ranges
        if (coinCalculator.isEqual(currentValue, rangeValues.middle)) {
            currentState = "Recommended";
            currentStateIndex = 2;
        } else if (coinCalculator.isGreater(currentValue, rangeValues.middle)) {
            // Above median - check High vs Very High
            if (coinCalculator.isGreaterOrEqual(currentValue, rangeValues.veryHigh)) {
                currentState = "Very high";
                currentStateIndex = 4;
            } else {
                currentState = "High";
                currentStateIndex = 3;
            }
        } else {
            // Below median - check Low vs Very Low
            if (coinCalculator.isLess(currentValue, rangeValues.low)) {
                currentState = "Very low";
                currentStateIndex = 0;
            } else {
                currentState = "Low";
                currentStateIndex = 1;
            }
        }

        // Progressive scale: enable all states up to current one
        for (var i = 0; i <= currentStateIndex && i < statesData.count; i++) {
            statesData.get(i).enabled = true;
        }

        // Fallback if no state found
        if (currentStateIndex < 0) {
            currentState = statesData.get(0).name;
            statesData.get(0).enabled = true;
        }

        // Set colors
        if (currentState === "Very low")
            currentColor = "#FF5F5F";
        else if (currentState === "Low")
            currentColor = "#FFCD44";
        else if (currentState === "Recommended")
            currentColor = "#CAFC33";
        else if (currentState === "High")
            currentColor = "#79FFFA";
        else if (currentState === "Very high")
            currentColor = "#9580FF";
    }

    function init(ranges) {
        rangeValues = ranges;
        initStates();
        setValue(rangeValues.middle);
    }

    function initStates() {
        statesData.clear();

        //very low range
        statesData.append({
            name: "Very low",
            minValue: rangeValues.veryLow,
            maxValue: rangeValues.low,
            enabled: false
        });

        //low range
        statesData.append({
            name: "Low",
            minValue: rangeValues.low,
            maxValue: rangeValues.middle,
            enabled: false
        });

        //recommended range - median point
        statesData.append({
            name: "Recommended",
            minValue: rangeValues.middle,
            maxValue: rangeValues.middle,
            enabled: false
        });

        //high range
        statesData.append({
            name: "High",
            minValue: rangeValues.high,
            maxValue: rangeValues.veryHigh,
            enabled: false
        });

        //very high range
        statesData.append({
            name: "Very high",
            minValue: rangeValues.veryHigh,
            maxValue: rangeValues.veryHighStop,
            enabled: false
        });
    }
}
