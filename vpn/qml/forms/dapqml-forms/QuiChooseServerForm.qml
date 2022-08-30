/* INCLUDES */

import QtQuick 2.1
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
    id: root

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseServer"

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
//    signal sigCurrentInexChanged();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief change current item index (int index)
    function setCurrentIndex(a_index) {
        csListView.currentIndex = a_index;

//        var count           = csListView.count
//        var current         = csListView.currentIndex;
//        for(var i = 0; i < count; i++) {
//            var entry       = csListView.itemAtIndex(i);
//            entry.checked   = (i === current)
//        }
//        root.sigCurrentInexChanged();
    }

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Choose server") + lang.notifier
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
        objectName: "chooseServerListView"

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
                property int ping: model.ping
                property int quality: model.connectionQuality

                text: model.name
                checked: csListView.currentIndex === model.index // parent.checked
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2

                Text {
                    text: `index [${parent.myIndex}] ping [${parent.ping}] quality [${parent.quality}]`
                }

                onClicked: root.sigSelect (model.index, model.name)
                Component.onCompleted: { items.push(this); }
            }
        }
    }
}

/*-----------------------------------------*/
