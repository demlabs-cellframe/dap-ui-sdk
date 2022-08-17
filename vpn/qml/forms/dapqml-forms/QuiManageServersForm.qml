/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
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
    property string formName: "ManageServers"

    enum Mode
    {
        M_LIST,
        M_EDIT,
        M_ADD
    }

    /// @brief controls form mode by aplying visibility rules
    property QtObject modeCtl: QtObject {
        property int mode: QuiManageServersForm.Mode.M_LIST
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief get current mode
    function mode() {
        return modeCtl.mode;
    }

    /// @brief set new mode
    function setMode(a_newMode) {
        /* store new mode */
        modeCtl.mode        = a_newMode;

        /* apply visibility */
        let list = (modeCtl.mode === QuiManageServersForm.Mode.M_LIST);
        let edit = (modeCtl.mode === QuiManageServersForm.Mode.M_EDIT);
        let add  = (modeCtl.mode === QuiManageServersForm.Mode.M_ADD);
        pageList.visible    = list;
        pageEdit.visible    = edit;
        pageAdd.visible     = add;
        inputFields.visible = edit | add;
    }

    Component.onCompleted: setMode (QuiManageServersForm.Mode.M_LIST)

    /// @}
    /****************************************//**
     *
     * Debug
     *
     ********************************************/

    Button {
        x: 4; y: 4; z: 10; width: 64; height: 28; text: "list"
        onClicked: root.setMode (QuiManageServersForm.Mode.M_LIST)
    }

    Button {
        x: 4+72; y: 4; z: 10; width: 64; height: 28; text: "edit"
        onClicked: root.setMode (QuiManageServersForm.Mode.M_EDIT)
    }

    Button {
        x: 4+144; y: 4; z: 10; width: 64; height: 28; text: "add"
        onClicked: root.setMode (QuiManageServersForm.Mode.M_ADD)
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlLabel {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlLabel {
        id: resizeField
        visible: false
        qss: "manser-resizer-field"
    }

    DapQmlLabel {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    /****************************************//**
     *
     * List
     *
     ********************************************/

    Item {
        id: pageList
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            id: title
            text: qsTr("Manage Servers") + lang.notifier
            qss: "dialog-title"
        }

        /****************************************//**
         * Content
         ********************************************/

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

            DapQmlLabel {
                visible: false
                id: resizerItem
                qss: "manser-resizer-item"
            }

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: DapQmlButton {
                id: delegate

                property int myIndex: model.index

                width: manserListView.width
                height: resizerItem.height
                buttonStyle: DapQmlButton.Style.IconMainSubIcon
                mainText: model.name
                subText: ""
                separator: true
                qss: "manser-item"
                mainQss: "manser-btn-lbl-main"
                //subQss: "manser-btn-lbl-sub"
                icon: model.icon
                iconSize: resizerItem.fontSize

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
    } // List

    /****************************************//**
     *
     * Input Fields
     *
     ********************************************/

    Item {
        id: inputFields
        anchors.fill: parent

        /****************************************//**
         * Title Input
         ********************************************/

        DapQmlInputField {
            id: inputTitle
            x: (root.width - width) / 2
            y: title.y + title.height * 2
            z: 2
            width: resizer.width
            height: resizeField.height
            clip: true
            title: "Title"
        }

        /****************************************//**
         * Adress Input
         ********************************************/

        DapQmlInputField {
            id: inputAddress
            x: (root.width - width) / 2
            y: inputTitle.y + resizerItem.height
            width: resizer.width
            height: resizeField.height
            clip: true
            title: "Address"
        }

        /****************************************//**
         * Port Input
         ********************************************/

        DapQmlInputField {
            id: inputPort
            x: (root.width - width) / 2
            y: inputAddress.y + resizerItem.height
            width: resizer.width
            height: resizeField.height
            clip: true
            title: "Port"
        }

    }

    /****************************************//**
     *
     * Edit
     *
     ********************************************/

    Item {
        id: pageEdit
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Edit server") + lang.notifier
            qss: "dialog-title"
        }

    } // Edit

    /****************************************//**
     *
     * Add
     *
     ********************************************/

    Item {
        id: pageAdd
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Add a new server") + lang.notifier
            qss: "dialog-title"
        }
    } // Add
}

/*-----------------------------------------*/
