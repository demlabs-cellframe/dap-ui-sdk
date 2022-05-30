import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

ComboBox {
    id: control

    implicitHeight: 45

    leftPadding: 10
    rightPadding: 10

    property int maximumPopupHeight: 200

    property string mainTextRole: "name"
    property string secondTextRole: "secondname"

    property string defaultText: qsTr("Undefined")

    displayText: currentIndex >= 0 ?
                     getModelData(currentIndex, mainTextRole) :
                     defaultText

    delegate:
    ItemDelegate
    {
        id: menuDelegate
        width: control.width

        contentItem:
        RowLayout
        {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            Text
            {
                Layout.fillWidth: true
                text: getModelData(index, mainTextRole)
                color: currTheme.textColor
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Text
            {
                text: getModelData(index, secondTextRole)
                color: currTheme.textColor
                font.family: control.font.family
                font.pointSize: control.font.pointSize - 2
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        background:
        Rectangle
        {
            anchors.fill: parent
            color: menuDelegate.highlighted ?
                       currTheme.buttonColorHover :
                       currTheme.backgroundMainScreen
        }

        highlighted: control.highlightedIndex === index
    }

    indicator:
    Image
    {
        id: canvas
        width: 24
        height: 24
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/icons/" + pathTheme + "/icon_arrow_down.png"
//        source: "qrc:/icon_arrow_down.png"
        sourceSize.width: 24
        rotation: control.popup.opened ? 180 : 0

        Behavior on rotation { NumberAnimation { duration: 200 } }
    }

    contentItem:
    Text
    {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: currTheme.textColorGray
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background:
    Item
    {
        anchors.fill: parent

        Rectangle
        {
            id: backGrnd
            width: parent.width
            height: control.popup.visible ?
                        parent.height + popupBackGrnd.height :
                        parent.height
            y: control.popup.visible && control.popup.y < 0?
                 - popupBackGrnd.height :
                 0

            color: control.popup.visible ?
                       currTheme.backgroundMainScreen :
                       currTheme.backgroundElements
            radius: 4
        }

        DropShadow
        {
            anchors.fill: backGrnd
            horizontalOffset: currTheme.hOffset
            verticalOffset: currTheme.vOffset
            radius: currTheme.radiusShadow
            color: currTheme.shadowColor
            source: backGrnd
            samples: 10
            cached: true
            visible: control.popup.visible
        }
    }

    popup:
    Popup
    {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight + 3
            //+3 is needed to make ListView less moovable
        padding: 1

        contentItem:
            ListView
            {
                id: listView
                implicitHeight:
                    contentHeight < maximumPopupHeight ?
                        contentHeight : maximumPopupHeight

                clip: true

                model: control.popup.visible ?
                           control.delegateModel : null
                currentIndex: control.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

        background:
        Rectangle
        {
            id: popupBackGrnd
            anchors.fill: parent

            color: currTheme.backgroundElements
        }
    }

    function getModelData(index, role)
    {
        var text = model.get(index)[role]

        if (text === undefined)
            return ""
        else
            return model.get(index)[role];
    }
}
