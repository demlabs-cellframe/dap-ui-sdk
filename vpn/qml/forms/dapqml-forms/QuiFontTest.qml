import QtQuick 2.4
import QtQuick.Controls 2.5
import "qrc:/dapqml-widgets"

Item {
    id: window
    width: 428
    height: 926
    visible: true

    property string fontFam: "Lato" // "Jost"
    property int fontSize: 11

//    FontLoader { source: "qrc:/fonts/Lato-Regular.ttf" }
//    FontLoader { source: "qrc:/fonts/Lato-Bold.ttf" }

    Column {
        function wtext(weight) {
            return "1 2 3 4 Aa Bb Cc Dd. font " + /*f.name +*/ " value: " + weight
        }
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
