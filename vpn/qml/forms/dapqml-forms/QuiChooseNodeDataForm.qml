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
 * @date 2022
 * @author Stanislav
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
    property string viewType: "default"

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(name: string);

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
                id: redioButton
                text: model.name
                checked: model.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
                onClicked: nodeDataForm.sigSelect (model.hash);

                DapQmlLabel {
                    horizontalAlign: (viewType=="orderView") ? Text.AlignLeft : Text.AlignRight
                    verticalAlign: (viewType=="orderView") ? Text.AlignBottom : Text.AlignVCenter
                    text: model.subText
                    //leftPadding: redioButton.textPadding
                    qss: "radiobtn-resizer-note"
                    width: resizer.width
                    clip: false
                    //onClicked: { root.toggle(); root.clicked(); }
                    onClicked: nodeDataForm.sigSelect (model.hash);
                }
            }

        }
        onCurrentIndexChanged: {

        }
    }
}

/*-----------------------------------------*/
