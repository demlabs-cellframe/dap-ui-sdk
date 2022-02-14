import QtQuick 2.0
import DapQmlThemeModel 1.0
import ChooseThemeInterface 1.0
import PageCtl 1.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "ChooseTheme"

    /* signals */
    signal sigSelect(int index, string name);

    /* vars */
    ChooseThemeInterface{ id: chooseThemeInterface }
    DapQmlStyle { id: style }

    /* methods */
    function updateState() {
        style.requestRedraw();
        backTimer.start();
    }

    /* go back timer */
    Timer {
        id: backTimer
        interval: 350
        running: false
        repeat: false
        onTriggered: PageCtl.slotBackwardAuto()
    }

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
            text: model.name + " Theme"
            separator: true
            iconSize: resizer.height
            width: resizer.width
            height: resizer.height
            checked: themeModel.isCurrent (model.name)
            onClicked: {
                chooseThemeInterface.clicked (model.name);
                style.requestRedraw();
                backTimer.start();
                root.sigSelect (mode.index, model.name);
            }
        }
    }}
