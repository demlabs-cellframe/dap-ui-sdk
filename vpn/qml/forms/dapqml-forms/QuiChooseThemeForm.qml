/* INCLUDES */

import QtQuick 2.0
import DapQmlThemeModel 1.0
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
 * -Theme model
 * -Resizer for items
 * -Listview with DapQmlRadioButton as delegate
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
    property string formName: "ChooseTheme"

    /// @brief items array
    ///
    /// Need to store all items
    property var items: Array();

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

    /// @brief request theme update ( DapQmlStyle.requestRedraw() ) and start return timer
    function updateState() {
        csListView.enabled = false;
        style.requestRedraw();
        backTimer.start();
    }

    /// @brief change current item index (int index)
    function setCurrentIndex(a_index) {
        csListView.currentIndex = a_index;
    }

    /// @}
    /****************************************//**
     * Timers
     ********************************************/

    Timer {
        id: backTimer
        interval: 350
        running: false
        repeat: false
        onTriggered: PageCtl.slotBackwardAuto()
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: "Language"
        qss: "dialog-title"
    }

    /****************************************//**
     * Model
     ********************************************/

    property DapQmlThemeModel themeModel: DapQmlThemeModel {
        id: themeModel
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

        model: themeModel

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height
            property string radioName: model.name
            property bool checked: false

            DapQmlRadioButton {
                text: model.name + " Theme"
                checked: csListView.currentIndex === model.index
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2
                onClicked: root.sigSelect (model.index, model.name);
            }

            Component.onCompleted: items.push(this)
        }
    }
}

/*-----------------------------------------*/
