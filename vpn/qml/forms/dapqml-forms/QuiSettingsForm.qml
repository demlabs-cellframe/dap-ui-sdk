/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
//import DapQmlModelSettings 1.0
import StyleDebugTree 1.0
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

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function isSep(sid) {
        if (sid === QuiSettingsForm.StyleId.SI_BUTTON
        || sid === QuiSettingsForm.StyleId.SI_BUTTONRED
        || sid === QuiSettingsForm.StyleId.SI_BUTTONGRAY
        || sid === QuiSettingsForm.StyleId.SI_LINK)
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

    /****************************************//**
     * Content
     ********************************************/

    Rectangle {
        id: settingsContainer
        anchors.fill: parent
        color: "transparent"

        ListView {
            id: settingsListView
            objectName: "settingsListView"

            x: 36
            y: 0
            width: root.width - 72
            height: root.height

            clip: true
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

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: Item {
                id: delegate
                height: calcHeight (model.sid) //model.sid !== QuiSettingsForm.StyleId.SI_TITLE ? resizer1.height : resizer2.height
                property int mySid: model.sid
                property var settingsModel: settingsListView.model

                DapQmlButton {
                    property int myIndex: model.index
                    visible: model.sid !== QuiSettingsForm.StyleId.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    buttonStyle: DapQmlButton.Style.IconMainSub
                    mainText: model.textMain + lang.notifier
                    subText: model.textSub + lang.notifier
                    separator: isSep(model.sid)
                    qss: "sett-item"
                    mainQss: "sett-btn-lbl-main"
                    subQss: model.sid !== QuiSettingsForm.StyleId.SI_BUTTONRED ? "sett-btn-lbl-sub" : "sett-btn-lbl-sub-red"
                    link: model.sid === QuiSettingsForm.StyleId.SI_LINK
                    icon: model.icon
                    iconSize: resizer1.fontSize
                    MouseArea {
                        anchors.fill: parent
                        onClicked: settingsModel.exec(parent.myIndex)
                    }
                }

                DapQmlLabel {
                    visible: model.sid === QuiSettingsForm.StyleId.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    text: model.textMain + lang.notifier
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

/*-----------------------------------------*/
