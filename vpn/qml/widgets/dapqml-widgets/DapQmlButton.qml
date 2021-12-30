import QtQuick 2.1
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

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
    property string qss: ""
    property string mainQss: ""
    property string leftQss: ""
    property string subQss: ""
//    property color mainColor: "#04004E"
//    property color leftColor: "#04004E"
//    property color subColor: "#A4A3C0"
//    property int mainSize: 20
//    property int subSize: 12
//    property int leftSize: 30
//    property int mainWeight: Font.Bold
//    property int subWeight: Font.Normal
//    property int leftWeight: Font.Bold
    property string icon: ""
    property int iconSize: 34
    property int buttonStyle: DapQmlButton.Style.TopMainBottomSub
    property bool separator: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

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
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: root.leftText
                qss: root.leftQss
//                color: root.leftColor
//                font.family: "Lato"
//                font.pixelSize: root.leftSize
//                font.weight: root.leftWeight
            }

            /* right side two labels */
            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 1

                /* main text */
                DapQmlLabel {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    text: root.mainText
                    qss: root.mainQss
//                    color: root.mainColor
//                    font.family: "Lato"
//                    font.pixelSize: root.mainSize
//                    font.weight: root.mainWeight
                }

                /* sub text */
                DapQmlLabel {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    text: root.subText
                    qss: root.subQss
//                    color: root.subColor
//                    font.family: "Lato"
//                    font.pixelSize: root.subSize
//                    font.weight: root.subWeight
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
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                text: root.mainText
                qss: root.mainQss
//                color: root.mainColor
//                font.family: "Lato"
//                font.pixelSize: root.mainSize
//                font.weight: root.mainWeight
            }

            /* sub text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                text: root.subText
                qss: root.subQss
//                color: root.subColor
//                font.family: "Lato"
//                font.pixelSize: root.subSize
//                font.weight: root.subWeight
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
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                text: root.subText
                qss: root.subQss
//                color: root.subColor
//                font.family: "Lato"
//                font.pixelSize: root.subSize
//                font.weight: root.subWeight
            }

            /* main text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                text: root.mainText
                qss: root.mainQss
//                color: root.mainColor
//                font.family: "Lato"
//                font.pixelSize: root.mainSize
//                font.weight: root.mainWeight
            }

        }

        /* IconMainSub */
        /* Three items by horiontal */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
            visible: (root.buttonStyle === DapQmlButton.Style.IconMainSub)

            Image {
                Layout.preferredWidth: root.iconSize
                Layout.preferredHeight: root.iconSize

                source: root.icon
                width: root.iconSize
                height: root.iconSize
            }

            /* main text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                text: root.mainText
                qss: root.mainQss
//                color: root.mainColor
//                font.family: "Lato"
//                font.pixelSize: root.mainSize
//                font.weight: root.mainWeight
            }

            /* sub text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
//                color: root.subColor
//                font.family: "Lato"
//                font.pixelSize: root.subSize
//                font.weight: root.subWeight
            }
        }

        /* bottom item */
        DapQmlSeparator { width: root.width; visible: root.separator }
    }

}
