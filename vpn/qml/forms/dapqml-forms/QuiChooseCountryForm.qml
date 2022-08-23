/* INCLUDES */

import QtQuick 2.0
import DapQmlCountryModel 1.0
import PageCtl 1.0
import DapQmlStyle 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Theme Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using:
 * -Country model
 * -Resizer for items
 * -Listview with DapQmlRadioButton as delegate
 *
 * @date 08.22
 * @author Stanislav
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseCountry"
    property int currentIndex: -1;

    DapQmlStyle { id: style }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function updateState() {
        backTimer.start();
        console.log("updateState")
    }

    function updateChecks() {
        countryModel.updateCheckedIndex();
        countryFilterLine.setFocus();
    }

    Component.onCompleted: updateChecks()

    /// @}
    /****************************************//**
     * Timers
     ********************************************/

    Timer {
        id: backTimer
        interval: 3050
        running: false
        repeat: false
        onTriggered: PageCtl.slotBackwardAuto()
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: "Country"
        qss: "dialog-title"
    }

    /****************************************//**
     * Model
     ********************************************/

    DapQmlCountryModel {
        id: countryModel
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    Loader {
        id: performanceLoader

        Connections {
//            target: root
            onVisibleChanged: {
                console.log("onVisibleChanged")
            }
        }
    }

    DapQmlButton {
        id: countryFilterLine
        objectName: "countryFilterLine"
        x: (parent.width - width) / 2
        y: title.y + title.height * 2
        z: 15
        width: parent.width - 74
        focus: true
        //height: parent.height

//            Rectangle {
//                color: "gray"
//                anchors.fill: parent
//            }

        buttonStyle: DapQmlButton.Style.EditTopMainBottomSub
        mainText: ""
        subText: "COUNTRY FILTER"
        qss: "login-btn-serial"
        mainQss: "login-btn-main"
        subQss: "login-btn-sub"
        placeHolderQss: "login-btn-main"
        separator: true

//            onClicked: root.sigChooseSerial()
        onTextChanged: {
            var text    = mainText;

            if (text.length == maxCountChar)
                root.textChangedAndFilledOut (mainText);
            else if (text.length == 0)
                root.textChangedAndCleaned();
            else
                root.sigSerialFillingIncorrect();
        }
        onTextEdited: {
            var text    = mainText;

            if (text.length == maxCountChar)
                root.textEditedAndFilledOut (mainText);
            else if (text.length == 0)
                root.textEditedAndCleaned();
            else
                root.sigSerialFillingIncorrect();
        }
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: csListView

        x: (root.width - width) / 2
        y: title.y + title.height * 2 + countryFilterLine.height
        width: resizer.width
        height: root.height - y
        clip: true

        model: countryModel

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height
            property string radioName: model.name
            property bool checked: false

            DapQmlRadioButton {
                text: model.name
                checked: model.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
                modelIndex: model.index
                onClicked: {
                    root.sigSelect (model.index, model.name);
                    csListView.currentIndex = model.index;
                }
            }
        }
        onCurrentIndexChanged: {
            root.updateChecks();
        }
    }
}

/*-----------------------------------------*/
