/* INCLUDES */

import QtQuick 2.1
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Server Form
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
    id: nodeDataForm

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

//    /// @brief items array
//    ///
//    /// Need to store all items
//    property var items: new Array

    property string title: "SET TITLE!!!"
    property QtObject dataModel: null

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);
//    signal sigCurrentInexChanged();

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
//        text: title + lang.notifier
        text: nodeDataForm.title
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

//        x: (root.width - width) / 2
//        y: title.y + title.height * 2 + countryFilterField.height
//        width: resizer.width
//        height: root.height - y - noticeResizer.height - noticeSpacer.height
//        clip: true

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        model: nodeDataForm.dataModel

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
//                    root.sigSelect (model.index, model.name);
//                    csListView.currentIndex = model.index;
                }
            }
        }
        onCurrentIndexChanged: {
//            root.updateChecks();
        }
    }
}

/*-----------------------------------------*/
