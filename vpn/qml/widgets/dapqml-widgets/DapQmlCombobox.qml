/* INCLUDES */

import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import DapQmlStyle 1.0
import Brand 1.0
import StyleDebugTree 1.0

/****************************************//**
 * @brief Dap QML Combobox Widget
 * @ingroup groupDapQmlWidgets
 *
 * ### Structure
 *
 * Widget with text label, separator and
 * submenu
 *
 * @date 19.08.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    property string qss
    property string title: ""
    property var items: []
    property int currentItem: -1

    DapQmlStyle { id: style; qss: root.qss; item: root }

    property QtObject internal: QtObject {
        property bool menuOpen: false
        property int padding: resizer.fontSize / 2
        property real preferHeight: root.height * 8
    }

    /// @brief animation fixer
    Timer {
        id: titleLabelFixer
        interval: 100
        running: true
        repeat: false
        onTriggered: {
            titleLabel.duration = 250
        }
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function _updateContent() {
        for (let i = 0; i < resources.length; i++)
        {
            let item        = resources[i];
            let shortcut    = ('shortcut' in item);
            let toggle      = ('toggle' in item);
            let text        = ('text' in item);

            if (shortcut && toggle && text)
            {
                // console.log (item.text);

                /* store */
                items.push(item);

                /* send into model */
                model.append({"text":item.text});

                /* change current */
                if (root.currentItem === -1)
                    root.currentItem = 0;
            }
        }
    }

    function _currentUpdated() {
        let before = titleLabel.qss;

        titleLabel.qss =
            (root.currentItem !== -1)
            ? "combobox-placeholder-out c-grey"
            : "combobox-placeholder-in c-grey";

        itemLabel.text =
            (root.currentItem !== -1)
            ? root.items[root.currentItem].text
            : "";

        //console.log(`tl index (${root.currentItem}) before (${before}) after (${titleLabel.qss})`);
    }

    Component.onCompleted: _updateContent()
    onCurrentItemChanged: _currentUpdated()

    /// @}
    /****************************************//**
     * Resizer
     ********************************************/

    DapQmlLabel {
        id: resizer
        property color backgroundColor
        property color hoverBackgroundColor
        qss: "combobox-item"
    }

    /****************************************//**
     * Separator
     ********************************************/

    DapQmlSeparator {
        id: separator
        x: 0
        y: root.height - height
        width: root.width
        qss: "combobox-separator"
    }

    /****************************************//**
     * Arrow
     ********************************************/

    DapQmlLabel {
        id: arrow
        x: root.width - width * 1.5
        y: (root.height - height) / 2
        z: 17
        qss: internal.menuOpen
             ? "combobox-arrow combobox-arrow-down"
             : "combobox-arrow combobox-arrow-up"
    }

    /****************************************//**
     * Title Label
     ********************************************/

    DapQmlLabel {
        id: titleLabel
        width: root.width
        height: root.height
        horizontalAlign: Text.AlignLeft
        qss: "combobox-placeholder-in c-grey"
        text: root.title
        property int duration: 0
        Behavior on y { PropertyAnimation { duration: titleLabel.duration } }
        Behavior on fontSize { PropertyAnimation { duration: titleLabel.duration } }
    }

    /****************************************//**
     * Current text
     ********************************************/

    DapQmlLabel {
        id: itemLabel
        width: root.width
        height: root.height
        horizontalAlign: Text.AlignLeft
        verticalAlign: Text.AlignBottom
        qss: "combobox-text c-label"
        //text: (root.currentItem === -1) ? "" : root.items[root.currentItem].text
    }

    /****************************************//**
     * List
     ********************************************/

    MouseArea {
        anchors.fill: root
        onClicked: internal.menuOpen = true
    }

    ListModel {
        id: model
        //         ListElement {
        //             text: "Bill Smith"
        //         }
        //         ListElement {
        //             text: "John Brown"
        //         }
        //         ListElement {
        //             text: "Sam Wise"
        //         }

        function actualHeight() {
            let preferHeight = count * resizer.height;

            if (preferHeight > root.internal.preferHeight)
                return root.internal.preferHeight + root.internal.padding * 2

            return preferHeight + root.internal.padding * 2;
        }
     }

    DapQmlRectangle {
        x: -root.internal.padding
        //y: -root.internal.padding
        z: 15
        width: root.width + root.internal.padding * 2
        height: model.actualHeight()
        visible: internal.menuOpen

        property color borderColor
        qss: "c-background combobox-border-color"

        border {
            width: 1
            color: borderColor
        }

        radius: resizer.fontSize / 4
    }

    ListView {
        id: listviewItems
        x: -root.internal.padding+1
        //y: -root.internal.padding/2
        //x: 1 // root.internal.padding
        y: root.internal.padding
        z: 16
        visible: internal.menuOpen
        width: root.width - 2 + root.internal.padding * 2
        height: model.actualHeight() - root.internal.padding * 2
        model: model // root.items

        delegate: Button {
            property int myIndex: model.index
            width: listviewItems.width
            height: resizer.height
            text: model.text
            flat: true

            contentItem: Item {
                property string text: parent.text
                Text {
                    x: root.internal.padding
                    text: parent.text
                    horizontalAlignment: Text.AlignLeft
                }
            }

            background: Rectangle {
                color: hovered ? resizer.hoverBackgroundColor : resizer.backgroundColor
            }

            onClicked: {
                root.currentItem    = myIndex;
                internal.menuOpen   = false
            }
        }

        Component.onCompleted: StyleDebugTree.describe (
           "listviewItems",
            ["x", "y", "width", "height"],
           this);
    }
}

/*-----------------------------------------*/
