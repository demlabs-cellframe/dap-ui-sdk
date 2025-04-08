import QtQuick
import QtQuick.Controls 2.2
import Qt5Compat.GraphicalEffects

SpinBox {
    id: spinbox

    property alias maxSym: textInput.maximumLength

    from: realFrom * factor
    to: realTo * factor
    value: defaultValue * factor
    stepSize: realStep * factor
    editable: true

    property int decimals: 2

    property real realFrom: 0
    property real realTo: 10
    property real realStep: 0.01
    property real defaultValue: 1

    property real factor: Math.pow(10, decimals)

    property real realValue: value / factor

    property real editedValue: 1

    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        console.log("textFromValue realValue", realValue)
        spinbox.editedValue = realValue
        return Number(value / factor).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * factor
    }

    contentItem:
        TextInput {
            z: 2
            id: textInput
            // width: spinbox.width - minusButton.width * 2 - 4

            // // anchors.top: spinbox.top
            // // anchors.bottom: spinbox.bottom

            anchors.fill: spinbox
            anchors.leftMargin: minusButton.width + 1
            anchors.rightMargin: minusButton.width + 1

            // height: spinbox.height
            text: spinbox.textFromValue(spinbox.value, spinbox.locale)

            font: spinbox.font
            color: currTheme.white
        //        selectionColor: "#21be2b"
        //        selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

//            maximumLength: 4


            readOnly: !spinbox.editable
//            readOnly: true
            validator: spinbox.validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly

            onTextEdited:
            {
                var tempValue = Number.fromLocaleString(spinbox.locale, textInput.text)

                if (tempValue >= realFrom && tempValue <= realTo)
                    spinbox.editedValue = tempValue

                console.log("onTextEdited editedValue", editedValue)
            }
    }

    up.indicator: DapButton {
        x: spinbox.mirrored ? 0 : parent.width - width
        width: spinbox.height
        height: spinbox.height
        textButton: "+"
        horizontalAligmentText: Qt.AlignHCenter
        onClicked: spinbox.increase()
    }

    down.indicator: DapButton {
        id: minusButton
        x: spinbox.mirrored ? parent.width - width : 0
        width: spinbox.height
        height: spinbox.height
        textButton: "-"
        horizontalAligmentText: Qt.AlignHCenter
        onClicked: spinbox.decrease()
    }


    background: Rectangle {
//        implicitWidth: spinbox.width
        color: "transparent"
    }
}
