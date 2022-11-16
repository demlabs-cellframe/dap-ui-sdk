/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
//import DapQmlModelSettings 1.0
import Qt.labs.platform 1.1
import StyleDebugTree 1.0
import DragAndDropCtl 1.0
//import SettingsInterface 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Manage CDB Form
 * @ingroup groupDapQmlForms
 *
 * @date 17.10.22
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
    property string formName: "ManageCdb"

    enum Mode
    {
        M_LIST,
        M_EDIT,
        M_ADD
    }

    /// @brief controls form mode by aplying visibility rules
    property QtObject modeCtl: QtObject {
        property int mode: QuiManageCdbForm.Mode.M_LIST
        property int lastIndex: -1
        property bool isRegistered: false

        property RegExpValidator validatorAddress: RegExpValidator {
            regExp: /(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])/
        }
        property RegExpValidator validatorPort: RegExpValidator {
            regExp: /^(0|[1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$/
        }

        function switchToList() {
            setMode (QuiManageCdbForm.Mode.M_LIST);
        }

        function switchToEdit (a_index) {
            lastIndex   = a_index;
            fill (a_index);
            setMode (QuiManageCdbForm.Mode.M_EDIT);
        }

        function switchToAdd() {
            clearFields();
            setMode (QuiManageCdbForm.Mode.M_ADD);
        }

        function setMode (a_newMode) {
            /* store new mode */
            mode        = a_newMode;

            /* apply visibility */
            let list = (mode === QuiManageCdbForm.Mode.M_LIST);
            let edit = (mode === QuiManageCdbForm.Mode.M_EDIT);
            let add  = (mode === QuiManageCdbForm.Mode.M_ADD);

            /* display only required page */
            pageList.visible    = list;
            pageEdit.visible    = edit;
            pageAdd.visible     = add;

            /* both "edit" and "add" fields are the same */
            inputFields.visible = edit | add;
        }

        function clearFields() {
            inputAddress.clear();
            inputPort.value = 80; //.clear();
        }

        function fill (a_index) {
            let model   = mancdbListView.model;

            if (a_index < 0 || a_index >= model.length || a_index === undefined)
                return;

            inputAddress.value  = model.value (a_index, "server");
            inputPort.value     = model.value (a_index, "port");
        }

        function addNewServer() {
            /* get field values */
            let model   = mancdbListView.model;
            let address = inputAddress.value;
            let port    = inputPort.value;

            /* store result & update model */
            let result  = model.add ({server:address,port:port});
            if (result)
                model.refreshContent();
            return result;
        }

//        function removeServer (a_index) {
//            lastIndex   = a_index;

//            /* perform removing & update model */
//            let model   = mancdbListView.model;
//            model.remove (lastIndex);
//            model.refreshContent();
//        }

        function applyChanges() {
            /* get field values */
            let model   = mancdbListView.model;
            let address = inputAddress.value;
            let port    = inputPort.value;

            /* store result & update model */
            let result  = model.edit (lastIndex, {server:address,port:port});
            if (result)
                model.refreshContent();
            return result;
        }
    }

    property int globalX
    property int globalY
    property int dropIndex

    Timer {
        interval: 250
        running: true
        repeat: false
        onTriggered: {
            inputAddress.validator  = modeCtl.validatorAddress;
            inputPort.validator     = modeCtl.validatorPort;
        }
    }

    DragAndDropCtl {
        id: dragAndDrop
        objectName: "dragAndDrop"
    }


    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief remove server menu clicked
    signal sigRemove(int a_index);

    signal sigDragButtonPressed (int a_index, int a_quality, string a_label);
    signal sigDragButtonEntered();
    signal sigDragButtonExited();

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
        case QuiManageCdbForm.Mode.M_LIST:  modeCtl.switchToList(); break;
        case QuiManageCdbForm.Mode.M_EDIT:  modeCtl.switchToEdit (a_data); break;
        case QuiManageCdbForm.Mode.M_ADD:   modeCtl.switchToAdd(); break;
        }

        slotSetErrorText ("");
    }

    function addNewServer() {
        modeCtl.addNewServer();
    }

    function removeServer (a_index) {
        root.sigRemove (a_index);
        //modeCtl.removeServer (a_index);
    }

    function slotFinishRemove() {
        setMode (QuiManageCdbForm.Mode.M_LIST);
    }

    function slotSetErrorText (a_text) {
        errorLabel.text = a_text;
    }

    function applyChanges() {
        if (modeCtl.mode === QuiManageCdbForm.Mode.M_ADD)
            return modeCtl.addNewServer();
        else
        if (modeCtl.mode === QuiManageCdbForm.Mode.M_EDIT)
            return modeCtl.applyChanges();
    }

    function doImport() {
        if (mancdbListView.model === undefined)
            return;
        fileDialog.updateMode (FileDialog.OpenFile);
        fileDialog.open();
    }

    function doExport() {
        if (mancdbListView.model === undefined)
            return;
        fileDialog.updateMode (FileDialog.SaveFile);
        fileDialog.open();
    }

    function _fillGlobal() {
        let point   = root.mapToGlobal(0,0);
        globalX     = point.x;
        globalY     = point.y;
    }

    function _pos (a_index) {
        return title.y + (title.height * 2.4) + (resizerItem.height * a_index);
    }

    Component.onCompleted: {
        _fillGlobal();
        setMode (QuiManageCdbForm.Mode.M_LIST);
        dragAndDrop.activate();
    }

    Component.onDestruction: {
        dragAndDrop.deactivate();
    }

    /// @}
    /****************************************//**
     *
     * Debug
     *
     ********************************************/

//    Button {
//        x: 4; y: 4; z: 10; width: 64; height: 28; text: "list"
//        onClicked: root.setMode (QuiManageCdbForm.Mode.M_LIST)
//    }

//    Button {
//        x: 4+72; y: 4; z: 10; width: 64; height: 28; text: "edit"
//        onClicked: root.setMode (QuiManageCdbForm.Mode.M_EDIT)
//    }

//    Button {
//        x: 4+144; y: 4; z: 10; width: 64; height: 28; text: "add"
//        onClicked: root.setMode (QuiManageCdbForm.Mode.M_ADD)
//    }

    /****************************************//**
     * Import/Export dialog
     ********************************************/

    FileDialog {
        id: fileDialog

        property string titleName;
        property string filterImages:   qsTr("Json files (*.json)") + lang.notifier;
        property string filterAllFiles: qsTr("All files (*.*)") + lang.notifier;

        onAccepted: {
            var result      = new String (currentFile);
            var filename    = result.slice(7);

            if (fileMode === FileDialog.OpenFile)
            {
                mancdbListView.model.doImport (filename);
                mancdbListView.model.refreshContent();
            }
            else
            if (fileMode === FileDialog.SaveFile)
                mancdbListView.model.doExport (filename);

            fileDialog.close()
        }

        function updateMode (a_newMode) {
            /* save mode */
            fileMode        = a_newMode;

            /* update title based on new mode */
            titleName       = (fileMode === FileDialog.OpenFile)
            ? qsTr("Open JSON") + lang.notifier
            : qsTr("Save JSON") + lang.notifier;

            /* complete */
            fileDialog.title       = titleName;
            fileDialog.nameFilters = [ filterImages, filterAllFiles ];
        }
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer c-grey"
    }

    DapQmlLabel {
        id: resizeField
        visible: false
        qss: "mancdb-resizer-field c-brand"
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
            text: qsTr("Manage CDB") + lang.notifier
            qss: "dialog-title"

            /* settings button */
            DapQmlPushButton {
                id: settingsBtn
                qss: "mancdb-settings-btn"

                x: parent.width - width
                y: (parent.height - height) / 2
                z: 16

                onClicked: settingsBtnMenu.popup();

                DapQmlMenu {
                    id: settingsBtnMenu

                    /* actions */
                    Action { text: "Add new"; onTriggered: root.setMode (QuiManageCdbForm.Mode.M_ADD) }
                    Action { text: "Import list"; onTriggered: root.doImport(); }
                    Action { text: "Export list"; onTriggered: root.doExport(); }
                }
            }
        }

        /****************************************//**
         * Content
         ********************************************/

        ListView {
            id: mancdbListView
            objectName: "mancdbListView"
            //interactive: true

            property int dragItemIndex: -1
            property int dropItemIndex: -1

            x: (root.width - width) / 2
            y: title.y + title.height * 2
            width: resizerItem.width
            height: root.height - y
            clip: true

            function dropItem() {
                console.log (`dropItem ${dragItemIndex} to ${dropItemIndex}`);

                dragIsActive        = false;
                placeholder.parent  = mancdbListView;

                let model           = mancdbListView.model;

                model.move (dragItemIndex, dropItemIndex);
                model.refreshContent();

                dragItemIndex       = -1;
                dropItemIndex       = -1;
            }

            /****************************************//**
             * Resizers
             ********************************************/

            DapQmlLabel {
                visible: false
                id: resizerItem
                qss: "mancdb-resizer-item c-background"
                //onColorChanged: console.log(`resizerItem >> bgcolor: ${resizerItem.color}, textcolor: ${resizeField.color}`);
            }

            /****************************************//**
             * Centered dragged item
             ********************************************/

            Rectangle {
                id: graggedItem
                objectName: "draggedItem"
                z: 20

                width: mancdbListView.width
                height: resizerItem.height

                color: resizerItem.color
                border.color: resizer.color
                border.width: 1
                radius: 6

                function setMainText(a_text) {
                    graggedItemLabel.text   = a_text;
                }

                function setIndex(a_index) {
                    graggedItemIndexLabel.text  = a_index + 1;
                }

                function setQuality(a_quality) {
                    graggedItemConnQualityIndicator.quality =
                        ((a_quality === -1) ? (0) : (5 - a_quality));
                }

                function calc() {
                    return height / 2;
                }

                /* icon */
                DapQmlLabel {
                    y: (parent.height - height) / 2
                    z: 21
                    width: resizerItem.fontSize * 0.725
                    height: resizerItem.fontSize * 0.725
                    qss: "ic_cdb-index-icon"
                }

                /* index */
                DapQmlLabel {
                    id: graggedItemIndexLabel
                    y: (parent.height - height) / 2
                    z: 22
                    width: resizerItem.fontSize * 0.725
                    height: resizerItem.fontSize * 0.725
                    qss: "mancdb-btn-lbl-main c-background"
                    text: dragAndDrop.dropIndex + 1
                }

                /* connection quality */
                DapQmlLabel {
                    id: graggedItemConnQualityIndicator
                    property int quality
                    x: parent.width - (width * 1.35) - graggedItemMoreBtn.width
                    y: (parent.height - height) / 2
                    width: resizer.height * 0.5
                    height: resizer.height * 0.5
                    qss: `ic_conn-${quality}`
                }

                Button {
                    id: graggedItemMoreBtn
                    icon {
                        source: "qrc:/nonthemed/drag-drop-icon.png"
                        color: "transparent"
                        width: graggedItemMoreBtn.width * 0.8
                        height: graggedItemMoreBtn.height * 0.8
                    }
                    background: Rectangle {
                        color: "transparent"
//                        border.color: "black"
//                        border.width: 1
//                        radius: 6
                    }

                    x: parent.width - width
                    y: (parent.height - height) / 2 - height / 8
                    z: 16
                    width: resizerItem.fontSize * 1.25
                    height: resizerItem.fontSize * 1.25
                }

                /* label */
                DapQmlLabel {
                    id: graggedItemLabel
                    x: resizerItem.fontSize * 0.9
                    z: 23
                    width: parent.width - x
                    height: parent.height
                    text: ""
                    horizontalAlign: Text.AlignLeft
                    qss: "mancdb-btn-lbl-main"
                }
            }


            /****************************************//**
             * Delegate
             ********************************************/

            delegate: Item {
                id: delegateItem
                width: mancdbListView.width
                height: resizerItem.height
                opacity: (graggedItem.visible && root.dropIndex === myIndex) ? 0 : 1

                property int myIndex: model.index

                DapQmlButton {
                    id: delegate

                    property int myIndex: model.index// + mancdbListView.model.notifyInt
                    property int ping: model.ping + mancdbListView.model.notifyInt
                    property int quality: calcConnectionQuality() + mancdbListView.model.notifyInt
                    property int globalX
                    property int globalY

                    Component.onCompleted: mancdbListView.model.regRow (this);

                    function updateValues() {
                        ping        = model.ping;
                        calcConnectionQuality();
                    }

                    function calcConnectionQuality() {
                        let a = (ping === -1) ? -1 : ping / 400;
                        //let b = a;

                        if (a > 5)
                            a = 5;

                        quality = a;

                        //console.log (`cdb item:${model.name}~${quality}|${a}|${ping}`);
                        return quality;
                    }

                    function updatePositions() {
                        let point   = mapToGlobal(0,0);
                        globalX     = point.x - root.globalX;
                        globalY     = point.y - root.globalY;

                        if (!modeCtl.isRegistered)
                        {
                            modeCtl.isRegistered    = true;
                            mancdbListView.model.regWindow (root);
                        }
                    }

                    width: mancdbListView.width
                    height: resizerItem.height
                    buttonStyle: DapQmlButton.Style.IconMainSubIcon
                    mainText: model.name + mancdbListView.model.notifyString
                    subText: "" //count()// + mancdbListView.model.notifyString
                    separator: true
                    qss: "mancdb-item"
                    mainQss: "mancdb-btn-lbl-main"
                    //subQss: "mancdb-btn-lbl-sub"
                    icon: "ic_cdb-index-icon"
                    iconSize: resizerItem.fontSize * 0.75

                    /* index */
                    DapQmlLabel {
                        y: (parent.height - height) / 2 - height * 0.25
                        width: resizerItem.fontSize * 0.75
                        height: resizerItem.fontSize * 0.75
                        qss: "mancdb-btn-lbl-main c-background"
                        text: parent.myIndex + 1
                    }

                    /* connection quality */
                    DapQmlLabel {
                        id: connQualityIndicator
                        property int quality: ((parent.quality === -1) ? (0) : (5 - parent.quality)) + mancdbListView.model.notifyInt
                        x: parent.width - (width * 1.35) - moreBtn.width
                        y: (parent.height - height) / 2
                        width: resizer.height * 0.5
                        height: resizer.height * 0.5
                        qss: `ic_conn-${quality}`

                        ToolTip {
                            id: id_tooltip
                            contentItem: Text{
                                color: "#21be2b"
                                text: ((delegate.ping === -1) ? "unavailable" : `ping ${delegate.ping} ms`) + mancdbListView.model.notifyString
                            }
                            background: Rectangle {
                                border.color: "#21be2b"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: id_tooltip.visible = true
                            onExited: id_tooltip.visible = false
                        }
                    }

                    /* more button */
                    Button {
                        id: moreBtn
                        icon {
                            source: "qrc:/nonthemed/drag-drop-icon.png"
                            color: "transparent"
                            width: moreBtn.width * 0.8
                            height: moreBtn.height * 0.8
                        }
                        background: Rectangle {
                            color: "transparent"
//                            border.color: "black"
//                            border.width: 1
//                            radius: 6
                        }

                        property int myIndex

                        Component.onCompleted: myIndex = parent.myIndex;

                        x: parent.width - width
                        y: (parent.height - height) / 2 - height / 8
                        z: 16
                        width: resizerItem.fontSize * 1.25
                        height: resizerItem.fontSize * 1.25

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: root.sigDragButtonEntered()
                            onExited: root.sigDragButtonExited()
                            onPressed: root.sigDragButtonPressed (parent.myIndex, delegate.quality, delegate.mainText);
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.setMode (QuiManageCdbForm.Mode.M_EDIT, parent.myIndex)
                    }
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
         * Title Buttons
         ********************************************/

        /* accept button */
        DapQmlPushButton {
            id: acceptBtn
            qss: "mancdb-accept-btn"

            x: title.x + title.width - width
            y: title.y + (title.height - height) / 2
            z: 16

            onClicked: {
                if (root.applyChanges())
                    root.setMode (QuiManageCdbForm.Mode.M_LIST)
            }
        }

        /* cancel button */
        DapQmlPushButton {
            id: cancelBtn
            qss: "mancdb-cancel-btn"

            x: title.x
            y: title.y + (title.height - height) / 2
            z: 16

            onClicked: root.setMode (QuiManageCdbForm.Mode.M_LIST)
        }

        /****************************************//**
         * Error label
         ********************************************/

        DapQmlLabel {
            id: errorLabel
            qss: "mancdb-error-label c-error"
        }

        /****************************************//**
         * Adress Input
         ********************************************/

        DapQmlInputField {
            id: inputAddress
            x: (root.width - width) / 2
            y: _pos(0) // title.y + title.height * 2
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Address"
            //inputMask: "000.000.000.000"
        }

        /****************************************//**
         * Port Input
         ********************************************/

        DapQmlInputField {
            id: inputPort
            x: (root.width - width) / 2
            y: _pos(1) // title.y + title.height * 2
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Port"
        }

        /****************************************//**
         * Remove button
         ********************************************/

        DapQmlLabel {
            id: btnRemove
            y: _pos(2) + height * 3
            text: qsTr("Remove server") + lang.notifier
            visible: root.modeCtl.mode === QuiManageCdbForm.Mode.M_EDIT
            qss: "manser-remove-btn c-brand"

            MouseArea {
                id: btnRemoveMouseArea
                anchors.fill: btnRemove
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    removeServer (root.modeCtl.lastIndex);
                    //root.setMode (QuiManageCdbForm.Mode.M_LIST)
                }
            }
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
