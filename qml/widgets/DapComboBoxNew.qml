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

//<<<<<<< HEAD
//        leftPadding: 10

//        delegate: ItemDelegate {
//            id: menuDelegate
//            width: control.width

//            contentItem: Text {
//                text: modelData.name
//                color: currTheme.textColor
<<<<<<< HEAD
//                font: _dapQuicksandFonts.dapMainFontTheme.dapFontQuicksandRegular16
=======
//                font: _dapQuicksandFonts.dapFont.regular16
>>>>>>> a9b1601b5b79d4fee7cd74271e87b8a462a0fbc9
//                elide: Text.ElideRight
//                verticalAlignment: Text.AlignVCenter
//            }

//            background: Rectangle {
//                anchors.fill: parent
//                color: menuDelegate.highlighted ? currTheme.buttonColorHover : currTheme.backgroundMainScreen
//            }

//            highlighted: control.highlightedIndex === index
//        }
//=======
    leftPadding: 10

    delegate: ItemDelegate {
        id: menuDelegate
        width: control.width
//>>>>>>> 278412e6b5d1457590deb4ce2cdc26e2fbb04687

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

//<<<<<<< HEAD
//            width: 24; height: 24
//            source: "qrc:/resources/icons/"+pathTheme+"/icon_arrow_down.png"
//            rotation: control.popup.opened ? 180 : 0
//=======
    indicator: Image {
        id: canvas
//>>>>>>> 278412e6b5d1457590deb4ce2cdc26e2fbb04687

        property color color: "transparent"

        x: control.width - width - 20
        y: control.topPadding + (control.availableHeight - height) / 2

//<<<<<<< HEAD
////            Behavior on rotation { NumberAnimation { duration: 200 } }
//        }
//=======
        width: 24; height: 24
//        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/icons/"+pathTheme+"/icon_arrow_down.png"
        sourceSize.width: 12
        rotation: control.popup.opened ? 180 : 0
//>>>>>>> 278412e6b5d1457590deb4ce2cdc26e2fbb04687

        ColorOverlay {
            anchors.fill: parent
            source: parent

//<<<<<<< HEAD
//            text: control.displayText
//            font: control.font
//            color: currTheme.textColor
//            verticalAlignment: Text.AlignVCenter
//            elide: Text.ElideRight
//=======
//            color: parent.color
//>>>>>>> 278412e6b5d1457590deb4ce2cdc26e2fbb04687
        }

//        Behavior on rotation { NumberAnimation { duration: 200 } }
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

        scale: mainWindow.scale

        Component.onCompleted:
        {
            print("DapComboBoxNew POPUP")
        }

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
