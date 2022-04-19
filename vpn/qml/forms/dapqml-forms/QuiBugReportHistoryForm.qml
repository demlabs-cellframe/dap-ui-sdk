import QtQuick 2.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "BugReportHistory"

    /* signals */
    signal sigTrashClicked(string name);

    /* title */
    DapQmlDialogTitle {
        id: title
        text: "Bug reports"
        qss: "dialog-title"
    }

    /* resizer */
    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "brh-btn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    DapQmlRectangle {
        id: icnResizer
        visible: false
        qss: "brh-icn-resizer"
    }

    /* listview */
    ListView {
        id: csListView
        objectName: "bugrepHistoryListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        Component.onCompleted: StyleDebugTree.describe (
           "listview",
            ["x", "y", "z", "width", "height"],
           this);

        //model: reportModel

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height

            DapQmlButton {
                property string modelName: model.name
                buttonStyle: DapQmlButton.IconMainSubIcon
                mainText: model.name
                subText: model.state
                mainQss: "brh-btn-label-main"
                subQss: "brh-btn-label-sub"
                icon: "ic_information_bug-report"
                iconRight: "ic_trash"
                separator: true
                iconSize: icnResizer.height
                iconRightSize: icnResizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2

                onRightClicked: sigTrashClicked(modelName)// console.log("right cliked")

                Component.onCompleted: {
                    StyleDebugTree.describe (
                        "Report sub " + model.index,
                        ["x", "y", "z", "width", "height",
                         "color", "horizontalAlign", "verticalAlign",
                         "leftPadding", "rightPadding"],
                        this.labelSub);
                    StyleDebugTree.describe (
                        "Report item " + model.index,
                        ["x", "y", "z", "width", "height", "iconSize"],
                        this);
                }
            }
        }
    }
}
