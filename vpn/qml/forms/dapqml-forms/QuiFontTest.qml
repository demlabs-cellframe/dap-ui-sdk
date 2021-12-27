import QtQuick 2.4
import QtQuick.Controls 2.5
import "qrc:/dapqml-widgets"

ApplicationWindow {
    id: window
    width: 428
    height: 926
    visible: true
    Column {
        function wtext(weight) {
            return "1 2 3 4 font.weight: Font." + weight
        }
        DapQmlLabel { text: parent.wtext(weight); weight: "Thin"       }
        DapQmlLabel { text: parent.wtext(weight); weight: "ExtraLight" }
        DapQmlLabel { text: parent.wtext(weight); weight: "Light"      }
        DapQmlLabel { text: parent.wtext(weight); weight: "Normal"     }
        DapQmlLabel { text: parent.wtext(weight); weight: "Medium"     }
        DapQmlLabel { text: parent.wtext(weight); weight: "DemiBold"   }
        DapQmlLabel { text: parent.wtext(weight); weight: "Bold"       }
        DapQmlLabel { text: parent.wtext(weight); weight: "ExtraBold"  }
        DapQmlLabel { text: parent.wtext(weight); weight: "Black"      }
    }
}
