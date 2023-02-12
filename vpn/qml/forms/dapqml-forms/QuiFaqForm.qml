/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"
import DapQmlStyle 1.0

/****************************************//**
 * @brief FAQ Form
 * @ingroup groupDapQmlForms
 *
 * @date 12.02.23
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
    property string formName: "FAQ"

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

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
     * Sizers
     ********************************************/

    DapQmlDummy { id: faqItemSizer; qss: "faq-item" }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: faqItem

        Item {
            clip: true
            width: scroll.width
            height: faqItemSizer.height + faqItemSep.height + (opened ? contentHeight : 0)

            property bool opened: false
            property real contentHeight: faqItemContent.height

            Behavior on height { PropertyAnimation { duration: 250 } }

            /* title */
            DapQmlLabel {
                id: faqItemLabel
                y: (faqItemSizer.height - contentHeight) / 2
                qss: "faq-item-label"
                text: "Question"
            }

            /* plus button */
            DapQmlPushButton {
                id: faqItemPlusBtn
                x: parent.width - width
                y: (faqItemSizer.height - height) / 2
                qss: "faq-plus-btn"
            }

            /* content */
            DapQmlLabel {
                id: faqItemContent
                width: parent.width
                height: contentHeight
                wrapMode: Text.WordWrap
                qss: "faq-item-text"
                text: "Is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived"
            }

            /* separator */
            DapQmlSeparator {
                id: faqItemSep
                y: parent.height - height
            }
        }
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
        contentHeight: scrollContent.childrenRect.height

        DapQmlStyle { item: scroll; qss: "faq-content"; }

        ColumnLayout {
            id: scrollContent
            width: scroll.width
            Loader { width: parent.width; height: faqItemSizer.height; sourceComponent: faqItem }
            Loader { width: parent.width; height: faqItemSizer.height; sourceComponent: faqItem }
            Loader { width: parent.width; height: faqItemSizer.height; sourceComponent: faqItem }
        }
    }
}

/*-----------------------------------------*/
