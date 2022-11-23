/* INCLUDES */

import QtQuick 2.0
import DapQmlCountryModel 1.0
import DapQmlCountrySortFilterProxyModel 1.0
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
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseCountry"
    property int currentIndex: -1;
    property int filterXPosition: 37

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
    }

    function updateChecks() {
        countryModel.updateCheckedIndex();
        setFocusTimer.start();
    }

    function countryExist() {
        console.log("countryModel.countryExist();", countryModel.countryExist());
        return countryModel.countryExist();
    }

    Component.onCompleted: updateChecks()

    /// @}
    /****************************************//**
     * Timers
     ********************************************/

    Timer {
        id: setFocusTimer
        interval: 500
        running: false
        repeat: false
        onTriggered: {countryFilterLine.setFocus();}
    }

    Timer {
        id: backTimer
        interval: 30
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

//    DapQmlCountryModel {
    DapQmlCountrySortFilterProxyModel{
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

    DapQmlRectangle {
        id: countryFilterField
        x: root.filterXPosition
        y: title.y + title.height * 2
        height: 32
        width: parent.width - 70
        qss: "ch-country-filter-border"
//        Rectangle {
//            color: "gray"
//            anchors.fill: parent
//        }
        DapQmlLineEdit {
            id: countryFilterLine
            objectName: "countryFilterLine"
            x: 2
            y: 2
            z: 15
            height: parent.height - 4
            width: parent.width - lineEditlIconRight.width
            focus: true

            mainText: ""
            mainQss: "ch-country-filter-text"
            iconLeft: "ic_country_filter"
            onTextChanged: {
                countryModel.setRowFilter(mainText);
            }
            onTextEdited: {
                countryModel.setRowFilter(mainText);
            }
        }

        DapQmlPushButton {
            id: lineEditlIconRight
            x: parent.width - width
            y: parent.height/2 - height/2
            z: 15
            height: 28
            width: 28
            inactive: "qrc:/light/btn_filter_clear.png"
            active: "qrc:/light/btn_filter_clear_hover.png"
            qss: "ch-country-filter-clear"
            visible: countryFilterLine.mainText.length > 0
            onClicked: {
                countryFilterLine.mainText = "";
                countryFilterLine.setFocus();
            }
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
        y: title.y + title.height * 2 + countryFilterField.height
        width: resizer.width
        height: root.height - y - noticeResizer.height - noticeSpacer.height
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

    /****************************************//**
     * Notice
     ********************************************/

    DapQmlRectangle {
        id: noticeResizer
        visible: false
        qss: "ph-notice-resizer"
    }

    DapQmlRectangle {
        id: noticeSpacer
        visible: false
        qss: "ph-notice-spacer"
    }

    DapQmlLabel {
        id: notice
        x: (root.width - noticeResizer.width) / 2
        y: root.height - noticeResizer.height - noticeSpacer.height
        width: noticeResizer.width
        height: noticeResizer.height
        text: "This selection will exclude your country from automatic server selection."
        wrapMode: Text.WordWrap
        qss: "ph-label-notice"
    }
}

/*-----------------------------------------*/
