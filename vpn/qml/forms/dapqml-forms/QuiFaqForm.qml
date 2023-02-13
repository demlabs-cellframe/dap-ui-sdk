/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import StyleDebugTree 1.0
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

    property QtObject internal: QtObject {
        property bool finished: false
    }

    Timer {
        interval: 250
        running: true
        repeat: false
        onTriggered: root.internal.finished = true
    }

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

    DapQmlDummy { id: faqItemSizer;  qss: "faq-item" }
    DapQmlDummy { id: faqItemSpacer; qss: "faq-item-spacer" }

    /****************************************//**
     * Components
     ********************************************/

    Component {
        id: faqItem

        Item {
            clip: true
            width: listview.width
            height: faqItemSizer.height + faqItemSep.height + (opened ? contentHeight : 0)

            property bool opened: false
            property real contentHeight: faqItemContent.height + faqItemSpacer.height

            Behavior on height { PropertyAnimation { duration: root.internal.finished ? 250 : 0 } }

            Component.onCompleted:
            {
                StyleDebugTree.describe (
                   "faqItem" + model.index,
                    ["x", "y", "width", "height", "contentHeight"],
                   this);
            }

            /* title */
            DapQmlLabel {
                id: faqItemLabel
                width: parent.width - faqItemPlusBtn.width
                height: faqItemSizer.height
                horizontalAlign: Text.AlignLeft
                qss: "faq-item-label c-label"
                text: model.question //"Question"

                Component.onCompleted: StyleDebugTree.describe (
                           "faqItemLabel" + model.index,
                            ["x", "y", "width", "height", "fontSize"],
                           this);
            }

            /* plus button */
            DapQmlPushButton {
                id: faqItemPlusBtn
                x: parent.width - width
                y: (faqItemSizer.height - height) / 2
                qss: parent.opened ? "faq-close-btn" : "faq-plus-btn"

                Component.onCompleted: StyleDebugTree.describe (
                           "faqItemPlusBtn" + model.index,
                            ["x", "y", "width", "height", "active"],
                           this);
            }

            /* content */
            DapQmlLabel {
                id: faqItemContent
                y: faqItemSizer.height
                width: parent.width
                height: contentHeight
                lineHeight: 1.5
                horizontalAlign: Text.AlignLeft
                wrapMode: Text.WordWrap
                qss: "faq-item-text"
                text: model.answer  // "Is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived"

                Component.onCompleted: StyleDebugTree.describe (
                           "faqItemContent" + model.index,
                            ["x", "y", "width", "height", "fontSize"],
                           this);
            }

            /* separator */
            DapQmlSeparator {
                id: faqItemSep
                y: parent.height - height
            }

            MouseArea {
                anchors.fill: parent
                onClicked: parent.opened = !parent.opened;
            }
        }
    }

    /****************************************//**
     * Content scrollarea
     ********************************************/

//    ScrollView
//    {
//        id: scroll
//        clip: true
//        height: root.height - y
//        contentWidth: width
//        contentHeight: scrollContent.childrenRect.height

//        DapQmlStyle { item: scroll; qss: "faq-content"; }

//        ColumnLayout {
//            id: scrollContent
//            width: scroll.width
//            Loader { width: parent.width; height: faqItemSizer.height; sourceComponent: faqItem }
//            Loader { width: parent.width; height: faqItemSizer.height; sourceComponent: faqItem }
//            Loader { width: parent.width; height: faqItemSizer.height; sourceComponent: faqItem }
//        }
//    }
    ListView {
        id: listview
        objectName: "faqListView"
        spacing: 4

        DapQmlStyle { item: listview; qss: "faq-content"; }

        Component.onCompleted: StyleDebugTree.describe (
                   "faqListView",
                    ["x", "y", "width", "height"],
                   this);

//        x: (root.width - width) / 2
//        y: title.y + title.height * 2
//        width: resizer.width
        height: root.height - y
        clip: true

        delegate: faqItem
    }
}

/*-----------------------------------------*/
