/* INCLUDES */

import QtQuick 2.12
import DapQmlStyle 1.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import StyleDebugTree 1.0
import TextEditContextMenu 1.0
import Brand 1.0
import Scaling 1.0
import PageCtl 1.0
import com.DapQmlWidgets 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Bug Report Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using three separate Item's which only
 * visible if desired mode is active.
 *
 * Button presses also change mode, so it is not necessary to call
 * setmode function from outside of the form.
 *
 * Form contains resizer item for advanced styling technic.
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

    enum Mode
    {
        Write,
        Loading,
        Result
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief current mode
    ///
    /// @see QuiBugReportForm.Mode
    property int mode: 0
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "BugReport"

    /// @brief controler for attach button
    property QtObject internal: QtObject {
        property bool debugMode: false
        property bool attachedImage: false;
        property string textAttach: qsTr("Click here to attach a screenshot") + lang.notifier
        property string textDetach: qsTr("Remove screenshot") + lang.notifier
        property bool showAnAgreement: Brand.isGooglePlayBuild()

        function updateAttachButton() {
            attach.text = (!attachedImage) ? textAttach : textDetach;
        }

        function attachClicked() {
            var empty      = new String;
            if (!attachedImage)
                /* start a file selection dialog */
                root.sigShowAttachImageDialog();
            else
                /* detach image */
                root.sigDetachImage();
        }

        onAttachedImageChanged: updateAttachButton()
        onTextAttachChanged: updateAttachButton()
        onTextDetachChanged: updateAttachButton()
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief button send clicked (report input)
    signal sigSend()
    /// @brief button share clicked (report input)
    signal sigShare()
    /// @brief button cancel clicked (report sending)
    signal sigCancel()
    /// @brief button back clicked (report success\\error)
    signal sigResultBack()
    /// @brief screenshot is selected
    signal sigImageSelected(string a_filename);
    /// @brief button attachImage clicked
    signal sigShowAttachImageDialog();
    /// @brief button detachImage clicked
    signal sigDetachImage();
    /// @brief user checked the agreement
    signal sigAgreementChecked();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief set form mode
    ///
    /// Three modes available:
    /// | name          | description |
    /// | ------------- | ----------- |
    /// | QuiBugReportForm.Mode.Write | report input screen |
    /// | QuiBugReportForm.Mode.Loading | report is sending (spinner) |
    /// | QuiBugReportForm.Mode.Result | report success\\error |
    function setmode(a_mode) {
        mode    = a_mode;
        switch(a_mode) {
        case QuiBugReportForm.Mode.Write:
        case QuiBugReportForm.Mode.Loading:
        case QuiBugReportForm.Mode.Result:
        }
    }

    /// @brief set report sending result text
    function setResultText(a_text) {
        bugrepResult.text   = a_text;
    }

    function showAttachImageDialog(a)
    {
        fileDialog.open();
    }

    /// @brief change attach button state on image selected
    function attachedImage() {
        internal.attachedImage  = true;
    }

    /// @brief change attach button state on report sended
    function detachedImage() {
        internal.attachedImage  = false;
    }

    /// @brief change attach button state on report sended
    function finishedReport() {
        detachedImage();
    }

    /// @brief show agreement notification
    function setAgreementCheck(a_value) {
        root.internal.showAnAgreement   = a_value;
    }

    function updateSendButtonState() {
        let data    = bugRepInputField.text;
        let simple  = true;
        for (var i = 0; i < data.length; i++)
        {
            if (data.charAt(i) === ' ')
                continue;
            simple = false;
            break;
        }

        btnSend.enabled = !simple;
    }

    /// @}
    /****************************************//**
     * @name COMPONENTS
     ********************************************/
    /// @{

    Component {
        id: inputLegacy

        BorderImage {
            id: inputContactBorder
            property string scaledPixmap

            DapQmlStyle { qss: "bugrep-input-contact bugrep-bg-75"; item: inputContactBorder }

            border { left: 25; top: 25; right: 25; bottom: 25 }
            horizontalTileMode: BorderImage.Stretch
            verticalTileMode: BorderImage.Stretch
            source: scaledPixmap

            DapQmlLineEdit {
                id: contactInputField
                objectName: "contactInputField"
                anchors.centerIn: parent
                mainQss: "bugrep-input-textarea"
                iconQss: "null-size null-pos"
                placeholderQss: "bugrep-input-placeholder"
                qss: "bugrep-input-contact-content"

                placeHolderText: qsTr("Email/ Telegram/Phone number to contact") + lang.notifier
                mainText: ""
            }
        }
    }

    Component {
        id: inputModern

        DapQmlRectangle {
            qss: "bugrep-input-contact"

            DapQmlLineEdit {
                id: contactInputField
                objectName: "contactInputField"
                anchors.centerIn: parent
                mainQss: "bugrep-input-textarea bugrep-input-textarea-contact"
                iconQss: "null-size null-pos"
                placeholderQss: "bugrep-input-placeholder"
                qss: "bugrep-input-contact-content"

                placeHolderText: qsTr("Email/ Telegram/Phone number to contact") + lang.notifier
                mainText: ""
            }
        }
    }

    Component {
        id: reportBackgroundLegacy

        BorderImage {
            id: reportBackgroundLegacyBorderImage
            border { left: 64; top: 64; right: 64; bottom: 64 }
            horizontalTileMode: BorderImage.Stretch
            verticalTileMode: BorderImage.Stretch
            source: inputFiller.scaledPixmap
            DapQmlStyle { qss: "bugrep-input"; item: reportBackgroundLegacyBorderImage }
        }
    }

    Component {
        id: reportBackgroundModern

        DapQmlRectangle {
            qss: "bugrep-input-bg"
        }
    }

    /// @}

    /****************************************//**
     * Content
     ********************************************/

    /* FILE DIALOG */

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile

        property string titleName:      qsTr("Open screenshot") + lang.notifier;
        property string filterImages:   qsTr("Images (*.png *.jpg, *.jpeg *.bmp *.PNG *.JPG, *.JPEG *.BMP)") + lang.notifier;
        property string filterAllFiles: qsTr("All files (*.*)") + lang.notifier;

        onAccepted: {
            var result      = new String(currentFile);
            root.sigImageSelected(result.slice(7));
            fileDialog.close()
        }

        function updateNameFilters() {
            title       = titleName;
            nameFilters = [ filterImages, filterAllFiles ];
        }

        Component.onCompleted: updateNameFilters()
        onFilterImagesChanged: updateNameFilters()
        onFilterAllFilesChanged: updateNameFilters()
        onTitleNameChanged: updateNameFilters()
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        qss: "bugrep-input-content"
    }

    DapQmlRectangle {
        id: contactResizer
        qss: "bugrep-contact-input-content"
    }

    /****************************************//**
     * Overlay
     ********************************************/

    FastBlur {
        anchors.fill: content
        source: content
        radius: 40
        cached: true
        z: 50
        visible: root.internal.showAnAgreement
    }

    /* popup dialog darkness */
    Rectangle {
        width: window.width
        height: window.height
        z: 60
        color: "#80000000"
        visible: root.internal.showAnAgreement
    }

    /****************************************//**
     * Agreement
     ********************************************/

    DapQmlRectangle {
        id: agreementPos
        qss: "bugrep-agreement-pos"
    }

    DapQmlRectangle {
        id: agreementTextPos
        qss: "bugrep-agreement-text-pos"
    }

    DapQmlRectangle {
        id: agreementSize
        qss: "bugrep-agreement-size"
    }

    DapQmlRectangle {
        id: agreementFrame
        y: root.height
        z: 70
        visible: y !== 0
        qss: "bugrep-agreement-frame"

        Behavior on y { PropertyAnimation { duration: agreementFrame.y !==0 ? 250 : 0 } }

        function updatePosition() {
            agreementFrame.y   = (root.internal.showAnAgreement)
                   ? agreementPos.y
                   : root.height;
        }

        /* animation timer */

        Timer {
            interval: 350; running: true; repeat: false
            onTriggered: {
//                console.log(`showAnAgreement:${root.internal.showAnAgreement},agreementPos.y:${agreementPos.y},root.height:${root.height}`);
                agreementFrame.updatePosition();
//                console.log(`agreementFrame.y:${agreementFrame.y}`);
            }
        }

        Component.onCompleted:
            StyleDebugTree.describe (
                       "agreementFrame",
                        ["x", "y", "width", "height"],
                       this);

        /* title */

        DapQmlLabel {
            text: "Rules title"
            qss: "bugrep-agreement-title"
        }

        /* close button */

        DapQmlPushButton {
            qss: "bugrep-agreement-title-close-btn"
            onClicked: {
                PageCtl.slotBackwardAuto();
                //root.internal.showAnAgreement = false;
                //agreementFrame.updatePosition();
            }
        }

        /* agreement text */

        Flickable {
            x: (root.width - agreementSize.width) / 2
            y: agreementTextPos.y
            clip: true
            width: agreementSize.width
            height: agreementSize.height
            contentWidth: agreementText.width
            contentHeight: agreementText.height

//            Component.onCompleted:
//                StyleDebugTree.describe (
//                           "Flickable",
//                            ["x", "y", "width", "height", "contentWidth", "contentHeight"],
//                           this);

            DapQmlLabel {
                id: agreementText
                objectName: "agreementText"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignTop
                lineHeight: 1.25
                disableClicking: true
                qss: "bugrep-agreement-text"
//                Component.onCompleted:
//                    StyleDebugTree.describe (
//                               "agreementText",
//                                ["x", "y", "width", "height"],
//                               this);
            }
        }

        /* line */

        DapQmlRectangle {
            width: root.width
            height: 1
            qss: "c-grey bugrep-agreement-line-pos"
        }

        /* button */

        DapQmlPushButton {
            x: (parent.width - width) / 2
            text: "I AGREE"
            qss: "bugrep-agreement-button push-button"
            onClicked: {
                root.internal.showAnAgreement = false;
                agreementFrame.updatePosition();
                root.sigAgreementChecked();
            }
        }
    }

    /****************************************//**
     * Content
     ********************************************/

    Item {
        id: content
        anchors.fill: parent
        visible: !root.internal.showAnAgreement

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Send bug report") + lang.notifier
            qss: "dialog-title"
        }

        /****************************************//**
         * Form Debug
         ********************************************/

        RowLayout {
            visible: root.internal.debugMode
            spacing: 8

            DapQmlLabel {
                width: contentWidth
                height: contentHeight
                text: "Mode Debug: "
            }

            DapQmlLabel {
                width: contentWidth
                height: contentHeight
                text: "Write"
                onClicked: root.setmode (QuiBugReportForm.Mode.Write)
            }

            DapQmlLabel {
                width: contentWidth
                height: contentHeight
                text: "Loading"
                onClicked: root.setmode (QuiBugReportForm.Mode.Loading)
            }

            DapQmlLabel {
                width: contentWidth
                height: contentHeight
                text: "Result"
                onClicked: root.setmode (QuiBugReportForm.Mode.Result)
            }
        }

        /****************************************//**
         * Mode Input Item
         ********************************************/

        Item {
            anchors.fill: parent
            visible: root.mode == 0

            /* contact input */

            Loader {
                anchors.fill: parent
                sourceComponent: Brand.isOldStyle() ? inputLegacy : inputModern
            }

            /* corner images */

            Loader {
                sourceComponent: Brand.isOldStyle() ? reportBackgroundLegacy : reportBackgroundModern
            }

            /* text input */
            Item {
                id: input
                z: 1
                property real yy: (input.height - resizer.height) / 2
                DapQmlStyle { qss: "bugrep-input"; item: input }

                /* corner images rectangle */
                DapQmlRectangle {
                    id: inputFiller
                    visible: false
                    anchors.fill: input
                    color: "transparent"
                    borderWidth: 2
                    borderColor: "EEEEEE"
                    qss: "bugrep-bg"
                    property string scaledPixmap: ""
                }

                /* placeholder */
                DapQmlLabel {
                    id: placeholder
                    x: (input.width - width) / 2
                    y: input.yy
                    z: 2
                    width: resizer.width
                    height: resizer.height
                    horizontalAlign: Text.AlignLeft
                    verticalAlign: Text.AlignTop
                    text: qsTr("Please describe the details of problem you faced. What actions did you take and what happened.") + lang.notifier
                    qss: "bugrep-input-placeholder"
                    wrapMode: TextEdit.Wrap
                    visible: bugRepInputField.text.length == 0 && ((Scaling.isAndroid()) ? !bugRepInputField.activeFocus : true)

    //                    Component.onCompleted: StyleDebugTree.describe (
    //                       "placeholder",
    //                        ["x", "y", "width", "height"],
    //                       this);
                }

//                Text {
//                    id: teststst

//                    function update() {
//                        text = `size ${bugRepInput.width}x${bugRepInput.height} : ${bugRepInput.contentHeight}`
//                    }

//                    Component.onCompleted: update()
//                }

                /* input scrollarea */
                Flickable {
                    id: bugRepInput
                    x: (input.width - resizer.width) / 2
                    y: input.yy
                    z: 3
                    clip: true
                    contentWidth: width
                    contentHeight: calcContentHeight()

//                    onContentHeightChanged: teststst.update()
//                    onWidthChanged: teststst.update()
//                    onHeightChanged: teststst.update()

                    DapQmlStyle { item: bugRepInput; qss: "bugrep-input-content"; }

    //                Component.onCompleted: StyleDebugTree.describe (
    //                   "Flickable",
    //                    ["x", "y", "width", "height"],
    //                   this);

                    function ensureVisible(r) {
                        if (contentX >= r.x)
                            contentX = r.x;
                        else if (contentX+width <= r.x+r.width)
                            contentX = r.x+r.width-width;
                        if (contentY >= r.y)
                            contentY = r.y;
                        else if (contentY+height <= r.y+r.height)
                            contentY = r.y+r.height-height;
                    }

                    function calcContentHeight() {
                        if (bugRepInputField.paintedHeight < inputFiller.height)
                            return inputFiller.height;
                        else
                            return bugRepInputField.paintedHeight;
                    }

                    /* input */
                    TextEdit {
                        id: bugRepInputField
                        z: 4
                        objectName: "bugRepInputField"
                        anchors.fill: parent
                        anchors.margins: 1
                        wrapMode: TextEdit.Wrap
                        persistentSelection: true
                        selectByMouse: true
                        //clip: true
                        font.family: Brand.fontName()
                        font.pixelSize: fontSize
                        font.weight: fontWeight
                        inputMethodHints: Qt.ImhSensitiveData

                        property int fontSize: 16
                        property int fontWeight: Font.Normal
                        property int maximumLength: 200
                        property string previousText: text

                        DapQmlStyle { item: bugRepInputField; qss: "bugrep-input-textarea"; }

                        TextEditContextMenu {
                            id: ctxMenu
                            Component.onCompleted: {
                                setSerialInpoutMode(false);
                                setTextEditWidget(bugRepInputField);
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            cursorShape: Qt.IBeamCursor
                            onClicked: {
                                if (Scaling.isDesktop())
                                    if (mouse.button === Qt.RightButton)
                                        contextMenu.open()
                            }
                            onPressAndHold: {
                                if (Scaling.isDesktop())
                                    if (mouse.source === Qt.MouseEventNotSynthesized)
                                        contextMenu.open()
                            }
                            DapQmlMenu {
                                id: contextMenu
                                shortcuts: [
                                    "Ctrl+X",
                                    "Ctrl+C",
                                    "Ctrl+V",
                                    ""
                                ]
                                Action {
                                    text: qsTr("Cut") + lang.notifier
                                    shortcut: "Ctrl+X"
                                    onTriggered: ctxMenu.execCut();
                                    Component.onCompleted: console.log(shortcut)
                                }
                                Action {
                                    text: qsTr("Copy") + lang.notifier
                                    shortcut: "Ctrl+C"
                                    onTriggered: ctxMenu.execCopy();
                                }
                                Action {
                                    text: qsTr("Paste") + lang.notifier
                                    shortcut: "Ctrl+V"
                                    onTriggered: ctxMenu.execPaste();
                                }
                                Action {
                                    text: qsTr("Delete") + lang.notifier
                                    //shortcut: "Delete"
                                    onTriggered: ctxMenu.execDelete();
                                }
                            }
                        }

                        onCursorRectangleChanged: bugRepInput.ensureVisible(cursorRectangle)

                        onTextChanged: {
                            if (text.length > maximumLength) {
                                var cursor = cursorPosition;
                                text = previousText;
                                if (cursor > text.length) {
                                    cursorPosition = text.length;
                                } else {
                                    cursorPosition = cursor-1;
                                }
                            }
                            previousText = text

                            letterAmount.text = text.length + "/200"
                            root.updateSendButtonState();
                        }
                    }
                }
            }

            /* letter counter */
            DapQmlLabel {
                id: letterAmount
                qss: "bugrep-letter-counter"
                text: "0/200"
                color: "#A4A3C0"
                horizontalAlign: Text.AlignRight
            }

            /* attach */
            DapQmlLabel {
                id: attach
                qss: "bugrep-attach-btn c-brand"

                MouseArea {
                    id: mouseArea
                    anchors.fill: attach
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.internal.attachClicked()
                    Component.onCompleted: root.internal.updateAttachButton() // update label
                }
            }

            /* send button */
            DapQmlPushButton {
                id: btnSend
                qss: "bugrep-send-btn push-button"
                text: qsTr("SEND REPORT") + lang.notifier
                onClicked: { root.mode = 1; root.sigSend(); }
                enabled: false // bugRepInputField.length >= 1
                opacity: 0.4 + 0.6 * enabled
            }

//            /* share button */
//            DapQmlPushButton {
//                qss: "bugrep-share-btn push-button"
//                text: qsTr("SHARE REPORT") + lang.notifier
//                onClicked: { root.sigShare(); }
//                enabled: true
//                opacity: 0.4 + 0.6 * enabled
//            }
        }

        /****************************************//**
         * Mode Sending Item
         ********************************************/

        Item {
            anchors.fill: parent
            visible: root.mode == 1

            /* info */
            DapQmlLabel {
                qss: "bugrep-sending c-label"
                text: qsTr("Sending...") + lang.notifier
            }

            /* animated spinner */
//            AnimatedImage {
//                id: animation
//                source: "qrc:/dapqml-forms-asset/Spinner.gif"
//                DapQmlStyle { qss: "bugrep-animation"; item: animation }
//            }
            Item {
                anchors.fill: parent
                anchors.bottomMargin: arcAnim.height * 0.75

                DapQmlArcAnimation {
                    id: arcAnim
                    anchors.centerIn: parent
                    qss: "bugrep-animation c-brand"
                }
            }

            /* cancel */
            DapQmlPushButton {
                qss: "bugrep-cancel-btn push-button"
                text: qsTr("CANCEL") + lang.notifier
                onClicked: { root.mode = 0; root.sigCancel(); }
            }
        }

        /****************************************//**
         * Mode Status Item
         ********************************************/

        Item {
            anchors.fill: parent
            visible: root.mode == 2

            /* status text */
            DapQmlLabel {
                id: bugrepResult
                qss: "bugrep-status"
                wrapMode: Text.WordWrap
                text: "Bug report # 368647 sent successfully"
            }

            /* back */
            DapQmlPushButton {
                qss: "bugrep-send-btn push-button"
                text: qsTr("BACK") + lang.notifier
                onClicked: { root.mode = 0; root.sigResultBack(); }
            }
        }
    }
}

/*-----------------------------------------*/
