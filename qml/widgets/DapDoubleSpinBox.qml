import QtQuick 2.4
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

SpinBox {
    id: spinbox

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

    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / factor).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * factor
    }

    contentItem:

        TextInput {
            z: 2

            anchors.top: spinbox.top
            anchors.bottom: spinbox.bottom

            height: spinbox.height
            text: spinbox.textFromValue(spinbox.value, spinbox.locale)

            font: spinbox.font
            color: currTheme.textColor
        //        selectionColor: "#21be2b"
        //        selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            maximumLength: 6


            readOnly: !spinbox.editable
//            readOnly: true
            validator: spinbox.validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly
    }



    up.indicator: Rectangle {
        x: spinbox.mirrored ? 0 : parent.width - width
        width: spinbox.height
        height: spinbox.height
        color: "transparent"
        border.color: "transparent"

        Image {
            anchors.fill: parent
            mipmap: true
            source: enabled ? spinbox.up.pressed ?
                                  "qrc:/icons/button_dark.png" :
                                  "qrc:/icons/button_on.png" :
                                  "qrc:/icons/button_off.png"
        }

        Text {
            text: "+"
            font.pixelSize: spinbox.font.pixelSize * 3
            color: currTheme.textColor
            anchors.fill: parent
            anchors.bottomMargin: 2
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    down.indicator: Rectangle {
        id: minusButton
        x: spinbox.mirrored ? parent.width - width : 0
        width: spinbox.height
        height: spinbox.height
        color: "transparent"
        border.color: "transparent"

        Image {
            anchors.fill: parent
            mipmap: true
            source: enabled ? spinbox.down.pressed ?
                                  "qrc:/icons/button_dark.png" :
                                  "qrc:/icons/button_on.png" :
                                  "qrc:/icons/button_off.png"
        }

        Text {
            text: "–"
            font.pixelSize: spinbox.font.pixelSize * 3
            color: currTheme.textColor
            anchors.fill: parent
            anchors.bottomMargin: 2
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }


    background: Rectangle {
//        implicitWidth: spinbox.width
        color: "transparent"
    }
}
