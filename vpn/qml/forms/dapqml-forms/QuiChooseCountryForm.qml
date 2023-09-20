/* INCLUDES */

import QtQuick 2.0
import DapQmlStyle 1.0
//import StyleDebugTree 1.0
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

    DapQmlStyle { id: style }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);

    /// @brief update filter string
    signal sigFilterChanged(string a_filter);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    Component.onCompleted: setFocusTimer.start();

    /// @}
    /****************************************//**
     * Timers
     ********************************************/

    Timer {
        id: setFocusTimer
        interval: 250
        running: false
        repeat: false
        onTriggered: {countryFilterLine.setFocus();}
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
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    /****************************************//**
     * Search field
     ********************************************/

    DapQmlRectangle {
        id: countryFilterField
        x: (parent.width - width) / 2
        y: title.y + title.height * 2
        //height: 32
        width: resizer.width // parent.width - 70
        qss: "ch-country-filter-border"

        property real innerSize: height * 0.875

//        Component.onCompleted: StyleDebugTree.describe (
//           "countryFilterField",
//            ["x", "y", "z", "width", "height"],
//           this);

        DapQmlLineEdit {
            id: countryFilterLine
            objectName: "countryFilterLine"
            x: parent.height * 0.0625
            y: (parent.height - height) / 2
            z: 15
            height: countryFilterField.innerSize // parent.height - 4
            width: parent.width - lineEditlIconRight.width * 1.5
            focus: true

            mainText: ""
            mainQss: "ch-country-filter-text"
            iconQss: "ic_country_filter"
            placeHolderText: "Search country"

//            Component.onCompleted: StyleDebugTree.describe (
//               "countryFilterLine",
//                ["x", "y", "z", "width", "height"],
//               this);

            onTextChanged: {
                root.sigFilterChanged(mainText); // countryModel.setRowFilter(mainText);
            }
            onTextEdited: {
                root.sigFilterChanged(mainText); // countryModel.setRowFilter(mainText);
            }
        }

        DapQmlPushButton {
            id: lineEditlIconRight
            x: parent.width - width - y
            y: (parent.height - height) / 2
            z: 15
            height: countryFilterField.innerSize * 0.5
            width: countryFilterField.innerSize * 0.5
            visible: countryFilterLine.mainText.length > 0

            qss: "ch-country-filter-clear"

//            Component.onCompleted: StyleDebugTree.describe (
//               "lineEditlIconRight",
//                ["x", "y", "z", "width", "height", "active", "inactive"],
//               this);

            onClicked: {
                countryFilterLine.mainText = "";
                countryFilterLine.setFocus();
            }
        }
    }

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: csListView
        objectName: "listview"

        x: (root.width - width) / 2
//        y: title.y + title.height * 2 + countryFilterField.height
//        width: resizer.width
//        height: root.height - y - noticeResizer.height - noticeSpacer.height
        clip: true

        DapQmlStyle { qss: "ch-country-listview"; item: csListView }

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height

            DapQmlRadioButton {
                text: model.name
                checked: model.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height + spacer.height
                y: spacer.height / 2
                onClicked: {
                    root.sigSelect (model.index, model.name);
                }
            }
        }
//        onCurrentIndexChanged: {
//            root.updateChecks();
//        }
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
