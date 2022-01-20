import QtQuick 2.4
import QtQuick.Layouts 1.3
import DapQmlModelSettings 1.0
import StyleDebugTree 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    clip: true
    DapQmlModelSettings { id: settingsModel }

    function isSep(sid) {
        if (sid === DapQmlModelSettings.SI_BUTTON
        || sid === DapQmlModelSettings.SI_BUTTONRED
        || sid === DapQmlModelSettings.SI_BUTTONGRAY
        || sid === DapQmlModelSettings.SI_LINK)
            return true;
        return false;
    }

    Rectangle {
        anchors.fill: parent
        color: "#F7F8FA"

        ListView {
            id: settingsListView

            x: 36
            y: 0
            width: root.width - 72
            height: root.height

            clip: true
            model: settingsModel

            /* this item simulates resizing to give values:*/
            /* height1 -> item.height, */
            /* height2 -> title.height, */
            /* fontSize1 -> item.iconSize */
            DapQmlLabel {
                visible: false
                id: resizer1
                qss: "sett-resizer1"
//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Resizer 1",
//                    ["x", "y", "width", "height"],
//                   this);
            }

            DapQmlLabel {
                visible: false
                id: resizer2
                qss: "sett-resizer2"
//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Resizer 2",
//                    ["x", "y", "width", "height"],
//                   this);
            }

            delegate: Item {
                id: delegate
                height: model.sid !== DapQmlModelSettings.SI_TITLE ? resizer1.height : resizer2.height
                property int mySid: model.sid

                DapQmlButton {
                    property int myIndex: model.index
                    visible: model.sid !== DapQmlModelSettings.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    buttonStyle: DapQmlButton.Style.IconMainSub
                    mainText: model.textMain
                    subText: model.textSub
                    separator: isSep(model.sid)
                    qss: "sett-item"
                    mainQss: "sett-btn-lbl-main"
                    subQss: "sett-btn-lbl-sub"
                    icon: model.icon
                    iconSize: resizer1.fontSize
                    MouseArea {
                        anchors.fill: parent
                        onClicked: settingsModel.exec(parent.myIndex)
                    }
                }

                DapQmlLabel {
                    visible: model.sid === DapQmlModelSettings.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    text: model.textMain
                    qss: "sett-title-lbl-main"
                }

//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Item " + model.index,
//                    ["height", "mySid"],
//                   this);
            }
        }
    }
}
