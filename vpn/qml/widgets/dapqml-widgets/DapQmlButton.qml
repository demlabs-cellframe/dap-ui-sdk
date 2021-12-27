import QtQuick 2.1
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Rectangle {
    id: root

    /* DEFS */

    enum Style
    {
        TopMainBottomSub,     ///< top:main, bottom:sub
        TopSubBottomMain,     ///< top:sub,  bottom:main
        LeftTopMainBottomSub, ///< left:left, top:main, bottom:sub
        IconMainSub,          ///< icon, main, sub
        EditTopMainBottomSub  ///< top:edit, bottom:sub
    }

    /* VARS */

    property string mainText: "Main text"
    property string subText: "Sub text"
    property string leftText: "Left text"
    property color mainColor: "#04004E"
    property color leftColor: "#04004E"
    property color subColor: "#A4A3C0"
    property int buttonStyle: DapQmlButton.Style.TopMainBottomSub
    property bool separator: true

    /* user no background */
    color: "transparent"

    /* Top - Item, Bottom - Separator */
    GridLayout {
        anchors.fill: parent;
        columns: 1

        /* LeftTopMainBottomSub */

        /* top item */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            visible: (root.buttonStyle === DapQmlButton.Style.LeftTopMainBottomSub)

            /* left big label */
            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: root.leftText
                color: root.leftColor
                font.family: "Lato"
                font.pixelSize: 30
                font.weight: Font.DemiBold
            }

            /* right side two labels */
            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 1

                /* main text */
                Text {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    text: root.mainText
                    color: root.mainColor
                    font.family: "Lato"
                    font.pixelSize: 20
                    font.weight: Font.Bold
                }

                /* sub text */
                Text {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    text: root.subText
                    color: root.subColor
                    font.family: "Lato"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                }

            }
        }

        /* TopMainBottomSub */
        /* Two items by vertical */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 1
            visible: (root.buttonStyle === DapQmlButton.Style.TopMainBottomSub)

            /* main text */
            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                text: root.mainText
                color: root.mainColor
                font.family: "Lato"
                font.pixelSize: 20
                font.weight: Font.Bold
            }

            /* sub text */
            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                text: root.subText
                color: root.subColor
                font.family: "Lato"
                font.pixelSize: 12
                font.weight: Font.Normal
            }

        }

        /* TopMainBottomSub */
        /* Two items by vertical */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 1
            visible: (root.buttonStyle === DapQmlButton.Style.TopSubBottomMain)

            /* sub text */
            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                text: root.subText
                color: root.subColor
                font.family: "Lato"
                font.pixelSize: 12
                font.weight: Font.Normal
            }

            /* main text */
            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                text: root.mainText
                color: root.mainColor
                font.family: "Lato"
                font.pixelSize: 20
                font.weight: Font.Bold
            }

        }

        /* bottom item */
        DapQmlSeparator { width: root.width; visible: root.separator }
    }

}
