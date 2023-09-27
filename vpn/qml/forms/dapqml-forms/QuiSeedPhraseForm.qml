/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQml.Models 2.1
import DapQmlStyle 1.0
import DapQmlLayout 1.0
import InterfaceManager 1.0
import ShelfCtl 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Generate Trial Key Form
 * @ingroup groupDapQmlForms
 *
 * @date 13.07.23
 * @author Mikhail Shilenko
 *******************************************/

DapQmlRectangle {
    id: root
    clip: true
    qss: "seedphrase-frame"

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "SeedPhrase"

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigAccept();
    signal sigCopy();
    signal sigAgreementCheck(bool a_checked);

    Component.onCompleted: {
        ShelfCtl.title  = qsTr("Seed phrase") + lang.notifier
        InterfaceManager.setupForm (root);
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * Sizers
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * Components
     ********************************************/
    /// @{

//    Component {
//        id: compWord

//        DapQmlRectangle {
//            id: wordFrame
//            width: wordLabel.width + (wordLabel.fontSize * 1.14)
//            height: wordLabel.height + (wordLabel.fontSize * 0.714)
//            radius: height
//            borderWidth: 1
//            borderColor: "#5A5A5A"

//            DapQmlLabel {
//                id: wordLabel
//                anchors.centerIn: parent
//                width: contentWidth
//                height: contentHeight
//                qss: "seedphrase-word-label"
//                text: model.name
//            }
//        }
//    }

    Component {
        id: compWord

        //property string text

        DapQmlRectangle {
            id: wordFrame
            width: wordLabel.width + wordLabel.fontSize * 2
            height: wordLabel.height * 2
            radius: height
            borderWidth: 1
            borderColor: wordLabel.color

            DapQmlLabel {
                id: wordLabel
                anchors.centerIn: parent
                width: contentWidth
                height: contentHeight
                qss: "seedphrase-word-label"
                text: wordFrame.parent.text
            }

            onHeightChanged: {
                parent.width    = wordFrame.width;
                parent.height   = wordFrame.height;
            }
        }
    }

    /// @}
    /****************************************//**
     * Description
     ********************************************/

    DapQmlLabel {
        wrapMode: Text.WordWrap
        qss: "seedphrase-description"
        text: qsTr("Save these words in safe place. They will be required to restore your key.") + lang.notifier
    }

    /****************************************//**
     * Words
     ********************************************/

//    GridView {
//        id: wordsGrid
//        model: ListModel {
//            ListElement { name: "word 1" }
//            ListElement { name: "word 2" }
//            ListElement { name: "word 3" }
//            ListElement { name: "word 4" }
//            ListElement { name: "word 5" }
//            ListElement { name: "word 6" }
//            ListElement { name: "word 7" }
//        }
//        delegate: compWord

//        DapQmlStyle { item: wordsGrid; qss: "seedphrase-words-grid" }
//    }

    ListView {}

    DapQmlLayout {
        id: wordsGrid
        spacing: wordsGridSpacing.height

        Loader { sourceComponent: compWord; property string text: "1. word" }
        Loader { sourceComponent: compWord; property string text: "2. word" }
        Loader { sourceComponent: compWord; property string text: "3. word" }
        Loader { sourceComponent: compWord; property string text: "4. word" }
        Loader { sourceComponent: compWord; property string text: "5. word +" }
        Loader { sourceComponent: compWord; property string text: "6. word" }
        Loader { sourceComponent: compWord; property string text: "7. word" }
        Loader { sourceComponent: compWord; property string text: "8. word ++" }
        Loader { sourceComponent: compWord; property string text: "9. word" }
        Loader { sourceComponent: compWord; property string text: "10. word" }
        Loader { sourceComponent: compWord; property string text: "11. long word" }
        Loader { sourceComponent: compWord; property string text: "12. word" }
        Loader { sourceComponent: compWord; property string text: "13. word" }
        Loader { sourceComponent: compWord; property string text: "14. long word" }
        Loader { sourceComponent: compWord; property string text: "15. word" }
        Loader { sourceComponent: compWord; property string text: "16. word" }

        DapQmlStyle { item: wordsGrid; qss: "seedphrase-words-grid" }
        DapQmlDummy { id: wordsGridSpacing; qss: "seedphrase-words-grid-spacing" }
    }

    /****************************************//**
     * Copy button
     ********************************************/

    DapQmlRectangle {
        id: copyBtn
        color: (hovered || pressed)
               ? copyBtnColorActive.color
               : copyBtnColorIdle.color
        qss: "seedphrase-copy-btn push-button"

        property bool hovered: false
        property bool pressed: false

        /* colors */

        DapQmlDummy { id: copyBtnColorIdle;   property string color; qss: "seedphrase-copy-btn-idle" }
        DapQmlDummy { id: copyBtnColorActive; property string color; qss: "seedphrase-coby-btn-active" }

        /* label */

        DapQmlLabel {
            anchors.centerIn: parent
            width: contentWidth
            height: contentHeight
            qss: "seedphrase-copy-btn c-label"
            text: parent.pressed
                  ? qsTr("Copied!") + lang.notifier
                  : qsTr("Copy to clipboard") + lang.notifier
        }

        /* clickable area */

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: copyBtn.hovered = true;
            onExited:  copyBtn.hovered = false;
            onClicked: {
                if (copyBtn.pressed)
                    return;

                copyBtn.pressed = true;
                copyBtnTimer.start();

                root.sigCopy();
            }
        }

        /* unpress timer */

        Timer {
            id: copyBtnTimer
            interval: 2000
            repeat: false
            running: false
            onTriggered: copyBtn.pressed = false;
        }
    }

    /****************************************//**
     * Checkbox
     ********************************************/

    DapQmlRectangle {
        qss: "seedphrase-understand-frame"

        RowLayout {
            anchors.fill: parent

            Item {
                Layout.minimumWidth: ucd.width * 0.5
                Layout.maximumWidth: ucd.width * 0.5
                Layout.fillHeight: true
                clip: true

                DapQmlRectangle {
                    id: ucd
                    anchors.centerIn: parent
                    qss: "seedphrase-understand-chbox"

                    property bool hovered: false
                    property bool checked: false

                    DapQmlDummy { id: chOn;  property string scaledPixmap; qss: "checkbox-on" }
                    DapQmlDummy { id: chOff; property string scaledPixmap; qss: "checkbox-off" }

                    DapQmlImage {
                        anchors.fill: parent
                        visible: ucd.checked || ucd.hovered
                        opacity: (!ucd.checked && ucd.hovered) ? 0.5 : 1
                        scaledPixmap: chOn.scaledPixmap
                    }

                    DapQmlImage {
                        anchors.fill: parent
                        visible: !ucd.checked && !ucd.hovered
                        scaledPixmap: chOff.scaledPixmap
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: ucd.hovered = true
                        onExited:  ucd.hovered = false
                        onClicked: ucd.checked = !ucd.checked
                    }
                }
            }

            DapQmlLabel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                wrapMode: Text.WordWrap
                horizontalAlign: Text.AlignLeft
                qss: "seedphrase-understand-description"
                text: qsTr("I understand that if I lose my seed"
                           + " phrase that I will not be able to"
                           + " recover my key.")
                           + lang.notifier
            }
        }
    }

    /****************************************//**
     * Accept
     ********************************************/

    DapQmlSeparator {
        qss: "seedphrase-accept-separator"
    }

    DapQmlPushButton {
        qss: "seedphrase-accept-btn"
        text: qsTr("ACCEPT") + lang.notifier
    }
}

/*-----------------------------------------*/
