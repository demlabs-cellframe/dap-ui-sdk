/* INCLUDES */

import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Controls 2.1
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Font Test Form
 * @ingroup groupDapQmlForms
 *
 * Used only for debug purposes
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: window
    clip: true
    width: 428
    height: 926
    visible: true

    property string fontFam: "Blinker" // "Jost"
    property int fontSize: 11

//    FontLoader { source: "qrc:/fonts/Lato-Regular.ttf" }
//    FontLoader { source: "qrc:/fonts/Lato-Bold.ttf" }

    function indexOf(name) {
        let size    = fontModel.count;

        for(let i = 0; i < size; i++)
            if (fontModel.get(i).text === name)
                return i;

        return 0;
    }

    ListModel {
        id: fontModel

        ListElement { text: "None" }
        ListElement { text: "Roboto" }
        ListElement { text: "Lato" }
        ListElement { text: "Jost" }
        ListElement { text: "Exo2" }
        ListElement { text: "Blinker" }
    }

    ComboBox {
        id: combo
        x: 4
        width: parent.width - 8
        height: 24
        textRole: "text"
        model: fontModel
        font.pointSize: window.fontSize
        //delegate: Text { text: model.name }
        onCurrentTextChanged: window.fontFam = currentText;
        Component.onCompleted: currentIndex = indexOf(window.fontFam)
    }

    Column {
        y: 28
        function wtext(weight) {
            return "1 2 3 4 Aa Bb Cc Dd. font " + /*f.name +*/ " value: " + weight
        }
        Text { text: "--= default =--"; font.family: fontFam; font.pointSize: fontSize; }
        Text { text: parent.wtext(font.weight); /*font.family: fontFam; */font.pointSize: fontSize; font.weight: Font.Normal      ; }
        Text { text: "--= weight =--"; font.family: fontFam; font.pointSize: fontSize; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.Thin      ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.ExtraLight; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.Light     ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.Normal    ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.Medium    ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.DemiBold  ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.Bold      ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.ExtraBold ; }
        Text { text: parent.wtext(font.weight); font.family: fontFam; font.pointSize: fontSize; font.weight: Font.Black     ; }
        Text { text: "--= styleName =--"; font.family: fontFam; font.pointSize: fontSize; }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "Thin" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "ExtraLight" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "Light" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "Normal" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "Medium" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "DemiBold" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "Bold" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "ExtraBold" }
        Text { text: parent.wtext(font.styleName); font.family: fontFam; font.pointSize: fontSize; font.styleName: "Black" }
    }
}

/*-----------------------------------------*/
