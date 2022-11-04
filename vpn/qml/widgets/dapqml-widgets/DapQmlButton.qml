/* INCLUDES */

import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1
import DapQmlStyle 1.0
import DapQmlSerialKeyInput 1.0
import TextEditContextMenu 1.0
import Scaling 1.0

/****************************************//**
 * @brief Dap QML Button Widget
 * @ingroup groupDapQmlWidgets
 *
 * @note Doesn't have public functions
 *
 * ### Structure
 *
 * Widget is made by two (or more) labels
 * stored inside item holder that can
 * take shape of frame, can have separator
 * at teh bottom and link icon at the right
 *
 * ### Modes
 * | name | description |
 * | ---- | ----------- |
 * | TopMainBottomSub | top:main, bottom:sub |
 * | TopSubBottomMain | top:sub,  bottom:main |
 * | LeftTopMainBottomSub | left:left, top:main, bottom:sub |
 * | IconMainSub | icon, main, sub |
 * | EditTopMainBottomSub | top:edit, bottom:sub |
 * | IconMainSubIcon | icon, main, sub, icon |
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Rectangle {
    id: root

    /* user no background */
    color: "transparent"

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum Style
    {
        TopMainBottomSub,     ///< top:main, bottom:sub
        TopSubBottomMain,     ///< top:sub,  bottom:main
        LeftTopMainBottomSub, ///< left:left, top:main, bottom:sub
        IconMainSub,          ///< icon, main, sub
        EditTopMainBottomSub, ///< top:edit, bottom:sub
        IconMainSubIcon,      ///< icon, main, sub, icon
        EditLine              ///< icon, main, icon
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string mainText: "Main text"
    property string subText: "Sub text"
    property string leftText: "Left text"
    property string placeHolderText: ""
    property string inputMask
    property string qss: ""
    property string mainQss: ""
    property string leftQss: ""
    property string subQss: ""
    property string placeHolderQss: ""
    property string icon: ""
    property string iconRight: ""
    property int iconSize: 34
    property int iconRightSize: 34
    property int iconLineEditSize: 20
    property int buttonStyle: DapQmlButton.Style.TopMainBottomSub
    property int editEchoMode: TextInput.Normal
    property bool separator: false
    property bool link: false
    property bool frame: false
    property bool checkbox: false
    property bool checked: false

    property var labelMain
    property var labelSub
    property var labelLeft
    property var labelIcon
    property var labelIconRight

    property int mainTextOffsetX: 0

    DapQmlStyle { id: style; qss: root.qss; item: root }

    MouseArea {
        anchors.fill: root;
        onClicked: root.clicked()
    }

    onClicked: {
        if (root.checkbox)
        {
            root.checked            = !root.checked;
            checkboxItem.checked    = root.checked;
        }
    }


    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief button clicked
    signal clicked();

    /// @brief right small button clicked
    signal rightClicked();

    /// @brief text field edited
    signal textEdited();

    /// @brief text field content changed
    signal textChanged();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function setEnable(value) {
        root.enabled = value;
        root.opacity = (value)? 1.0: 0.5;
    }

    /* store references */
    Component.onCompleted: {
        if(root.buttonStyle === DapQmlButton.Style.LeftTopMainBottomSub)
        {
            root.labelMain  = ltmbsMain;
            root.labelSub   = ltmbsSub;
            root.labelLeft  = ltmbsLeft;
        }
        else if(root.buttonStyle === DapQmlButton.Style.TopMainBottomSub)
        {
            root.labelMain  = tmbsMain;
            root.labelSub   = tmbsSub;
        }
        else if(root.buttonStyle === DapQmlButton.Style.TopSubBottomMain)
        {
            root.labelMain  = tsbmMain;
            root.labelSub   = tsbmSub;
        }
        else if(root.buttonStyle === DapQmlButton.Style.IconMainSub)
        {
            root.labelMain  = imsMain;
            root.labelSub   = imsSub;
            root.labelIcon  = imsIcon;
        }
        else if(root.buttonStyle === DapQmlButton.Style.EditTopMainBottomSub)
        {
            root.labelMain  = etmbsMain;
            root.labelSub   = etmbsSub;
        }
        else if(root.buttonStyle === DapQmlButton.Style.IconMainSubIcon)
        {
            root.labelMain      = imsiMain;
            root.labelSub       = imsiSub;
            root.labelIcon      = imsiIcon;
            root.labelIconRight = imsiRightIcon;
        }
        else if(root.buttonStyle === DapQmlButton.Style.EditLine)
        {
            root.labelMain          = lineEditField;
            root.labelIcon          = lineEditIcon;
//            root.labelIconRight     = lineEditlIconRight;
        }
    }

    function _magickSpacer() {
        return (0.074 * height);
    }

    function _magickWidth() {
        return root.width;
    }

    function _magickHeight() {
        return root.height / 2;
    }

    function _magickSubPos() {
        return _magickHeight() + _magickSpacer();
    }

    function setFocus() {
        etmbsMain.forceActiveFocus();
        console.log("setFocus")
    }

    /// @}
    /****************************************//**
     * Link image
     ********************************************/

    DapQmlImage {
        id: linkImage
        x: root.width - (root.frame ? (width * 2.4) : (width * 2))
        y: (root.height - height) / 2
        z: 1
        width: root.height / 5
        height: root.height / 5
        visible: root.link && root.buttonStyle !== DapQmlButton.Style.IconMainSub
        scaledPixmap: "qrc:/light/ic_arrow-right.png"
    }

    /****************************************//**
     * Frame image
     ********************************************/

    DapQmlLabel {
        anchors.fill: parent
        z: -1
        visible: root.frame
        qss: "btn-frame"
    }

    /****************************************//**
     * Checkbox
     ********************************************/

    DapQmlCheckbox {
        id: checkboxItem
        x: root.width - width + (width * 0.2)
        y: (root.height - height) / 2 - (height * 0.05)
        z: 1
        width: root.height
        height: root.height
        iconSize: root.height
        visible: root.checkbox
        checked: root.checked
        qss: "btn-checkbox"
//        onToggled: {
//            //root.checked    = a_state;
//            root.clicked();
//        }
        onClicked: root.clicked();
    }

    /****************************************//**
     *  Top   - Item
     * Bottom - Separator
     ********************************************/

    GridLayout {
        anchors.fill: parent;
        columns: 1

        /* LeftTopMainBottomSub */

        /* top item */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            visible: (root.buttonStyle === DapQmlButton.Style.LeftTopMainBottomSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = ltmbsMain;
                    root.labelSub   = ltmbsSub;
                    root.labelLeft  = ltmbsLeft;
                }
            }

            /* left big label */
            DapQmlLabel {
                id: ltmbsLeft
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignVCenter
                text: root.leftText
                qss: root.leftQss
                onClicked: root.clicked();
            }

            /* right side two labels */
            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 1

                /* main text */
                DapQmlLabel {
                    id: ltmbsMain
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: text.length > 0

                    horizontalAlign: Text.AlignLeft
                    verticalAlign: root.subText.length > 0 ? Text.AlignBottom : Text.AlignVCenter
                    text: root.mainText
                    qss: root.mainQss
                    onClicked: root.clicked();
                }

                /* sub text */
                DapQmlLabel {
                    id: ltmbsSub
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: text.length > 0

                    horizontalAlign: Text.AlignLeft
                    verticalAlign: root.subText.length > 0 ? Text.AlignTop : Text.AlignVCenter
                    text: root.subText
                    qss: root.subQss
                    onClicked: root.clicked();
                }

            }
        }

        /* TopMainBottomSub */
        /* Two items by vertical */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.TopMainBottomSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = tmbsMain;
                    root.labelSub   = tmbsSub;
                }
            }

            /* main text */
            DapQmlLabel {
                id: tmbsMain
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignBottom
                text: root.mainText
                qss: root.mainQss
                onClicked: root.clicked();
            }

            /* sub text */
            DapQmlLabel {
                id: tmbsSub
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight()
                y: _magickSubPos()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.subText
                qss: root.subQss
                onClicked: root.clicked();
            }

        }

        /* TopSubBottomMain */
        /* Two items by vertical */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.TopSubBottomMain)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = tsbmMain;
                    root.labelSub   = tsbmSub;
                }
            }

            /* sub text */
            DapQmlLabel {
                id: tsbmSub
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignBottom
                text: root.subText
                qss: root.subQss
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: tsbmMain
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight() - _magickSpacer() / 2
                y: _magickHeight()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.mainText
                qss: root.mainQss
                onClicked: root.clicked();
            }

        }

        /* EditTopMainBottomSub */
        /* Two items by vertical with editable text field */
        Item {
            id: etmbs
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.EditTopMainBottomSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = etmbsMain;
                    root.labelSub   = etmbsSub;
                }
            }

            /* main text */
            TextField {
                id: etmbsMain
                y: _magickSpacer()
                width: _magickWidth()
                height: _magickHeight() - _magickSpacer()

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                echoMode: root.editEchoMode
                text: root.mainText
                //qss: root.mainQss
                inputMask: root.inputMask
                // android virtual keyboard
                inputMethodHints: Qt.ImhSensitiveData

                DapQmlLabel {
                    anchors.fill: etmbsMain
                    enabled: false
                    text: (root.mainText.length === 0 && etmbsMain.activeFocus === false) ? root.placeHolderText : ""
                    qss: root.placeHolderQss
                }

                DapQmlSerialKeyInput {
                    //anchors.fill: parent
                    id: filter
                    objectName: "serialInputFilter"
                }

                TextEditContextMenu {
                    id: ctxMenu
                    Component.onCompleted: setTextEditWidget(etmbsMain)
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
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
                            text: "Cut"
                            shortcut: "Ctrl+X"
                            onTriggered: ctxMenu.execCut();
                        }
                        Action {
                            text: "Copy"
                            shortcut: "Ctrl+C"
                            onTriggered: ctxMenu.execCopy();
                        }
                        Action {
                            text: "Paste"
                            shortcut: "Ctrl+V"
                            onTriggered: ctxMenu.execPaste();
                        }
                        Action {
                            text: "Delete"
                            //shortcut: "Delete"
                            onTriggered: ctxMenu.execDelete();
                        }
                    }
                }

                Component.onCompleted: {
                    if(root.buttonStyle === DapQmlButton.Style.EditTopMainBottomSub)
                        filter.setup(this);
                }

                /* vars */
                property string fontFamiliy: "Lato"
                property int fontSize: 12
                property int fontWeight: Font.Normal

                /* style */
                DapQmlStyle { qss: root.mainQss; item: etmbsMain }

                /* background */
                background: DapQmlRectangle {
                    qss: "login-serialkey-input"
                }

                /* font config */
                font {
                    family: etmbsMain.fontFamiliy
                    pixelSize: etmbsMain.fontSize
                    weight: etmbsMain.fontWeight
                }

                /* signals */
                onTextEdited: { root.mainText = text; root.textEdited(); }
                onTextChanged: { root.mainText = text; root.textChanged(); }
            }

            /* sub text */
            DapQmlLabel {
                id: etmbsSub
                width: _magickWidth()
                height: _magickHeight()
                y: _magickSubPos()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.subText
                qss: root.subQss
                onClicked: root.clicked();
            }

        }

        /* IconMainSub */
        /* Three items by horiontal */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rightMargin: root.link ? root.iconRightSize : 0
            visible: (root.buttonStyle === DapQmlButton.Style.IconMainSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = imsMain;
                    root.labelSub   = imsSub;
                    root.labelIcon  = imsIcon;
                }
            }

            /* icon */
            DapQmlLabel {
                id: imsIcon
                y: (parent.height - height) / 2
                width: root.iconSize
                height: root.iconSize

                qss: root.icon
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: imsMain
                x: (imsIcon.width > 0) ? imsIcon.width + imsIcon.width / 4
                                       : root.mainTextOffsetX
                width: contentWidth
                height: parent.height

                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignVCenter
                text: root.mainText
                qss: root.mainQss
                clip: false
                onClicked: root.clicked();
            }

            /* sub text */
            DapQmlLabel {
                id: imsSub
                x: imsMain.x + imsMain.width + imsIcon.width / 2
                width: parent.width - imsIcon.width - imsMain.width - linkImage1.minus
                height: parent.height

                horizontalAlign: Text.AlignRight
                verticalAlign: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
                clip: false
                visible: text.length > 0
                onClicked: root.clicked();
            }

            DapQmlImage {
                property real minus: (root.frame ? (width * 2.4) : (width * 2))

                id: linkImage1
                x: root.width - minus
                y: imsMain.y + imsMain.height / 2 - width / 2
                z: 1
                width: root.height / 4
                height: root.height / 4
                visible: root.link
                scaledPixmap: "qrc:/light/ic_arrow-right.png"

//                Rectangle {
//                    color: "blue"
//                    anchors.fill: parent
//                    opacity: 0.3
//                }
            }
        }

        /* IconMainSubIcon */
        /* Three items by horiontal */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.IconMainSubIcon)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = imsiMain;
                    root.labelSub   = imsiSub;
                    root.labelIcon  = imsiIcon;
                }
            }

            /* icon */
            DapQmlLabel {
                id: imsiIcon
                y: (parent.height - height) / 2
                width: root.iconSize
                height: root.iconSize

                qss: root.icon
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: imsiMain
                x: (imsiIcon.width > 0) ? imsiIcon.width + imsiIcon.width / 4
                                        : root.mainTextOffsetX
                width: contentWidth
                height: parent.height

                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignVCenter
                text: root.mainText
                qss: root.mainQss
                clip: false
                onClicked: root.clicked();

            }

            /* sub text */
            DapQmlLabel {
                id: imsiSub
                x: imsiMain.x + imsiMain.width + imsiIcon.width / 2
                width: parent.width - imsiIcon.width - imsiMain.width - imsiRightIcon.width
                height: parent.height

                horizontalAlign: Text.AlignRight
                verticalAlign: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
                clip: false
                visible: text.length > 0
                onClicked: root.clicked();

            }

            /* icon */
            DapQmlLabel {
                id: imsiRightIcon
                x: parent.width - width - imsiIcon.width / 4
                y: (parent.height - height) / 2
                width: root.iconRightSize
                height: root.iconRightSize

                qss: root.iconRight
                onClicked: root.rightClicked();

            }
        }

        /* Line edit */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.EditLine)
            function workHeight() { return height - 4; }

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain          = lineEditField;
                    root.labelIcon          = lineEditIcon;
                }
            }

            DapQmlLabel {
                id: lineEditIcon
                x: 3
                y: lineEditField.y
                width: root.iconLineEditSize
                height: root.iconLineEditSize

                qss: root.iconRight
                scaledPixmap: "qrc:/light/ic_close_hover.png"
                onClicked: root.rightClicked();
            }

            /* main text */
            TextField {
                id: lineEditField
                y: (root.height - height) / 2
                x: lineEditIcon.width + 3
                width: parent.width - lineEditIcon.width
                height: parent.height

                //                Rectangle {
                //                    color: "gray"
                //                    anchors.fill: parent
                //                }

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignBottom
                text: root.mainText
                //qss: root.mainQss
                inputMask: root.inputMask
                // android virtual keyboard
                inputMethodHints: Qt.ImhSensitiveData

                TextEditContextMenu {
                    id: lineEditMenu
                    Component.onCompleted: setTextEditWidget(lineEditField)
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
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
                    Menu {
                        id: lineEditContextMenu
                        MenuItem {
                            text: "Cut"
                            shortcut: "Ctrl+X"
                            onTriggered: lineEditMenu.execCut();
                        }
                        MenuItem {
                            text: "Copy"
                            shortcut: "Ctrl+C"
                            onTriggered: lineEditMenu.execCopy();
                        }
                        MenuItem {
                            text: "Paste"
                            shortcut: "Ctrl+V"
                            onTriggered: lineEditMenu.execPaste();
                        }
                        MenuItem {
                            text: "Delete"
                            //shortcut: "Delete"
                            onTriggered: lineEditMenu.execDelete();
                        }
                    }
                }

                Component.onCompleted: {
                    if(root.buttonStyle === DapQmlButton.Style.EditTopMainBottomSub)
                        filter.setup(this);
                }

                /* vars */
                property string fontFamiliy: "Lato"
                property int fontSize: 14
                property int fontWeight: Font.Normal

                /* style */
                DapQmlStyle { qss: root.mainQss; item: lineEditField }

                /* background */
                background: DapQmlRectangle {
                    qss: "ch-country-filter"
                }

                /* font config */
                font {
                    family: lineEditField.fontFamiliy
                    pixelSize: lineEditField.fontSize
                    weight: lineEditField.fontWeight
                }

                /* signals */
                onTextEdited: { root.mainText = text; root.textEdited(); }
                onTextChanged: { root.mainText = text; root.textChanged(); }
            }
        }

        /* bottom item */
        DapQmlSeparator {
            Layout.fillWidth: true
            width: root.width
            visible: root.separator
        }
    }

}

/*-----------------------------------------*/
