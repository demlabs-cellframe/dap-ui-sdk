import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

ComboBox {
    id: control
//    ///@detalis mainRow The model role for the main line of cloded ComboBox.
//    property var mainRow: [""]
//    //@detalis mainLineText Text without unneccesary part.
//    property string mainLineText
//    ///@detalis comboBoxTextRole The model roles used for the ComboBox.
//    property var comboBoxTextRole: ["text"]


//    model: (popup.visible) ? comboBoxTextRole.length : 1

        leftPadding: 10

        delegate: ItemDelegate {
            id: menuDelegate
            width: control.width

            contentItem: Text {
                text: modelData.name
                color: currTheme.textColor
                font: dapQuicksandFonts.dapMainFontTheme.dapFontQuicksandRegular16
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

            width: 24; height: 24
            source: "qrc:/resources/icons/"+pathTheme+"/icon_arrow_down.png"
            rotation: control.popup.opened ? 180 : 0

            ColorOverlay {
                anchors.fill: parent
                source: parent

                color: parent.color
            }

//            Behavior on rotation { NumberAnimation { duration: 200 } }
        }

        contentItem: Text {
            leftPadding: 0
            rightPadding: control.indicator.width + control.spacing

            text: control.displayText
            font: control.font
            color: currTheme.textColor
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
