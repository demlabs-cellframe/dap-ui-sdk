import QtQuick 2.4
import QtQuick.Layouts 1.3
import StyleDebugTree 1.0
import DapQmlStyle 1.0
import Brand 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    objectName: "settingsForm"
    clip: true

    enum StyleId
    {
      SI_TITLE,
      SI_TITLETOP,
      SI_BUTTON,
      SI_BUTTONRED,
      SI_BUTTONGRAY,
      SI_LINK,
      SI_CHECKBOX,
      SI_SPACER
    }

    property string formName: "Settings"

    property bool darkTheme: false

    property QtObject internal: QtObject {
        property bool disableAnim: false
        property var items: new Array();

        function appendItem (item) {
            items.push (item);
        }

        function removeItem (item) {
            var index = items.indexOf(item);
            if (index > -1)
                items.splice(index, 1);
        }
    }

    function isSep(sid) {
        if (sid === QuiSettingsForm.StyleId.SI_BUTTON
        || sid === QuiSettingsForm.StyleId.SI_BUTTONRED
        || sid === QuiSettingsForm.StyleId.SI_BUTTONGRAY
        || sid === QuiSettingsForm.StyleId.SI_LINK
        || sid === QuiSettingsForm.StyleId.SI_CHECKBOX)
            return true;
        return false;
    }

    function calcHeight(sid) {

        switch(sid) {
        case QuiSettingsForm.StyleId.SI_TITLE: return resizer2.height;
        case QuiSettingsForm.StyleId.SI_SPACER: return spacer.height;
        }

        return resizer1.height;
    }

    function disableColorAnim() {
        root.internal.disableAnim = true;
        disableAnimInterval.start();
    }

    Timer {
        id: disableAnimInterval
        interval: 1000
        running: false
        repeat: false
        onTriggered: root.internal.disableAnim = false
    }

    DapQmlDummy {
        id: contentRect
        qss: "content-mid"
    }

    Rectangle {
        id: settingsContainer
        anchors.fill: parent
        color: "transparent"

        ListView {
            id: settingsListView
            objectName: "settingsListView"

            x: (parent.width - width) / 2 //contentRect.x
            y: 0
            width: listviewSizer.width ? listviewSizer.width : contentRect.width // root.width - 72
            height: root.height

            clip: false

            DapQmlLabel {
                visible: false
                id: resizer1
                qss: "sett-resizer1"
            }

            DapQmlLabel {
                visible: false
                id: resizer2
                qss: "sett-resizer2"
            }

            DapQmlLabel {
                visible: false
                id: spacer
                qss: "sett-spacer"
            }

            DapQmlDummy {
                id: listviewSizer
                qss: "sett-content";
            }

            DapQmlDummy {
                id: lvHoverColor
                qss: "sett-btn-hover-bg";
                property string color
            }

            DapQmlDummy {
                id: lvNormalColor
                qss: "c-background";
                property string color
            }

            delegate: Item {
                id: delegate
                width: settingsListView.width
                height: calcHeight (model.sid)
                clip: true

                property int mySid: model.sid
                property var settingsModel: settingsListView.model
                property bool hovered: false

                DapQmlButton {
                    property int myIndex: model.index
                    property string myText: model.textMain + settingsModel.notifier
                    property bool preventDoubleExec: false

                    visible: model.sid !== QuiSettingsForm.StyleId.SI_TITLE
                    x: (parent.width - width) / 2
                    z: 50
                    width: contentRect.width
                    height: delegate.height
                    buttonStyle: DapQmlButton.Style.IconMainSub
                    mainText: myText
                    subText: model.textSub + settingsModel.notifier
                    separator: isSep(model.sid)
                    qss: "sett-item"
                    mainQss: "sett-btn-lbl-main"
                    subQss: {
                        model.sid === QuiSettingsForm.StyleId.SI_LINK ? "sett-btn-lbl-sub-link" :
                        model.sid === QuiSettingsForm.StyleId.SI_BUTTONRED ? "sett-btn-lbl-sub-red" : "sett-btn-lbl-sub"
                    }
                    link: model.sid === QuiSettingsForm.StyleId.SI_LINK
                    checkbox: model.sid === QuiSettingsForm.SI_CHECKBOX
                    checked: (model.icon === "settings_icon ic_theme") ? root.darkTheme : false
                    icon: model.icon
                    iconSize: resizer1.fontSize
                    disableClicking: (model.icon === "settings_icon ic_key" && settingsModel.connectWalletBusy)
                    mouseArea.hoverEnabled: true
                    mouseArea.onEntered: delegate.hovered   = true
                    mouseArea.onExited:  delegate.hovered   = false

                    function buttonClicked(a_isButtonSignal) {
                        if (preventDoubleExec)
                            return;

                        preventDoubleExec   = true;

                        if(a_isButtonSignal === false)
                            clicked();

                        settingsModel.exec (myIndex, this);

                        preventDoubleExec   = false;

                        if (model.icon === "settings_icon ic_theme")
                            root.darkTheme = checked;
                    }

                    onClicked: buttonClicked(true)
                    MouseArea {
                        anchors.fill: parent
                        onClicked: parent.buttonClicked(false)
                    }

                    onMyTextChanged: mainText = myText;

                    Component.onCompleted: root.internal.appendItem (this)
                    Component.onDestruction: root.internal.removeItem (this)
                }

                DapQmlSeparator {
                    y: parent.height - height
                    width: settingsListView.width
                    z: 40
                    visible: model.sid !== QuiSettingsForm.StyleId.SI_TITLE
                             && isSep(model.sid)
                }

                Rectangle {
                    anchors.fill: parent
                    anchors.rightMargin: 0-1
                    z: 10
                    visible: !Brand.legacyStyle() && model.index > 0
                    color: delegate.hovered ? lvHoverColor.color : lvNormalColor.color
                    Behavior on color { PropertyAnimation { duration: root.internal.disableAnim ? 0 : 150 } }
                }

                DapQmlLabel {
                    z: 40
                    visible: model.sid === QuiSettingsForm.StyleId.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    text: model.textMain + settingsModel.notifier
                    qss: "sett-title-lbl-main"
                }
            }
        }
    }
}

/*-----------------------------------------*/
