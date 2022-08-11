/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
//import DapQmlModelSettings 1.0
import StyleDebugTree 1.0
//import SettingsInterface 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Manage Servers Form
 * @ingroup groupDapQmlForms
 *
 * @date 15.07.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "Manage Servers"

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Manage Servers") + lang.notifier
        qss: "dialog-title"
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    /****************************************//**
     * Content
     ********************************************/

    Rectangle {
        id: manserContanier
        anchors.fill: parent
        color: "transparent"

        ListView {
            id: manserListView
            objectName: "manserListView"

            x: (root.width - width) / 2
            y: title.y + title.height * 2
            width: resizer.width
            height: root.height - y
            clip: true

            /****************************************//**
             * Resizers
             ********************************************/

            DapQmlRectangle {
                visible: false
                id: resizer1
                qss: "manser-resizer1"
            }

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: DapQmlButton {
                id: delegate

                property int myIndex: model.index
                property int mySid: model.sid

                width: settingsListView.width
                height: resizer1.height
                buttonStyle: DapQmlButton.Style.IconMainSubIcon
                mainText: model.textMain
                subText: ""
                separator: true
                qss: "manser-item"
                mainQss: "manser-btn-lbl-main"
                //subQss: "manser-btn-lbl-sub"
                icon: model.icon
                iconSize: resizer1.fontSize

                function buttonClicked(a_isButtonSignal) {
                    if(!a_isButtonSignal)
                        clicked();
                    model.exec (myIndex, this);
                }

                onClicked: buttonClicked(true)

                MouseArea {
                    anchors.fill: parent
                    onClicked: parent.buttonClicked(false)
                }
            }
        }
    }
}

/*-----------------------------------------*/
