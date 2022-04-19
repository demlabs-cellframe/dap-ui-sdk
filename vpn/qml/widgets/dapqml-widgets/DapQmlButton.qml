import QtQuick 2.1
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import DapQmlStyle 1.0

Rectangle {
    id: root

    /* signals */
    signal clicked();
    signal rightClicked();
    signal textEdited();
    signal textChanged();

    /* DEFS */

    enum Style
    {
        TopMainBottomSub,     ///< top:main, bottom:sub
        TopSubBottomMain,     ///< top:sub,  bottom:main
        LeftTopMainBottomSub, ///< left:left, top:main, bottom:sub
        IconMainSub,          ///< icon, main, sub
        EditTopMainBottomSub, ///< top:edit, bottom:sub
        IconMainSubIcon       ///< icon, main, sub, icon
    }

    /* VARS */

    property string mainText: "Main text"
    property string subText: "Sub text"
    property string leftText: "Left text"
    property string inputMask
    property string qss: ""
    property string mainQss: ""
    property string leftQss: ""
    property string subQss: ""
    property string icon: ""
    property string iconRight: ""
    property int iconSize: 34
    property int iconRightSize: 34
    property int buttonStyle: DapQmlButton.Style.TopMainBottomSub
    property bool separator: false
    property bool link: false
    property bool frame: false

    property var labelMain
    property var labelSub
    property var labelLeft
    property var labelIcon
    property var labelIconRight

    DapQmlStyle { id: style; qss: root.qss; item: root }

    MouseArea {
        anchors.fill: root;
        onClicked: root.clicked();
    }

    /* user no background */
    color: "transparent"

    /* store references */
    Component.onCompleted: {
        if(root.buttonStyle === DapQmlButton.Style.LeftTopMainBottomSub)
        {
            root.labelMain  = ltmbsMain;
            root.labelSub   = ltmbsSub;
            root.labelLeft  = ltmbsLeft;
        }
        else if(root.buttonStyle === DapQmlButton.Style.TopMainBottomSub)
        {
            root.labelMain  = tmbsMain;
            root.labelSub   = tmbsSub;
        }
        else if(root.buttonStyle === DapQmlButton.Style.TopSubBottomMain)
        {
            root.labelMain  = tsbmMain;
            root.labelSub   = tsbmSub;
        }
        else if(root.buttonStyle === DapQmlButton.Style.IconMainSub)
        {
            root.labelMain  = imsMain;
            root.labelSub   = imsSub;
            root.labelIcon  = imsIcon;
        }
        else if(root.buttonStyle === DapQmlButton.Style.EditTopMainBottomSub)
        {
            root.labelMain  = etmbsMain;
            root.labelSub   = etmbsSub;
        }
        else if(root.buttonStyle === DapQmlButton.Style.IconMainSubIcon)
        {
            root.labelMain      = imsiMain;
            root.labelSub       = imsiSub;
            root.labelIcon      = imsiIcon;
            root.labelIconRight = imsiRightIcon;
        }

    }

    /* functions */

    function _magickSpacer() {
        return (0.074 * height);
    }

    function _magickWidth() {
        return root.width;
    }

    function _magickHeight() {
        return root.height / 2;
    }

    function _magickSubPos() {
        return _magickHeight() + _magickSpacer();
    }

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
    DapQmlLabel {
        anchors.fill: parent
        z: -1
        visible: root.frame
        qss: "btn-frame"
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

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = ltmbsMain;
                    root.labelSub   = ltmbsSub;
                    root.labelLeft  = ltmbsLeft;
                }
            }

            /* left big label */
            DapQmlLabel {
                id: ltmbsLeft
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignVCenter
                text: root.leftText
                qss: root.leftQss
                onClicked: root.clicked();
            }

            /* right side two labels */
            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 1

                /* main text */
                DapQmlLabel {
                    id: ltmbsMain
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: text.length > 0

                    horizontalAlign: Text.AlignLeft
                    verticalAlign: root.subText.length > 0 ? Text.AlignBottom : Text.AlignVCenter
                    text: root.mainText
                    qss: root.mainQss
                    onClicked: root.clicked();
                }

                /* sub text */
                DapQmlLabel {
                    id: ltmbsSub
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: text.length > 0

                    horizontalAlign: Text.AlignLeft
                    verticalAlign: root.subText.length > 0 ? Text.AlignTop : Text.AlignVCenter
                    text: root.subText
                    qss: root.subQss
                    onClicked: root.clicked();
                }

            }
        }

        /* TopMainBottomSub */
        /* Two items by vertical */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.TopMainBottomSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = tmbsMain;
                    root.labelSub   = tmbsSub;
                }
            }

            /* main text */
            DapQmlLabel {
                id: tmbsMain
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignBottom
                text: root.mainText
                qss: root.mainQss
                onClicked: root.clicked();
            }

            /* sub text */
            DapQmlLabel {
                id: tmbsSub
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight()
                y: _magickSubPos()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.subText
                qss: root.subQss
                onClicked: root.clicked();
            }

        }

        /* TopSubBottomMain */
        /* Two items by vertical */
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.TopSubBottomMain)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = tsbmMain;
                    root.labelSub   = tsbmSub;
                }
            }

            /* sub text */
            DapQmlLabel {
                id: tsbmSub
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignBottom
                text: root.subText
                qss: root.subQss
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: tsbmMain
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: _magickWidth()
                height: _magickHeight() - _magickSpacer() / 2
                y: _magickHeight()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.mainText
                qss: root.mainQss
                onClicked: root.clicked();
            }

        }

        /* EditTopMainBottomSub */
        /* Two items by vertical with editable text field */
        Item {
            id: etmbs
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: (root.buttonStyle === DapQmlButton.Style.EditTopMainBottomSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = etmbsMain;
                    root.labelSub   = etmbsSub;
                }
            }

            /* main text */
            TextField {
                id: etmbsMain
                y: _magickSpacer()
                width: _magickWidth()
                height: _magickHeight() - _magickSpacer()

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                text: root.mainText
                //qss: root.mainQss
                inputMask: root.inputMask

                /* vars */
                property string fontFamiliy: "Lato"
                property int fontSize: 12
                property int fontWeight: Font.Normal

                /* style */
                DapQmlStyle { qss: root.mainQss; item: etmbsMain }

                /* background */
                background: DapQmlRectangle {
                    qss: "login-serialkey-input"
                }

                /* font config */
                font {
                    family: etmbsMain.fontFamiliy
                    pixelSize: etmbsMain.fontSize
                    weight: etmbsMain.fontWeight
                }

                /* signals */
                onTextEdited: { root.mainText = text; root.textEdited(); }
                onTextChanged: { root.mainText = text; root.textChanged(); }
            }

            /* sub text */
            DapQmlLabel {
                id: etmbsSub
                width: _magickWidth()
                height: _magickHeight()
                y: _magickSubPos()

                horizontalAlign: Text.AlignHCenter
                verticalAlign: Text.AlignTop
                text: root.subText
                qss: root.subQss
                onClicked: root.clicked();
            }

        }

        /* IconMainSub */
        /* Three items by horiontal */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
            visible: (root.buttonStyle === DapQmlButton.Style.IconMainSub)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = imsMain;
                    root.labelSub   = imsSub;
                    root.labelIcon  = imsIcon;
                }
            }

            /* icon */
            DapQmlLabel {
                id: imsIcon
                Layout.preferredWidth: root.iconSize
                Layout.preferredHeight: root.iconSize

                qss: root.icon
                width: root.iconSize
                height: root.iconSize
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: imsMain
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignVCenter
                text: root.mainText
                qss: root.mainQss
                clip: false
                onClicked: root.clicked();
            }

            /* sub text */
            DapQmlLabel {
                id: imsSub
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: text.length > 0
                Layout.fillHeight: true

                horizontalAlign: Text.AlignRight
                verticalAlign: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
                clip: false
                visible: text.length > 0
                onClicked: root.clicked();
            }
        }

        /* IconMainSubIcon */
        /* Three items by horiontal */
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 4
            visible: (root.buttonStyle === DapQmlButton.Style.IconMainSubIcon)

            /* store references */
            Component.onCompleted: {
                if(visible)
                {
                    root.labelMain  = imsiMain;
                    root.labelSub   = imsiSub;
                    root.labelIcon  = imsiIcon;
                }
            }

            /* icon */
            DapQmlLabel {
                id: imsiIcon
                Layout.preferredWidth: root.iconSize
                Layout.preferredHeight: root.iconSize

                qss: root.icon
                width: root.iconSize
                height: root.iconSize
                onClicked: root.clicked();
            }

            /* main text */
            DapQmlLabel {
                id: imsiMain
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true

                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignVCenter
                text: root.mainText
                qss: root.mainQss
                clip: false
                onClicked: root.clicked();
            }

            /* sub text */
            DapQmlLabel {
                id: imsiSub
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: text.length > 0
                Layout.fillHeight: true

                horizontalAlign: Text.AlignRight
                verticalAlign: Text.AlignVCenter
                text: root.subText
                qss: root.subQss
                clip: false
                visible: text.length > 0
                onClicked: root.clicked();
            }

            /* icon */
            DapQmlLabel {
                id: imsiRightIcon
                Layout.preferredWidth: root.iconRightSize
                Layout.preferredHeight: root.iconRightSize

                qss: root.iconRight
                width: root.iconRightSize
                height: root.iconRightSize
                onClicked: root.rightClicked();
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
