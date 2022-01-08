import QtQuick 2.4
import QtQuick.Controls 2.5
import "qrc:/dapqml-widgets"

Item {
    id: window
    width: 428
    height: 926
    visible: true
    Column {
        function wtext(weight) {
            return "1 2 3 4 font.weight: Font." + weight
        }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.Thin       }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.ExtraLight }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.Light      }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.Normal     }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.Medium     }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.DemiBold   }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.Bold       }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.ExtraBold  }
        Text { text: parent.wtext(font.weight); font.family: 'Lato'; font.weight: Font.Black      }
    }
}
