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
        ShelfCtl.title  = qsTr("Seed Phrase") + lang.notifier
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

        Loader { sourceComponent: compWord; property string text: "word 1" }
        Loader { sourceComponent: compWord; property string text: "word  2" }
        Loader { sourceComponent: compWord; property string text: "word   3" }
        Loader { sourceComponent: compWord; property string text: "word 4" }
        Loader { sourceComponent: compWord; property string text: "word    5" }
        Loader { sourceComponent: compWord; property string text: "word  6" }
        Loader { sourceComponent: compWord; property string text: "word      7" }
        Loader { sourceComponent: compWord; property string text: "word 1" }
        Loader { sourceComponent: compWord; property string text: "word  2" }
        Loader { sourceComponent: compWord; property string text: "word   3" }
        Loader { sourceComponent: compWord; property string text: "word 4" }
        Loader { sourceComponent: compWord; property string text: "word    5" }
        Loader { sourceComponent: compWord; property string text: "word  6" }
        Loader { sourceComponent: compWord; property string text: "word      7" }

        DapQmlStyle { item: wordsGrid; qss: "seedphrase-words-grid" }
        DapQmlDummy { id: wordsGridSpacing; qss: "seedphrase-words-grid-spacing" }
    }

    /****************************************//**
     * Copy button
     ********************************************/

    /****************************************//**
     * Checkbox
     ********************************************/

    /****************************************//**
     * Accept
     ********************************************/
}

/*-----------------------------------------*/
