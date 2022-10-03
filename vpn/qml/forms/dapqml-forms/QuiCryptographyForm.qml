/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Cryptography Form
 * @ingroup groupDapQmlForms
 *
 * @date 17.07.22
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
    property string formName: "Cryptography"

    enum Mode
    {
        M_LIST,
        M_INFO,
        M_GENERATE
    }

    /// @brief controls form mode by aplying visibility rules
    property QtObject modeCtl: QtObject {
        property int mode: QuiCryptographyForm.Mode.M_LIST

        function setMode(a_newMode) {
            /* store new mode */
            mode        = a_newMode;

            /* apply visibility */
            let list = (mode === QuiCryptographyForm.Mode.M_LIST);
            let info = (mode === QuiCryptographyForm.Mode.M_INFO);
            let gen  = (mode === QuiCryptographyForm.Mode.M_GENERATE);

            /* display only required page */
            pageList.visible        = list;
            pageInfo.visible        = info;
            pageGenerate.visible    = gen;

            /* show cancel and accept buttons when mode is not list */
            acceptBtn.visible       = info | gen;
            cancelBtn.visible       = info | gen;
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
    function setMode(a_newMode) {
        modeCtl.setMode(a_newMode);
    }

    function _pos(a_index) {
        return title.y + (title.height * 2.4) + (resizerItem.height * a_index);
    }

    Component.onCompleted: setMode (QuiCryptographyForm.Mode.M_LIST)

    /// @}
    /****************************************//**
     *
     * Debug
     *
     ********************************************/

//    Button {
//        x: 4; y: 4; z: 10; width: 64; height: 28; text: "list"
//        onClicked: root.setMode (QuiCryptographyForm.Mode.M_LIST)
//    }

//    Button {
//        x: 4+72; y: 4; z: 10; width: 64; height: 28; text: "info"
//        onClicked: root.setMode (QuiCryptographyForm.Mode.M_INFO)
//    }

//    Button {
//        x: 4+144; y: 4; z: 10; width: 64; height: 28; text: "gen"
//        onClicked: root.setMode (QuiCryptographyForm.Mode.M_GENERATE)
//    }

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
        qss: "crypto-resizer-field"
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
            text: qsTr("Cryptohraphy") + lang.notifier
            qss: "dialog-title"

            /* settings button */
            DapQmlPushButton {
                id: settingsBtn
                qss: "crypto-settings-btn"

                x: parent.width - width
                y: (parent.height - height) / 2
                z: 16

                onClicked: settingsBtnMenu.popup();

                DapQmlMenu {
                    id: settingsBtnMenu

                    /* actions */
                    Action { text: "Restart server"; }
                    Action { text: "Generate certificate"; onTriggered: root.setMode (QuiCryptographyForm.Mode.M_GENERATE) }
                    Action { text: "Export public key" }
                }
            }
        }

        /****************************************//**
         * Content
         ********************************************/

        ListView {
            id: cryptoListView
            objectName: "cryptoListView"

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
                qss: "crypto-resizer-item"
            }

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: DapQmlButton {
                id: delegate

                property int myIndex: model.index

                width: cryptoListView.width - moreBtn.width / 2
                height: resizerItem.height
                buttonStyle: DapQmlButton.Style.IconMainSubIcon
                mainText: model.display
                subText: "public key"
                separator: true
                qss: "crypto-item"
                mainQss: "crypto-btn-lbl-main"
                subQss: "crypto-btn-lbl-sub"
                iconSize: 0
                mainTextOffsetX: moreBtn.width / 2

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

                    x: parent.width - width * 1 / 2
                    y: (parent.height - height) / 2 - height / 8
                    z: 16
                    width: resizerItem.fontSize * 1.25
                    height: resizerItem.fontSize * 1.25

                    onClicked: moreBtnMenu.popup();

                    DapQmlMenu {
                        id: moreBtnMenu

                        /* actions */
                        Action { text: "Copy fingerprint" }
                        Action { text: "Info"; onTriggered: root.setMode (QuiCryptographyForm.Mode.M_INFO) }
                    }
                }

                function buttonClicked(a_isButtonSignal) {
                    // if(!a_isButtonSignal)
                    //     clicked();
                    // model.exec (myIndex, this);
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
     * Title Buttons
     ********************************************/

    /* accept button */
    DapQmlPushButton {
        id: acceptBtn
        qss: "crypto-accept-btn"

        x: title.x + title.width - width
        y: title.y + (title.height - height) / 2
        z: 16

        onClicked: root.setMode (QuiCryptographyForm.Mode.M_LIST)
    }

    /* cancel button */
    DapQmlPushButton {
        id: cancelBtn
        qss: "crypto-cancel-btn"

        x: title.x
        y: title.y + (title.height - height) / 2
        z: 16

        onClicked: root.setMode (QuiCryptographyForm.Mode.M_LIST)
    }

    /****************************************//**
     *
     * Info
     *
     ********************************************/

    Item {
        id: pageInfo
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Info") + lang.notifier
            qss: "dialog-title"
        }

        /****************************************//**
         * Info Name
         ********************************************/

        DapQmlInputField {
            id: inputInfoName
            x: (root.width - width) / 2
            y: _pos(0) // title.y + title.height * 2
            z: 2
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Name"
        }

        /****************************************//**
         * Info Type
         ********************************************/

        DapQmlInputField {
            id: inputInfoType
            x: (root.width - width) / 2
            y: _pos(1) // inputTitle.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Type"
        }

        /****************************************//**
         * Info Fingerprint
         ********************************************/

        DapQmlInputField {
            id: inputInfoFingerprint
            x: (root.width - width) / 2
            y: _pos(2) // inputAddress.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Fingerprint"
        }
    } // Info

    /****************************************//**
     *
     * Generate
     *
     ********************************************/

    Item {
        id: pageGenerate
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Generate certificate") + lang.notifier
            qss: "dialog-title"
        }

        /****************************************//**
         * Gen Signature
         ********************************************/

        DapQmlCombobox {
            id: inputGenSignature
            x: (root.width - width) / 2
            y: _pos(0) // title.y + title.height * 2
            z: 2
            width: resizerItem.width
            height: resizeField.height
            title: "Signature Type"

            Action { text: "Tesla" }
            Action { text: "Bliss" }
            Action { text: "Picnic" }
            Action { text: "Crystal-Dylithium" }
        }

        /****************************************//**
         * Gen Title
         ********************************************/

        DapQmlInputField {
            id: inputGenTitle
            x: (root.width - width) / 2
            y: _pos(1) // inputTitle.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Title"
        }

        /****************************************//**
         * Gen Domain
         ********************************************/

        DapQmlInputField {
            id: inputGenDomain
            x: (root.width - width) / 2
            y: _pos(2) // inputAddress.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Domain"
        }

        /****************************************//**
         * Gen Exp Date
         ********************************************/

        DapQmlInputField {
            id: inputGenExpDate
            x: (root.width - width) / 2
            y: _pos(3) // title.y + title.height * 2
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Expiration date"
        }

        /****************************************//**
         * Gen Organization
         ********************************************/

        DapQmlInputField {
            id: inputGenOrg
            x: (root.width - width) / 2
            y: _pos(4) // inputTitle.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Organization"
        }

        /****************************************//**
         * Gen Fullname
         ********************************************/

        DapQmlInputField {
            id: inputGenFullname
            x: (root.width - width) / 2
            y: _pos(5) // inputAddress.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Full name"
        }

        /****************************************//**
         * Gen Email
         ********************************************/

        DapQmlInputField {
            id: inputGenEmail
            x: (root.width - width) / 2
            y: _pos(6) // inputAddress.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Email"
        }
    } // Generate
}
