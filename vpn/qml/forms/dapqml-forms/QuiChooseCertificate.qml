/* INCLUDES */

import QtQuick 2.1
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Certificate Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using simple ListView with DapQmlRadioButton as delegate
 *
 * Form contains resizer for ListView items
 *
 * @date 25.09.22
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
    property string formName: "ChooseCert"

    /// @brief items array
    ///
    /// Need to store all items
    property var items: new Array

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
        text: qsTr("Choose certificate") + lang.notifier
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
        objectName: "chooseCertListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height
            property bool checked: false

            DapQmlRadioButton {
                property int myIndex: model.index

                text: model.name
                checked: csListView.currentIndex === model.index // parent.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
                onClicked: root.sigSelect (model.index, model.name)
                Component.onCompleted: { items.push(this); }
            }
        }
    }
}

/*-----------------------------------------*/
