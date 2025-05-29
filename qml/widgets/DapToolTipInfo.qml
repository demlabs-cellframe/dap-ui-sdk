import QtQuick 2.4
import QtQml
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import Qt5Compat.GraphicalEffects

Image {
    property string indicatorSrcNormal: pathResources + pathTheme + "/icons/other/ic_infoGray.svg"
    property string indicatorSrcHover:  pathResources + pathTheme + "/icons/other/ic_infoGray.svg"
    property string contentText: ""
    property alias toolTip: toolTip
    property alias text: text
    property alias mouseArea: area
    property font textFont: mainFont.dapFont.medium12
    property double scaleFactor: 1.0

    signal clicked()

//    mipmap: true

    source: area.containsMouse ? indicatorSrcHover : indicatorSrcNormal

    sourceSize: Qt.size( img.sourceSize.width, img.sourceSize.height )

    Image {
        id: img
        source: parent.source
        width: 0
        height: 0
    }

    MouseArea{
        id: area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: parent.clicked()
    }

    ToolTip{
        id: toolTip
        visible: area.containsMouse
        width: 213 * scaleFactor
        y: -(height + 10 * scaleFactor)
        x: 0

        contentItem:
        Item{
            anchors.fill: parent
            anchors.margins: 8 * scaleFactor
            Text
            {
                id: text
                anchors.fill: parent
                color: currTheme.white
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
                color: currTheme.mainBackground
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
