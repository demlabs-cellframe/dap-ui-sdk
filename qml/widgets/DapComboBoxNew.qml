import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

ComboBox {
    id: control

    leftPadding: 10

    delegate: ItemDelegate {
        id: menuDelegate
        width: control.width

        contentItem: Text {
            text: modelData
            color: currTheme.textColor
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            anchors.fill: parent
            color: menuDelegate.highlighted ? currTheme.buttonColorHover : currTheme.backgroundMainScreen
        }

        highlighted: control.highlightedIndex === index
    }

    indicator: Image {
        id: canvas

        property color color: "transparent"

        x: control.width - width - 20
        y: control.topPadding + (control.availableHeight - height) / 2

        width: 25; height: 15
        fillMode: Image.PreserveAspectFit
        source: "qrc:/icons/arrow_down.svg"
        sourceSize.width: 12
        rotation: control.popup.opened ? 180 : 0

        ColorOverlay {
            anchors.fill: parent
            source: parent

            color: parent.color
        }

        Behavior on rotation { NumberAnimation { duration: 200 } }
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: "Gray"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitHeight: 50
        implicitWidth: 260
        color: control.popup.visible ? currTheme.backgroundMainScreen : currTheme.backgroundElements
        radius: 4
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            id: popupBackGrnd
            anchors.fill: parent
            color: currTheme.backgroundMainScreen
        }
    }
}
