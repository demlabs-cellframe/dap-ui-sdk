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

    property color currentColor: "#10B981"
    property string currentState: "Recommended"

    property color backgroundProgressColor: "#2E3441"
    property color vertLowColor: "#EF4444"      //red
    property color lowColor: "#F59E0B"          //orange
    property color recommendedColor: "#10B981"  //green
    property color highColor: "#22D3EE"         //neon
    property color vertHighColor: "#9580FF"     //purple

    signal valueChange
    signal cannotSetValue(bool isBigValue)
    //width: 278
    height: 90 * guiApp.scaleFactor

    // Create DapCoinCalculator instance for precise calculations
    DapCoinCalculator {
        id: coinCalculator
    }

    ListModel {
        id: statesData
    }

    Component.onCompleted: {
        init(rangeValues);
        // Force initialization if statesData is empty
        if (statesData.count === 0) {
            initStates();
            updateState();
        }
    }

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

            MouseArea {
                anchors.fill: parent
                onClicked: valueText.forceActiveFocus()
            }

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
                        if (processedValue === "" || processedValue === "." || processedValue === "0." || processedValue === ".0")
                            processedValue = "0.0";

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

        // Brand new fee selector component
        Item {
            id: feeSelector
            Layout.fillWidth: true
            Layout.preferredHeight: 24 * guiApp.scaleFactor
            // Layout.topMargin: 20 * guiApp.scaleFactor
            Layout.bottomMargin: 9 * guiApp.scaleFactor

            property int activeLevel: getActiveLevel()

            // Background track
            Rectangle {
                id: track
                anchors.fill: parent
                anchors.topMargin: 9 * guiApp.scaleFactor
                anchors.bottomMargin: 9 * guiApp.scaleFactor
                height: 6 * guiApp.scaleFactor
                radius: 3 * guiApp.scaleFactor
                color: backgroundProgressColor
            }

            // Active portion overlay
            Rectangle {
                id: activePortion
                anchors.top: track.top
                anchors.bottom: track.bottom
                x: feeSelector.getActiveX()
                width: feeSelector.getActiveWidth()
                radius: track.radius
                color: currentColor

                Behavior on color {
                    PropertyAnimation {
                        duration: 200
                    }
                }
            }

            // Control points
            Rectangle {
                id: point1
                x: track.width * 0.0 - width / 2
                anchors.verticalCenter: track.verticalCenter
                width: feeSelector.getPointSize(0)
                height: width
                radius: width / 2
                color: feeSelector.getPointColor(0)

                Rectangle {
                    visible: feeSelector.activeLevel === 0
                    anchors.centerIn: parent
                    width: 6 * guiApp.scaleFactor
                    height: 6 * guiApp.scaleFactor
                    radius: width / 2
                    color: backgroundProgressColor
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4 * guiApp.scaleFactor
                    onClicked: {
                        if (statesData.count > 0) {
                            let targetValue = statesData.get(0).minValue;
                            let medianValue = statesData.count > 2 ? statesData.get(2).minValue : targetValue;

                            if (coinCalculator.isEqual(medianValue, minimalValue)) {
                                cannotSetValue(false);
                            } else if (coinCalculator.isLess(targetValue, minimalValue)) {
                                cannotSetValue(false);
                            } else if (coinCalculator.isGreater(targetValue, maximumValue)) {
                                cannotSetValue(true);
                            } else {
                                setValue(targetValue);
                            }
                        }
                    }
                }

                Behavior on color {
                    PropertyAnimation {
                        duration: 200
                    }
                }
            }

            Rectangle {
                id: point2
                x: track.width * 0.25 - width / 2
                anchors.verticalCenter: track.verticalCenter
                width: feeSelector.getPointSize(1)
                height: width
                radius: width / 2
                color: feeSelector.getPointColor(1)

                Rectangle {
                    visible: feeSelector.activeLevel === 1
                    anchors.centerIn: parent
                    width: 6 * guiApp.scaleFactor
                    height: 6 * guiApp.scaleFactor
                    radius: width / 2
                    color: backgroundProgressColor
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4 * guiApp.scaleFactor
                    onClicked: {
                        if (statesData.count > 1) {
                            let targetValue = statesData.get(1).minValue;
                            let medianValue = statesData.count > 2 ? statesData.get(2).minValue : targetValue;
                            if (coinCalculator.isEqual(medianValue, minimalValue)) {
                                cannotSetValue(false);
                            } else if (coinCalculator.isLess(targetValue, minimalValue)) {
                                cannotSetValue(false);
                            } else {
                                setValue(targetValue);
                            }
                        }
                    }
                }

                Behavior on color {
                    PropertyAnimation {
                        duration: 200
                    }
                }
            }

            Rectangle {
                id: point3
                x: track.width * 0.5 - width / 2
                anchors.verticalCenter: track.verticalCenter
                width: feeSelector.getPointSize(2)
                height: width
                radius: width / 2
                color: feeSelector.getPointColor(2)

                Rectangle {
                    visible: feeSelector.activeLevel === 2
                    anchors.centerIn: parent
                    width: 6 * guiApp.scaleFactor
                    height: 6 * guiApp.scaleFactor
                    radius: width / 2
                    color: backgroundProgressColor
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4 * guiApp.scaleFactor
                    onClicked: if (statesData.count > 2)
                        setValue(statesData.get(2).minValue)
                }

                Behavior on color {
                    PropertyAnimation {
                        duration: 200
                    }
                }
            }

            Rectangle {
                id: point4
                x: track.width * 0.75 - width / 2
                anchors.verticalCenter: track.verticalCenter
                width: feeSelector.getPointSize(3)
                height: width
                radius: width / 2
                color: feeSelector.getPointColor(3)

                Rectangle {
                    visible: feeSelector.activeLevel === 3
                    anchors.centerIn: parent
                    width: 6 * guiApp.scaleFactor
                    height: 6 * guiApp.scaleFactor
                    radius: width / 2
                    color: backgroundProgressColor
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4 * guiApp.scaleFactor
                    onClicked: {
                        if (statesData.count > 3) {
                            let targetValue = statesData.get(3).minValue;
                            let medianValue = statesData.count > 2 ? statesData.get(2).minValue : targetValue;
                            if (coinCalculator.isEqual(medianValue, maximumValue)) {
                                cannotSetValue(true);
                            } else if (coinCalculator.isGreater(targetValue, maximumValue)) {
                                cannotSetValue(true);
                            } else {
                                setValue(targetValue);
                            }
                        }
                    }
                }

                Behavior on color {
                    PropertyAnimation {
                        duration: 200
                    }
                }
            }

            Rectangle {
                id: point5
                x: track.width * 1.0 - width / 2
                anchors.verticalCenter: track.verticalCenter
                width: feeSelector.getPointSize(4)
                height: width
                radius: width / 2
                color: feeSelector.getPointColor(4)

                Rectangle {
                    visible: feeSelector.activeLevel === 4
                    anchors.centerIn: parent
                    width: 6 * guiApp.scaleFactor
                    height: 6 * guiApp.scaleFactor
                    radius: width / 2
                    color: backgroundProgressColor
                }

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4 * guiApp.scaleFactor
                    onClicked: {
                        if (statesData.count > 4) {
                            let targetValue = statesData.get(4).minValue;
                            let medianValue = statesData.count > 2 ? statesData.get(2).minValue : targetValue;

                            if (coinCalculator.isEqual(medianValue, maximumValue)) {
                                cannotSetValue(true);
                            } else if (coinCalculator.isLess(targetValue, minimalValue)) {
                                cannotSetValue(false);
                            } else if (coinCalculator.isGreater(targetValue, maximumValue)) {
                                cannotSetValue(true);
                            } else {
                                setValue(targetValue);
                            }
                        }
                    }
                }

                Behavior on color {
                    PropertyAnimation {
                        duration: 200
                    }
                }
            }

            function getActiveLevel() {
                for (let i = 0; i < statesData.count; i++) {
                    if (statesData.get(i).name === currentState) {
                        return i;
                    }
                }
                return -1;
            }

            function getActiveWidth() {
                if (activeLevel === -1)
                    return 0;

                let centerPos = 2;  // "Recommended" position

                if (activeLevel === centerPos) {
                    // No fill for "Recommended" - only point is colored
                    return 0;
                } else if (activeLevel < centerPos) {
                    // From activeLevel to center
                    return track.width * ((centerPos - activeLevel) * 0.25);
                } else {
                    // From center to activeLevel
                    return track.width * ((activeLevel - centerPos) * 0.25);
                }
            }

            function getActiveX() {
                if (activeLevel === -1)
                    return 0;

                let centerPos = 2;  // "Recommended" position

                if (activeLevel === centerPos) {
                    // No fill for "Recommended"
                    return 0;
                } else if (activeLevel < centerPos) {
                    // From activeLevel position
                    return track.width * (activeLevel * 0.25);
                } else {
                    // From center position
                    return track.width * (centerPos * 0.25);
                }
            }

            function getPointSize(pointIndex) {
                if (activeLevel === pointIndex) {
                    return 18 * guiApp.scaleFactor;
                } else {
                    return 12 * guiApp.scaleFactor;
                }
            }

            function getPointColor(pointIndex) {
                if (activeLevel === -1)
                    return backgroundProgressColor;

                let centerPos = 2;  // "Recommended" position

                // Points between center and current selection get current color
                if (activeLevel === centerPos) {
                    // Only center point active for "Recommended"
                    if (pointIndex === centerPos) {
                        return currentColor;
                    } else {
                        return backgroundProgressColor;
                    }
                } else if (activeLevel < centerPos) {
                    // Points from activeLevel to center
                    if (pointIndex >= activeLevel && pointIndex <= centerPos) {
                        return currentColor;
                    }
                } else {
                    // Points from center to activeLevel
                    if (pointIndex >= centerPos && pointIndex <= activeLevel) {
                        return currentColor;
                    }
                }

                return backgroundProgressColor;
            }
        }

        Text {
            id: textState
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: -8 * guiApp.scaleFactor
            text: currentState
            color: currentColor
            font: mainFont.dapFont.regular14

            Behavior on color {
                PropertyAnimation {
                    duration: 150
                }
            }
        }
    }

    function updateMedian(new_median) {
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

        // Color spread from center (Recommended=2) to selected point
        if (currentStateIndex >= 0) {
            const centerIndex = 2; // Recommended

            if (currentStateIndex === centerIndex) {
                // Special case: Recommended selected - entire bar is green
                for (var i = 0; i < statesData.count; i++) {
                    statesData.get(i).enabled = true;
                }
            } else {
                // Always enable center (Recommended)
                if (centerIndex < statesData.count) {
                    statesData.get(centerIndex).enabled = true;
                }

                // Enable current selected state
                if (currentStateIndex < statesData.count) {
                    statesData.get(currentStateIndex).enabled = true;
                }

                // Fill path between center and current state
                if (currentStateIndex > centerIndex) {
                    // Right side: Recommended → High/Very High
                    for (var i = centerIndex + 1; i < currentStateIndex && i < statesData.count; i++) {
                        statesData.get(i).enabled = true;
                    }
                } else if (currentStateIndex < centerIndex) {
                    // Left side: Very Low/Low ← Recommended
                    for (var i = currentStateIndex + 1; i < centerIndex && i < statesData.count; i++) {
                        statesData.get(i).enabled = true;
                    }
                }
            }
        }

        // Fallback if no state found
        if (currentStateIndex < 0) {
            currentState = statesData.get(0).name;
            statesData.get(0).enabled = true;
        }

        // Set colors
        if (currentState === "Very low")
            currentColor = vertLowColor;
        else if (currentState === "Low")
            currentColor = lowColor;
        else if (currentState === "Recommended")
            currentColor = recommendedColor;
        else if (currentState === "High")
            currentColor = highColor;
        else if (currentState === "Very high")
            currentColor = vertHighColor;
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

    // Public function to commit current input value
    function commitInput() {
        if (!editable || !valueText.text) {
            return;
        }

        // Same logic as onEditingFinished
        let processedValue = valueText.text;
        if (processedValue === "" || processedValue === "." || processedValue === "0." || processedValue === ".0")
            processedValue = "0.0";

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

            if (clampedValue !== valueText.text) {
                valueText.text = clampedValue;
            }

            setValue(clampedValue);
        }
    }
}
