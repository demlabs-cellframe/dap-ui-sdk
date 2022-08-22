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
        property int lastIndex: -1

        function switchToList() {
            setMode (QuiManageServersForm.Mode.M_LIST);
        }

        function switchToEdit (a_index) {
            lastIndex   = a_index;
            fill (a_index);
            setMode (QuiManageServersForm.Mode.M_EDIT);
        }

        function switchToAdd() {
            clearFields();
            setMode (QuiManageServersForm.Mode.M_ADD);
        }

        function setMode (a_newMode) {
            /* store new mode */
            mode        = a_newMode;

            /* apply visibility */
            let list = (mode === QuiManageServersForm.Mode.M_LIST);
            let edit = (mode === QuiManageServersForm.Mode.M_EDIT);
            let add  = (mode === QuiManageServersForm.Mode.M_ADD);

            /* display only required page */
            pageList.visible    = list;
            pageEdit.visible    = edit;
            pageAdd.visible     = add;

            /* both "edit" and "add" fields are the same */
            inputFields.visible = edit | add;
        }

        function clearFields() {
            inputTitle.clear();
            inputAddress.clear();
            inputPort.clear();
        }

        function fill (a_index) {
            let model   = manserListView.model;

            if (a_index < 0 || a_index >= model.length || a_index === undefined)
                return;

            inputTitle.value      = model.value (a_index, "name");
            inputAddress.value    = model.value (a_index, "address");
            inputPort.value       = model.value (a_index, "port");
        }

        function applyChanges() {
            /* get field values */
            let name    = inputTitle.value;
            let address = inputAddress.value;
            let port    = Number (inputPort.value);

            /* store result & update model */
            let model   = manserListView.model;
            model.edit (lastIndex, {name:name, address:address, port:port});
            model.refreshContent();
        }
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
    function setMode (a_newMode, a_data) {

        switch(a_newMode)
        {
        case QuiManageServersForm.Mode.M_LIST:  modeCtl.switchToList(); break;
        case QuiManageServersForm.Mode.M_EDIT:  modeCtl.switchToEdit (a_data); break;
        case QuiManageServersForm.Mode.M_ADD:   modeCtl.switchToAdd(); break;
        }

        // modeCtl.setMode(a_newMode);
    }

    function applyChanges() {
        modeCtl.applyChanges();
    }

    function _pos (a_index) {
        return title.y + (title.height * 2.4) + (resizerItem.height * a_index);
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

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlLabel {
        id: resizeField
        visible: false
        qss: "manser-resizer-field"
        //onColorChanged: console.log(`resizeField >> bgcolor: ${resizerItem.color}, textcolor: ${resizeField.color}`);
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

            /* settings button */
            DapQmlPushButton {
                id: settingsBtn
                qss: "manser-settings-btn"

                x: parent.width - width
                y: (parent.height - height) / 2
                z: 16

                onClicked: settingsBtnMenu.popup();

                DapQmlMenu {
                    id: settingsBtnMenu

                    /* actions */
                    Action { text: "Restart server"; }
                    Action { text: "Add new"; onTriggered: root.setMode (QuiManageServersForm.Mode.M_ADD) }
                    Action { text: "Import list" }
                    Action { text: "Export list" }
                }
            }
        }

        /****************************************//**
         * Content
         ********************************************/

        ListView {
            id: manserListView
            objectName: "manserListView"

            x: (root.width - width) / 2
            y: title.y + title.height * 2
            width: resizerItem.width
            height: root.height - y
            clip: true

            /****************************************//**
             * Resizers
             ********************************************/

            DapQmlLabel {
                visible: false
                id: resizerItem
                qss: "manser-resizer-item"
                //onColorChanged: console.log(`resizerItem >> bgcolor: ${resizerItem.color}, textcolor: ${resizeField.color}`);
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

                /* icon favorite */
                Button {
                    id: checkFavorite
                    checkable: true
                    checked: model.favorite
                    icon {
                        source: (checkFavorite.checked)
                                ? "qrc:/nonthemed/star-checked.png"
                                : "qrc:/nonthemed/star-unchecked.png"
                        color: "transparent"
                        width: checkFavorite.width
                        height: checkFavorite.height
                    }
                    background: Rectangle { color: "transparent" }

                    x: parent.width - width * 2// - (74 / 2)
                    y: (parent.height - height) / 2 - height / 8
                    z: 16
                    width: parent.iconSize * 1.25
                    height: parent.iconSize * 1.25

                    //onCheckedChanged: internal.showPassword = checked
                }

                /* more button */
                Button {
                    id: moreBtn
                    icon {
                        source: "qrc:/nonthemed/more.png"
                        color: "transparent"
                        width: moreBtn.width
                        height: moreBtn.height
                    }
                    background: Rectangle { color: "transparent" }
                    property int myIndex: parent.myIndex

                    x: parent.width - width
                    y: (parent.height - height) / 2 - height / 8
                    z: 16
                    width: parent.iconSize * 1.25
                    height: parent.iconSize * 1.25

                    onClicked: moreBtnMenu.popup();

                    DapQmlMenu {
                        id: moreBtnMenu
                        property int myIndex: parent.myIndex

                        /* actions */
                        Action { text: "Edit"; onTriggered: root.setMode (QuiManageServersForm.Mode.M_EDIT, myIndex) }
                        Action { text: "Delete" }
                    }
                }

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

//                Component.onCompleted: if (myIndex === 0)
//                    StyleDebugTree.describe (
//                        "manser-item",
//                        ["x", "y", "width", "height", "icon", "iconSize"],
//                        this);
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
         * Title Buttons
         ********************************************/

        /* accept button */
        DapQmlPushButton {
            id: acceptBtn
            qss: "manser-accept-btn"

            x: title.x + title.width - width
            y: title.y + (title.height - height) / 2
            z: 16

            onClicked: {
                root.applyChanges();
                root.setMode (QuiManageServersForm.Mode.M_LIST)
            }
        }

        /* cancel button */
        DapQmlPushButton {
            id: cancelBtn
            qss: "manser-cancel-btn"

            x: title.x
            y: title.y + (title.height - height) / 2
            z: 16

            onClicked: root.setMode (QuiManageServersForm.Mode.M_LIST)
        }

        /****************************************//**
         * Title Input
         ********************************************/

        DapQmlInputField {
            id: inputTitle
            x: (root.width - width) / 2
            y: _pos(0) // title.y + title.height * 2
            width: resizerItem.width
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
            y: _pos(1) // inputTitle.y + resizerItem.height
            width: resizerItem.width
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
            y: _pos(2) // inputAddress.y + resizerItem.height
            width: resizerItem.width
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
            hideClose: true
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
            hideClose: true
        }
    } // Add
}

/*-----------------------------------------*/
