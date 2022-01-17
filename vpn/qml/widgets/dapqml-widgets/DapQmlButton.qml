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
    property string icon: ""
    property int iconSize: 34
    property int buttonStyle: DapQmlButton.Style.TopMainBottomSub
    property bool separator: false
    property bool link: false
    property bool frame: false

    DapQmlStyle { id: style; qss: root.qss; item: root }

    /* user no background */
    color: "transparent"

    /* link */
    Image {
        x: root.width - (width * 3)
        y: (root.height - height) / 2
        z: 1
        width: 22
        height: 22
        visible: root.link
        source: "qrc:/light/ic_arrow-right.png"
    }

    /* background */
    Image {
        anchors.fill: parent
        z: -1
        visible: root.frame
        source: "qrc:/light/btn_server_bg.png"
    }

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

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignVCenter
                text: root.leftText
                qss: root.leftQss
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

                    horizontalAlign: Text.AlignLeft
                    verticalAlign: Text.AlignBottom
                    text: root.mainText
                    qss: root.mainQss
                }

                /* sub text */
                DapQmlLabel {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlign: Text.AlignLeft
                    verticalAlign: Text.AlignTop
                    text: root.subText
                    qss: root.subQss
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

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignBottom
                text: root.mainText
                qss: root.mainQss
            }

            /* sub text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.subText
                qss: root.subQss
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

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignBottom
                text: root.subText
                qss: root.subQss
            }

            /* main text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.mainText
                qss: root.mainQss
            }

        }

        /* IconMainSub */
        /* Three items by horiontal */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
            visible: (root.buttonStyle === DapQmlButton.Style.IconMainSub)

            //Image {
            DapQmlLabel {
                Layout.preferredWidth: root.iconSize
                Layout.preferredHeight: root.iconSize

                qss: root.icon
                width: root.iconSize
                height: root.iconSize
            }

            /* main text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignVCenter
                text: root.mainText
                qss: root.mainQss
            }

            /* sub text */
            DapQmlLabel {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignRight
                verticalAlign: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
            }
        }

        /* bottom item */
        DapQmlSeparator {
            Layout.fillWidth: true
            width: root.width
            visible: root.separator
        }
    }

}
