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

    property QtObject internal: QtObject {
        property bool finished: false
        property var checkboxes: []
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
        text: "FAQ"
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
            height: (labelHeight > faqItemSizer.height ? labelHeight : faqItemSizer.height)
                    + faqItemSep.height
                    + (openedFlag ? contentHeight : 0)

            property bool openedFlag: root.internal.checkboxes[model.index] || false
            property real labelHeight: faqItemLabel.height
            property real contentHeight: faqItemContent.height + faqItemSpacer.height

            Behavior on height { PropertyAnimation { duration: root.internal.finished ? 250 : 0 } }

            /* title */
            DapQmlLabel {
                id: faqItemLabel
                y: (faqItemSizer.height - height) / 2
                width: parent.width - faqItemPlusBtn.width
                height: contentHeight // faqItemSizer.height
                horizontalAlign: Text.AlignLeft
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                qss: "faq-item-label c-label"
                text: model.question //"Question"
            }

            /* plus button */
            DapQmlPushButton {
                id: faqItemPlusBtn
                x: parent.width - faqItemPlusBtnStyle.width + ((faqItemPlusBtnStyle.width - width) / 2)
                y: (faqItemSizer.height - height) / 2
                qss: openedFlag ? "faq-close-btn" : "faq-plus-btn"

                DapQmlDummy { id: faqItemPlusBtnStyle; qss: "faq-close-btn" }
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
                disableClicking: true
                textFormat: Text.RichText
                qss: "faq-item-text"
                text: model.answer  // "Is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived"
            }

            /* separator */
            DapQmlSeparator {
                id: faqItemSep
                y: parent.height - height
            }

            MouseArea {
                width: parent.width
                height: faqItemSizer.height + faqItemSep.height
                onClicked: {
                    openedFlag = !openedFlag;
                    root.internal.checkboxes[model.index] = openedFlag;

                    if (openedFlag) {
                        Qt.callLater(() => {
                            const item = listview.itemAtIndex(model.index);
                            if (item) {
                                const estimatedHeight = (labelHeight > faqItemSizer.height ? labelHeight : faqItemSizer.height)
                                             + faqItemSep.height + item.contentHeight;
                                const targetY = item.y + estimatedHeight - listview.height;

                                if (targetY > listview.contentY) {
                                    listview.contentY = targetY;
                                }
                            }
                        });
                    }
                }
            }
        }
    }

    /****************************************//**
     * Content scrollarea
     ********************************************/

    ListView {
        id: listview
        objectName: "faqListView"
        spacing: 4

        DapQmlStyle { item: listview; qss: "faq-content"; }

        height: root.height - y
        clip: true

        delegate: faqItem

        Behavior on contentY {
            NumberAnimation {
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }
    }
}

/*-----------------------------------------*/
