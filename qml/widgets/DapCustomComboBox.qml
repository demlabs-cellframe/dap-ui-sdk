import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

Item
{
    id: mainItem

    implicitHeight: 45

    property int rightMarginIndicator: 16
    property int leftMarginText: 16
    property int rightMarginText: 16
    property int maximumPopupHeight: 200

    property int bgRadius: 0

    property bool topOrientation: false

    property alias model: popupListView.model

    property string indicatorSource: "qrc:/Resources/" + pathTheme + "/icons/other/icon_arrowDown.svg"

    property int currentIndex: -1
    property string currentText: displayText
    property int count: popupListView.model ? popupListView.model.count : 0

    property bool popupVisible: false

    property font font: mainFont.dapFont.medium14

    property string mainTextRole: "name"
    property string secondTextRole: "secondname"

    property string defaultText: qsTr("Undefined")

    property string displayText: defaultText

    property color backgroundColorNormal: currTheme.mainBackground
    property color backgroundColorShow: currTheme.mainBackground
    property alias background: background

    signal itemSelected(var index)

    onModelChanged:
    {
        console.log("DapCustomComboBox", "onModelChanged",
              "popupListView.currentIndex", popupListView.currentIndex)

        if (popupListView.currentIndex < 0)
//            displayText = getModelData(0, mainTextRole)
            displayText = defaultText
        else
            displayText = getModelData(popupListView.currentIndex, mainTextRole)
    }

    onCountChanged:
    {
        console.log("DapCustomComboBox", "onCountChanged",
              "popupListView.currentIndex", popupListView.currentIndex)

        if (popupListView.currentIndex < 0)
            displayText = getModelData(0, mainTextRole)
        else
            displayText = getModelData(popupListView.currentIndex, mainTextRole)
    }

    Rectangle
    {
        id: background
        anchors.fill: parent

        radius: bgRadius
        color: popupVisible ?
                   backgroundColorNormal :
                   backgroundColorShow

        RowLayout
        {
            anchors.fill: parent
            anchors.leftMargin: leftMarginText
            anchors.rightMargin: rightMarginIndicator

            Text
            {
                id: mainTextItem
                Layout.fillWidth: true

                text: mainItem.displayText
                font: mainItem.font
                color: popupVisible ?
                           currTheme.gray : currTheme.white
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            DapImageRender
            {
                id: indicator
                source: indicatorSource
                rotation: popupVisible ? 180 : 0

                Behavior on rotation
                {
                    NumberAnimation
                    {
                        duration: 200
                    }
                }
            }
        }
    }

    DropShadow
    {
        visible: popupVisible
        anchors.fill: background
        horizontalOffset: currTheme.hOffset
        verticalOffset: topOrientation ?
                            - currTheme.vOffset : currTheme.vOffset
        radius: currTheme.radiusShadow
        color: currTheme.shadowColor
        source: background
        samples: 10
        cached: true
        opacity: 0.42
    }

//    InnerShadow
//    {
//        visible: popupVisible
//        anchors.fill: background
//        horizontalOffset: 1
//        verticalOffset: 1
//        radius: 1
//        samples: 10
//        cached: true
//        color: "#524D64"
//        source: background
//        spread: 0
//    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed:
        {
            popupVisible = !popupVisible

            popup.visible = popupVisible

//            print("DapCustomComboBox", "mouseArea",
//                  "popupVisible", popupVisible)

            if (popupVisible)
            {
                popupListView.positionViewAtIndex(
                            currentIndex, ListView.Contain)
            }
        }
    }

    Popup
    {
        id: popup

//        visible: popupVisible

//        x: 0
//        y: mainItem.height

        x: 0
        y: mainItem.height
           - (topOrientation ? height + background.height : 0)

        width: popupBackground.width
        height: popupBackground.height

        padding: 0

        onVisibleChanged:
        {
//            print("DapCustomComboBox", "onVisibleChanged",
//                  "visible", visible,
//                  "popupVisible", popupVisible)

            if (!mouseArea.containsMouse &&
                visible === false && popupVisible === true)
                popupVisible = false
        }

        Rectangle
        {
            id: popupBackground

            width: mainItem.width
            height: popupListView.height + border.width*2

            color: currTheme.mainBackground

//            border.width: 1
//            border.color: currTheme.mainBackground

            ListView
            {
                id: popupListView

//                visible: popupVisible

                x: popupBackground.border.width
                y: popupBackground.border.width
                width: mainItem.width - popupBackground.border.width*2
                implicitHeight:
                    contentHeight < maximumPopupHeight ?
                        contentHeight : maximumPopupHeight

                clip: true

                ScrollIndicator.vertical:
                    ScrollIndicator { }

                delegate:
                Rectangle
                {
                    id: menuDelegate
                    width: mainItem.width
                    height: 40

                    color: area.containsMouse ?
                               currTheme.lime :
                               currTheme.secondaryBackground

                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.leftMargin: leftMarginText
                        anchors.rightMargin: rightMarginText

                        Text
                        {
                            Layout.fillWidth: true
                            text: getModelData(index, mainTextRole)
                            color: area.containsMouse ?
                                       currTheme.boxes :
                                       currTheme.white
                            font: mainItem.font
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text
                        {
                            text: getModelData(index, secondTextRole)
                            color: area.containsMouse ?
                                       currTheme.boxes :
                                       currTheme.white
                            font.family: mainItem.font.family
                            font.pointSize: mainItem.font.pointSize - 3
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    MouseArea
                    {
                        id: area
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked:
                        {
                            popupListView.currentIndex = index
                            popup.visible = false
                            itemSelected(index)
                        }
                    }
                }

                onCurrentIndexChanged:
                {
                    displayText = getModelData(currentIndex, mainTextRole)
                    mainItem.currentIndex = currentIndex
                }

            }

        }

        DropShadow
        {
            visible: popupVisible
            anchors.fill: popupBackground
            horizontalOffset: currTheme.hOffset
            verticalOffset: topOrientation ?
                                - currTheme.vOffset : currTheme.vOffset
            radius: currTheme.radiusShadow
            color: currTheme.shadowColor
            source: popupBackground
            samples: 10
            cached: true
            opacity: 0.42
        }

//        InnerShadow {
//            visible: popupVisible
//            anchors.fill: popupBackground
//            horizontalOffset: 1
//            verticalOffset: 0
//            radius: 1
//            samples: 10
//            cached: true
//            color: "#524D64"
//            source: popupBackground
//        }
    }

    function getModelData(index, role)
    {
        if(count <= 0)
            return ""

        if (model.get(index) === undefined)
            return ""

        var text = model.get(index)[role]

        if (text === undefined)
            return ""
        else
            return text;
    }

    function setCurrentIndex(index)
    {
        popupListView.currentIndex = index
        mainItem.currentIndex = index
//        currentIndex = index
    }
}
