import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

Page
{
    id: root

    title: root.title

    background: Rectangle {
        id: backGrndRect
        color: currTheme.backgroundMainScreen
    }

    contentItem: Rectangle {
        id: frame
        anchors.fill: parent
        anchors.margins: 24 * pt
        color: currTheme.backgroundElements
        radius: 16 * pt

        InnerShadow {
            id: topLeftSadow
            anchors.fill: frame
            cached: true
            horizontalOffset: 5
            verticalOffset: 5
            radius: 4
            samples: 32
            color: "#2A2C33"
            smooth: true
            source: frame
        }
        InnerShadow {
            anchors.fill: frame
            cached: true
            horizontalOffset: -1
            verticalOffset: -1
            radius: 1
            samples: 32
            color: "#4C4B5A"
            source: topLeftSadow
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 15

            Rectangle {
                id: closeButton
                color: frame.color
                width: 20 * pt
                height: 20 * pt

                Image {
                    id: btnIco
                    anchors.fill: parent
                    source: "qrc:/resources/icons/" + pathTheme + "/close_icon.png"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onEntered: btnIco.source = "qrc:/resources/icons/" + pathTheme + "/close_icon_hover.png"
                    onExited: btnIco.source = "qrc:/resources/icons/" + pathTheme + "/close_icon.png"
                }

            }

            Text {
                text: title
                Layout.alignment: Qt.AlignLeft
                color: currTheme.textColor
            }

            Item {
                id: rowSpacer
                Layout.fillWidth: true
            }
        }

        Item {
            id: columnSpacer
            Layout.fillHeight: true
        }
    }
}
