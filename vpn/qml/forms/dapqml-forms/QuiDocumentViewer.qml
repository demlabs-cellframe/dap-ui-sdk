/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0

/****************************************//**
 * @brief document Viewver Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using ScrollView with Text item inside
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
    property string formName: "DocumentViewer"

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief setup document
    function show(a_docName, a_docContent) {
        title.text      = a_docName;
        content.text    = a_docContent;
    }

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: "Dialog title"
        qss: "dialog-title"
    }

    /****************************************//**
     * Content scrollarea
     ********************************************/

    ScrollView
    {
        id: scroll
        clip: true
        height: root.height - y
        contentWidth: width
        property int fontSize: 12
        DapQmlStyle { item: scroll; qss: "dv-content"; }

        //Component.onCompleted: scroll.setHorizontalScrollBarEnabled (false);

        Text {
            id: content
            width: scroll.width - scroll.width / 16
            font.pixelSize: scroll.fontSize
            textFormat: Text.RichText
            wrapMode: Text.Wrap
            lineHeight: 1.5

            DapQmlStyle { item: content; qss: "dv-text"; }
        }
    }
}

/*-----------------------------------------*/
