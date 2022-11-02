import QtQuick 2.4
import QtQml 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0

Image {
    property string indicatorSrcNormal: "qrc:/Resources/BlackTheme/icons/other/ic_infoGray.svg"
    property string indicatorSrcHover: "qrc:/Resources/BlackTheme/icons/other/ic_infoGray.svg"
    property string contentText: ""
    property alias toolTip: toolTip
    property alias text: text
    property font textFont: mainFont.dapFont.medium12

    signal clicked()


    mipmap: true

    source: area.containsMouse ? indicatorSrcHover : indicatorSrcNormal

    MouseArea{
        id: area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: parent.clicked()
    }

    ToolTip{
        id: toolTip
        visible: area.containsMouse
        width: 213
        y: -(height + 10)
        x: 0

        contentItem:
        Item{
            anchors.fill: parent
            anchors.margins: 8
            Text
            {
                id: text
                anchors.fill: parent
                color: currTheme.textColor
                text: contentText
                font: textFont
                horizontalAlignment: Qt.AlignHCenter
                wrapMode: Text.WordWrap
                verticalAlignment: Qt.AlignVCenter
            }
        }

        background:Item{
            Rectangle
            {
                id: backgroundToolTip
                anchors.fill: parent
                radius: 4
                color: currTheme.backgroundMainScreen
            }
            DropShadow {
                anchors.fill: backgroundToolTip
                source: backgroundToolTip
                color: currTheme.reflection
                horizontalOffset: -1
                verticalOffset: -1
                radius: 0
                samples: 0
                opacity: 1
                fast: true
                cached: true
            }
            DropShadow {
                anchors.fill: backgroundToolTip
                source: backgroundToolTip
                color: currTheme.shadowColor
                horizontalOffset: 2
                verticalOffset: 2
                radius: 10
                samples: 20
                opacity: 1
            }
        }
    }
}
