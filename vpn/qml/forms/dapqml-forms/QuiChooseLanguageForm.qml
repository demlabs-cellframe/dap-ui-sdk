/* INCLUDES */

import QtQuick 2.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Language Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using simple ListView with DapQmlRadioButton as delegate
 *
 * Form contains resizer for ListView items
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
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
    property string formName: "ChooseLanguage"

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

    /// @brief change current item index (int index)
    function setCurrentIndex(a_index) {
        csListView.currentIndex = a_index;
    }

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Language") + lang.notifier
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
     * Listview
     ********************************************/

    ListView {
        id: csListView
        objectName: "chooseLanguageListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        model: lang.model()


        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height

            DapQmlRadioButton {
                text: model.display
                checked: csListView.currentIndex === model.index
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
                onClicked: root.sigSelect (model.index, model.display)
            }
        }
    }
}

/*-----------------------------------------*/
