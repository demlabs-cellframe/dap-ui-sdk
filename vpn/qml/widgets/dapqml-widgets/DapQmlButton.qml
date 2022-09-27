/* INCLUDES */

import QtQuick 2.10
import QtQuick.Controls 2.1
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
        IconMainSubIcon       ///< icon, main, sub, icon
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

    /// @}
    /****************************************//**
     * Link image
     ********************************************/

    DapQmlImage {
        x: root.width - (root.frame ? (width * 2.4) : (width * 2))
        y: (root.height - height) / 2
        z: 1
        width: root.height / 5
        height: root.height / 5
        visible: root.link
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
                    Menu {
                        id: contextMenu
                        MenuItem {
                            text: "Cut"
                            shortcut: "Ctrl+X"
                            onTriggered: ctxMenu.execCut();
                        }
                        MenuItem {
                            text: "Copy"
                            shortcut: "Ctrl+C"
                            onTriggered: ctxMenu.execCopy();
                        }
                        MenuItem {
                            text: "Paste"
                            shortcut: "Ctrl+V"
                            onTriggered: ctxMenu.execPaste();
                        }
                        MenuItem {
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
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
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
                Layout.preferredWidth: root.iconSize
                Layout.preferredHeight: root.iconSize

                qss: root.icon
                width: root.iconSize
                height: root.iconSize
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: imsMain
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

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
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: text.length > 0
                Layout.fillHeight: true

                horizontalAlign: Text.AlignRight
                verticalAlign: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
                clip: false
                visible: text.length > 0
                onClicked: root.clicked();
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
                x: imsiIcon.width + imsiIcon.width / 4
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

        /* bottom item */
        DapQmlSeparator {
            Layout.fillWidth: true
            width: root.width
            visible: root.separator
        }
    }

}

/*-----------------------------------------*/
