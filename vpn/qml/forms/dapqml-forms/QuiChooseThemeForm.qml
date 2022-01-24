import QtQuick 2.0
import DapQmlThemeModel 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root

    signal close()

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Language"
        qss: "dialog-title"
    }

    /* model */
    DapQmlThemeModel {
        id: themeModel
    }

    /* resizer */
    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "choosetheme-btn-resizer"
    }

    /* listview */
    ListView {
        id: csListView

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: root.width - 72
        height: root.height - y
        clip: true

        model: themeModel

        delegate: DapQmlRadioButton {
            text: model.name
            separator: true
            iconSize: resizer.height
            width: resizer.width
            height: resizer.height
        }
    }}
