import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Window 2.12

Item {
    id: mainItem

    property double scaleFactor: 1.0
    property int rightMarginIndicator: 16 * scaleFactor
    property int leftMarginDisplayText: 16 * scaleFactor
    property int leftMarginPopupContain: 16 * scaleFactor
    property int rightMarginPopupContain: 16 * scaleFactor
    property int popupBorderWidth: 1 * scaleFactor

    property int delegateHeight: 40 * scaleFactor
    property bool isHighPopup: false

    property int popupWidth: 0
    property bool isHighlightDisplayTextPopup: false

    property bool changingRound: false
    property bool isSingleColor: false
    property bool isInnerShadow: true
    property bool isNecessaryToHideCurrentIndex: false

    property string displayTextNormalColor: currTheme.white
    property string displayTextPopupColor: currTheme.gray

    property int maximumPopupHeight: 200 * scaleFactor
    property int padding: 15 * scaleFactor
    property int spacing: 15 * scaleFactor

    property alias model: popupListView.model
    property alias popupListView: popupListView

    property int currentIndex: -1
    property string currentText: displayText
    property int count: popupListView.model ? popupListView.model.count : 0

    property bool popupVisible: false

    property font font: mainFont.dapFont.medium14

    property string mainTextRole: "name"
    property string secondTextRole: "secondname"
    property string imageRole: "status"

    property string defaultText: qsTr("Undefined")

    property string displayText: defaultText

    property color backgroundColorNormal: currTheme.mainBackground
    property color backgroundColorShow: currTheme.mainBackground
    property alias background: background

    property string enabledIcon: ""
    property string disabledIcon: ""

    signal itemSelected(var index)
    signal currentDisplayTextChanged(var text)

    implicitHeight: 45 * scaleFactor

    /* Allow key handling when popup is open */
    focus: true
    Keys.onPressed: function (event) {
        if (!(popup.visible || popupVisible))
            return;

        // Close by keyboard scrolling keys; do not block outer scroll (except Escape)
        switch (event.key) {
        case Qt.Key_Up:
        case Qt.Key_Down:
        case Qt.Key_PageUp:
        case Qt.Key_PageDown:
        case Qt.Key_Home:
        case Qt.Key_End:
            closeCombo();
            event.accepted = false;
            break;
        case Qt.Key_Escape:
            closeCombo();
            event.accepted = true;
            break;
        default:
            break;
        }
    }

    onModelChanged: {
        print("DapCustomComboBox", "onModelChanged", "popupListView.currentIndex", popupListView.currentIndex);

        if (popupListView.currentIndex < 0)
            displayText = getModelData(0, mainTextRole);
        else
            displayText = getModelData(popupListView.currentIndex, mainTextRole);

        if (displayText === "")
            displayText = defaultText;

        currentDisplayTextChanged(displayText);
    }

    onCountChanged: {
        // print("DapCustomComboBox", "onCountChanged",
        //       "popupListView.currentIndex", popupListView.currentIndex)

        if (popupListView.currentIndex < 0)
            displayText = getModelData(0, mainTextRole);
        else
            displayText = getModelData(popupListView.currentIndex, mainTextRole);

        if (displayText === "")
            displayText = defaultText;

        currentDisplayTextChanged(displayText);
    }

    Rectangle {
        id: background
        border.width: 0
        anchors.fill: parent

        color: popupVisible && !isHighlightDisplayTextPopup ? backgroundColorNormal : backgroundColorShow

        Rectangle {
            visible: popupVisible && changingRound && !isHighPopup && !isHighlightDisplayTextPopup
            height: parent.radius
            anchors {
                right: parent.right
                left: parent.left
                bottom: parent.bottom
            }
            color: parent.color
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: leftMarginDisplayText
            anchors.rightMargin: rightMarginIndicator

            Text {
                id: mainTextItem
                Layout.fillWidth: true

                text: mainItem.displayText === "" ? defaultText : mainItem.displayText
                font: mainItem.font
                color: popupVisible && !isHighlightDisplayTextPopup ? displayTextPopupColor : displayTextNormalColor
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Image {
                id: indicator
                source: pathResources + pathTheme + "/icons/other/icon_arrowDown.svg"
                rotation: popupVisible ? 180 : 0
                mipmap: true

                Behavior on rotation {
                    NumberAnimation {
                        duration: 200
                    }
                }
            }
        }
    }

    DropShadow {
        visible: popupVisible && !isHighPopup && !isHighlightDisplayTextPopup
        anchors.fill: background
        horizontalOffset: currTheme.hOffset
        verticalOffset: currTheme.vOffset
        radius: currTheme.radiusShadow
        color: currTheme.shadowColor
        source: background
        samples: 10
        cached: true
    }

    InnerShadow {
        visible: popupVisible && isInnerShadow && !isHighPopup && !isHighlightDisplayTextPopup
        anchors.fill: background
        horizontalOffset: 1
        verticalOffset: 1
        radius: 1
        samples: 10
        cached: true
        color: "#524D64"
        source: background
        spread: 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            forceActiveFocus();
            popupVisible = !popupVisible;

            popup.visible = popupVisible;

            //            print("DapCustomComboBox", "mouseArea",
            //                  "popupVisible", popupVisible)

            if (popupVisible) {
                popupListView.positionViewAtIndex(currentIndex, ListView.Contain);
            }
        }
    }

    Popup {
        id: popup
        // scale: scaleFactor
        x: -width * (1 / scale - 1) * 0.5
        y: isHighPopup ? -delegateHeight * (mainItem.count - 1) : mainItem.height - height * (1 / scaleFactor - 1) * 0.5

        width: popupBackground.width
        height: popupBackground.height

        padding: 0
        modal: false
        dim: false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        onVisibleChanged: {
            //            print("DapCustomComboBox", "onVisibleChanged",
            //                  "visible", visible,
            //                  "popupVisible", popupVisible)

            if (!mouseArea.containsMouse && visible === false && popupVisible === true)
                popupVisible = false;
        }

        Rectangle {
            id: popupBackground
            radius: background.radius
            width: popupWidth === 0 ? mainItem.width : popupWidth
            height: !isHighPopup ? popupListView.height + border.width * 2 : popupListView.height + border.width * 2 + delegateHeight + radius

            color: isSingleColor ? background.color : currTheme.mainBackground

            border.width: popupBorderWidth
            border.color: currTheme.mainBackground

            Rectangle {
                visible: popupVisible && changingRound
                height: parent.radius
                anchors {
                    right: parent.right
                    left: parent.left
                    bottom: parent.top
                }
                color: parent.color
            }

            ListView {
                id: popupListView

                //                visible: popupVisible

                x: popupBackground.border.width
                y: popupBackground.border.width
                width: popupBackground.width - popupBackground.border.width * 2
                implicitHeight: contentHeight < maximumPopupHeight ? contentHeight : maximumPopupHeight

                clip: true

                ScrollIndicator.vertical: ScrollIndicator {}

                delegate: Rectangle {
                    id: menuDelegate
                    width: popupWidth === 0 ? mainItem.width : popupWidth
                    height: {
                        if (index === currentIndex && isNecessaryToHideCurrentIndex) {
                            return 0;
                        }

                        return delegateHeight;
                    }

                    color: area.containsMouse ? currTheme.lime : isSingleColor ? background.color : currTheme.mainBackground

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: leftMarginPopupContain
                        anchors.rightMargin: rightMarginPopupContain

                        Text {
                            Layout.fillWidth: true
                            text: getModelData(index, mainTextRole)
                            color: area.containsMouse ? currTheme.boxes : currTheme.white
                            font: mainItem.font
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text {
                            text: getModelData(index, secondTextRole)
                            color: area.containsMouse ? currTheme.boxes : currTheme.white
                            font.family: mainItem.font.family
                            font.pointSize: mainItem.font.pointSize - 3
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        Image {
                            id: statusIcon
                            property var data: getModelData(index, imageRole)
                            visible: data === "" ? false : true
                            // wallets combobox
                            source: data === "Active" ? enabledIcon : disabledIcon
                            mipmap: true
                        }
                    }

                    MouseArea {
                        id: area
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            forceActiveFocus();
                            popupListView.currentIndex = index;
                            // Close completely after selection
                            popup.visible = false;
                            popupVisible = false;
                            itemSelected(index);
                        }
                    }
                }

                onCurrentIndexChanged: {
                    displayText = getModelData(currentIndex, mainTextRole);
                    currentDisplayTextChanged(displayText);
                    mainItem.currentIndex = currentIndex;
                    if (displayText) {
                        console.log("New item selected: " + displayText);
                    }
                }
            }

            Rectangle {
                id: fakeField
                border.width: 0
                //                anchors.fill: parent
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: popupListView.bottom
                anchors.topMargin: background.radius
                visible: isHighPopup
                height: isHighPopup ? mainItem.height - background.radius * 2 : 0
                color: popupVisible ? backgroundColorNormal : backgroundColorShow

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: leftMarginDisplayText
                    anchors.rightMargin: rightMarginIndicator

                    Text {
                        id: fakeMainTextItem
                        Layout.fillWidth: true

                        text: mainItem.displayText === "" ? defaultText : mainItem.displayText
                        font: mainItem.font
                        color: popupVisible ? displayTextPopupColor : displayTextNormalColor
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    Image {
                        id: fakeIndicator
                        source: pathResources + pathTheme + "/icons/other/icon_arrowDown.svg"
                        rotation: popupVisible ? 180 : 0
                        mipmap: true

                        Behavior on rotation {
                            NumberAnimation {
                                duration: 200
                            }
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        forceActiveFocus();
                        popup.visible = false;
                    }
                }
            }
        }

        DropShadow {
            visible: popupVisible
            anchors.fill: popupBackground
            horizontalOffset: currTheme.hOffset
            verticalOffset: currTheme.vOffset
            radius: currTheme.radiusShadow
            color: currTheme.shadowColor
            source: popupBackground
            samples: 10
            cached: true
        }

        InnerShadow {
            visible: popupVisible && isInnerShadow
            anchors.fill: popupBackground
            horizontalOffset: 1
            verticalOffset: 0
            radius: 1
            samples: 10
            cached: true
            color: "#524D64"
            source: popupBackground
        }
    }

    // Close popup on wheel scroll outside of the component (field + popup)
    Item {
        id: overlayCatcher
        // Observe wheel events outside component bounds; prefer Overlay.overlay, fallback to window contentItem
        parent: Overlay.overlay ? Overlay.overlay : (mainItem.window ? mainItem.window.contentItem : mainItem)
        anchors.fill: parent
        visible: popup.visible
        z: -1

        MouseArea {
            anchors.fill: parent
            hoverEnabled: false
            // Do not consume clicks; we only listen for wheel and let others pass
            acceptedButtons: Qt.NoButton
            propagateComposedEvents: true
            onWheel: function (w) {
                // Check if wheel is inside popup background
                var pPopup = popupBackground.mapFromItem(Overlay.overlay, w.x, w.y);
                var insidePopup = pPopup.x >= 0 && pPopup.y >= 0 && pPopup.x <= popupBackground.width && pPopup.y <= popupBackground.height;

                // If high popup mode draws extra field area, include it into inside test
                var insideFake = false;
                if (fakeField && fakeField.visible) {
                    var pFake = fakeField.mapFromItem(Overlay.overlay, w.x, w.y);
                    insideFake = pFake.x >= 0 && pFake.y >= 0 && pFake.x <= fakeField.width && pFake.y <= fakeField.height;
                }

                // Check if wheel is inside the main component field
                var pMain = mainItem.mapFromItem(Overlay.overlay, w.x, w.y);
                var insideMain = pMain.x >= 0 && pMain.y >= 0 && pMain.x <= mainItem.width && pMain.y <= mainItem.height;

                if (!(insidePopup || insideFake || insideMain)) {
                    closeCombo();
                }

                // Do not consume wheel to keep outer scroll working
                w.accepted = false;
            }
        }
    }

    // Helper to fully close combo state (popup + internal flags)
    function closeCombo() {
        popupVisible = false;
        popup.visible = false;
        popup.close()
    }

    function getModelData(index, role) {
        if (count <= 0)
            return "";

        if (model.get(index) === undefined)
            return "";

        var text = model.get(index)[role];

        if (text === undefined)
            return "";
        else
            return text;
    }

    function setCurrentIndex(index) {
        popupListView.currentIndex = index;
        mainItem.currentIndex = index;
    //        currentIndex = index
    }
}
