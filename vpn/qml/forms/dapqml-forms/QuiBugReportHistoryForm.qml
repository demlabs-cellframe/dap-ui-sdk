import QtQuick 2.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Bug reports"
        qss: "dialog-title"
    }

    /* model */
    ListModel {
        id: reportModel
        ListElement {
            name: "Report #1264"
            state: "In developing"
        }
        ListElement {
            name: "Report #1270"
            state: "In developing"
        }
        ListElement {
            name: "Report #3335"
            state: "Resolved"
        }
        ListElement {
            name: "Report #1264"
            state: "Resolved"
        }
    }

    /* resizer */
    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "brh-btn-resizer"
    }

    DapQmlRectangle {
        id: icnResizer
        visible: false
        qss: "brh-icn-resizer"
    }

    /* listview */
    ListView {
        id: csListView

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: root.width - 72
        height: root.height - y
        clip: true

        model: reportModel

        delegate: DapQmlButton {
            buttonStyle: DapQmlButton.IconMainSub
            mainText: model.name
            subText: model.state
            mainQss: "brh-btn-label-main"
            subQss: "brh-btn-label-sub"
            icon: "ic_information_bug-report"
            separator: true
            iconSize: icnResizer.height
            width: resizer.width
            height: resizer.height

//            Component.onCompleted: StyleDebugTree.describe (
//               "Report " + model.index,
//                ["x", "y", "z", "width", "height"],
//               this);
        }
    }
}
