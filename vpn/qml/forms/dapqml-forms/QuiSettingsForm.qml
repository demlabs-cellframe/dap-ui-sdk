/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
//import DapQmlModelSettings 1.0
import StyleDebugTree 1.0
import DapQmlStyle 1.0
import Brand 1.0
//import SettingsInterface 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Settings Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using ListView with DapQmlButton and DapQmlLabel as delegate
 *
 * Delegate choses when to display DapQmlButton or DapQmlLabel
 *
 * If DapQmlButton is clicked, delegate executes settingsModel.exec static method
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

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

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "Settings"
    //DapQmlModelSettings { id: settingsModel }
    //SettingsInterface { id: settingsInterface; Component.onCompleted: setup(settingsModel); }

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

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

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

    /****************************************//**
     * Resizer
     ********************************************/

    DapQmlDummy {
        id: contentRect
        qss: "content-mid"
    }

    /****************************************//**
     * Content
     ********************************************/

//    property string testText: "test"
//    property int testIndex: 0
//    Timer {
//        interval: 1000
//        running: true
//        repeat: true
//        onTriggered: {
//            if(testIndex < 3)
//                testIndex++
//            else
//                testIndex = 0;

//            testText    = "test";

//            for(let i = 0; i < testIndex; i++)
//                testText += ".";
//        }
//    }

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
            //model: settingsModel


            /****************************************//**
             * Resizers
             ********************************************/

            /* this item simulates resizing to give values:*/
            /* height1 -> item.height, */
            /* height2 -> title.height, */
            /* fontSize1 -> item.iconSize */
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

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: Item {
                id: delegate
                width: settingsListView.width
                height: calcHeight (model.sid) //model.sid !== QuiSettingsForm.StyleId.SI_TITLE ? resizer1.height : resizer2.height
                clip: true

                property int mySid: model.sid
                property var settingsModel: settingsListView.model
                property bool hovered: false

                DapQmlButton {
                    property int myIndex: model.index
                    property string myText: model.textMain + settingsModel.notifier // + testText
                    property bool preventDoubleExec: false

                    visible: model.sid !== QuiSettingsForm.StyleId.SI_TITLE
                    x: (parent.width - width) / 2
                    z: 50
                    width: contentRect.width // settingsListView.width
                    height: delegate.height
                    buttonStyle: DapQmlButton.Style.IconMainSub
                    mainText: myText // model.textMain + settingsModel.notifier + testText
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
                    }

                    onClicked: buttonClicked(true)
                    MouseArea {
                        anchors.fill: parent
                        onClicked: parent.buttonClicked(false)
//                        {
//                            parent.clicked();
//                            settingsModel.exec (parent.myIndex, parent);
//                        }
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

//                DapQmlRectangle {
//                    anchors.fill: parent
//                    anchors.rightMargin: 0-1
//                    z: 10
//                    visible: !Brand.legacyStyle() && model.index > 0
//                    qss: delegate.hovered ? "sett-btn-hover-bg" : "c-background"
//                    Behavior on color { PropertyAnimation { duration: root.internal.disableAnim ? 0 : 150 } }
//                }

                DapQmlLabel {
                    z: 40
                    visible: model.sid === QuiSettingsForm.StyleId.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    text: model.textMain + settingsModel.notifier // + testText
                    qss: "sett-title-lbl-main"
                }

//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Item " + model.index,
//                    ["width", "mySid"],
//                   this);
            }
        }
    }
}

/*-----------------------------------------*/
